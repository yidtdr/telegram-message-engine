#include "messages/messagedb.h"
#include "parsing/message_extractor.h"
#include <core/multithreading/pipeline.h>
#include <functional>
#include <gumbo.h>
#include <pybind11/embed.h>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace py = pybind11;

std::string readFileToString(const std::string& filePath) {
    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1) {
        throw std::runtime_error("Could not open file");
    }
    
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        close(fd);
        throw std::runtime_error("Could not get file size");
    }
    
    char* pData = static_cast<char*>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (pData == MAP_FAILED) {
        close(fd);
        throw std::runtime_error("Memory mapping failed");
    }
    
    std::string content(pData, sb.st_size);
    
    munmap(pData, sb.st_size);
    close(fd);
    
    return content;
}

void printMessage(Message msg) {
	auto&& data = msg.getData();
	std::cout << data.sender << ": \"" << data.content << "\" At:" << data.timestamp << "Id: " << data.id << std::endl;
}

// Test main

int main(){

	auto start = std::chrono::high_resolution_clock::now();
	
	MessageDatabase& db = MessageDatabase::getInstance();
	db.reserve(100000);

	std::setlocale(LC_ALL, "");
	

	MessageExtractor& ME = MessageExtractor::getInstance();

	const auto&& selector = MessageQuery(MessageQueryParameters{.sender = "Егор"});

	Pipeline<std::vector<Message>> pipeline(6);
	
	std::vector<std::string> files;
	std::vector<std::future<std::vector<Message>>> resFutures;
	files.reserve(102);
	files.push_back("./data/messages.html");
	for (int i = 2; i < 101; i++) {
		files.push_back("./data/messages" + std::to_string(i) + ".html");
	}

	for (auto& file : files) {
		std::function<std::vector<Message>()> func = [&] {
			std::string h = readFileToString(file);
			auto doc = gumbo_parse(h.c_str());
			const auto& res = ME.extract(doc->root);
			gumbo_destroy_output(&kGumboDefaultOptions, doc);

			return res;
		};

		resFutures.push_back(pipeline.submitTask(func));
	}

	for (auto& resFuture : resFutures) {
		auto res = resFuture.get();
		for (auto&& msg : res) {
			db.insert(std::move(msg));
		}
	}

	const auto messages = db.select(selector);
	//for (auto&& msg : messages) {
	//	printMessage(std::move(msg));
	//}

	std::cout << std::endl
		<< messages.size()
		<< std::endl;

	auto stop = std::chrono::high_resolution_clock::now();

	auto dur = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	std::cout << dur.count();

	try {
		py::scoped_interpreter guard{};

		py::module_ sys = py::module_::import("sys");
		sys.attr("path").attr("append")("./scripts");

		py::module_ script = py::module_::import("analysis");

		std::vector<std::string> msgs = {"Hello", "World"};
    		py::list py_msgs;
    		for (const auto& msg : msgs) {
        		py_msgs.append(py::cast(msg));
    		}
    
		py::object result = script.attr("analyze")(py_msgs);

		for (auto item : result) {
			std::cout << item.cast<std::string>() << "\n";
		}
	} catch (py::error_already_set &e) {
		std::cout << "Python error:" << e.what();
	}

	return 0;
}
