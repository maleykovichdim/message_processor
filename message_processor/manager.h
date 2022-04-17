/**
 * @file manager.h
 *
 * Project:	message processor
 *
 *   Main manager
 */

#pragma once

#include "message_types.h"
#include "core_message_processor.h"
#include "file_handler.h"
#include "speed_synchronisation.h"

#include <string.h>
#include <vector>
#include <optional>
#include <thread>
#include <future>

namespace message_processor
{

	template< typename T >
	class Manager
	{
	public:

		explicit Manager(unsigned int numThreads = 4);
		~Manager();


		//params:
		//MessageType messageType - not used, only for setting
		//std::shared_ptr<std::vector<fs::path>> files - list of paths of files with messages
		//long long numRequests - X from (restriction: X requests in Y second)
		//double forSecond - Y from (restriction: X requests in Y second)
		//const std::string& outputPath - path for output data file
		std::optional<std::string> init(
			MessageType messageType,
			std::shared_ptr<std::vector<fs::path>> files,
			long long numRequests,
			double forSecond,
			const std::string& outputPath);

		bool start();

	private:

		int run(int num);
		void processLoop();

	private:
		bool isSet = false;
		std::atomic<int> isDone{ 0 };
		SpeedSync speedSync;
		MessageType messageType;
		unsigned int numThreads;
		std::vector<std::future<int>> threads;
		std::shared_ptr<std::vector<fs::path>> filesList;
		CoreMessageProcessor<T>* coreMessageProcessors = nullptr;
		FileHandler<T>* fileHandler = nullptr;
	};
}
