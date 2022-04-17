/**
 * @file manager.h
 *
 * Project:	message processor
 *
 *   Main manager
 */


#include "manager.h"

#include <memory>
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <condition_variable>
#include <mutex>
#include <shared_mutex >

#include <queue>

#include <utility>
#include <functional>

namespace message_processor
{

		template< typename T >
		Manager<T>::Manager(unsigned int numThreads)
		{
			if (numThreads < 1 || numThreads > 128) numThreads = 1;
			this->numThreads = numThreads;
		}

		template< typename T >
		Manager<T>::~Manager()
		{
			for (unsigned int i = 0; i < threads.size(); i++) threads[i].wait();
			delete[] coreMessageProcessors;
			delete fileHandler;
		}

		template< typename T >
		std::optional<std::string> Manager<T>::init(
			MessageType messageType,
			std::shared_ptr<std::vector<fs::path>> files,
			long long numRequests,
			double forSecond,
			const std::string& outputPath)
		{
			isSet = false;
			if (numRequests < numThreads)
				return std::optional<std::string>{"number of requests must be more than number of threads"};
			if (numRequests < 1 || forSecond <= 0.0) return std::optional<std::string>{"speed limit errors"};
			speedSync.init(numThreads, numRequests, forSecond);
			if (coreMessageProcessors) delete[] coreMessageProcessors;
			this->messageType = messageType;
			if (files.get() == NULL || !files.get()->size()) return std::optional<std::string>{"input list errors"};
			this->filesList = files;
			if (!Utils::createOrRewriteFile(outputPath)) return std::optional<std::string>{"recreation file error"};
			fileHandler = new FileHandler<T>(outputPath);
			coreMessageProcessors = new CoreMessageProcessor<T>[numThreads];
			for (int i = 0; i < numThreads; i++)
				coreMessageProcessors[i].init(&speedSync);
			isSet = true;
			return std::nullopt;
		}

		template< typename T >
		bool Manager<T>::start()
		{
			if (!isSet) return false;
			for (unsigned int i = 0; i < numThreads; i++)
			{
				threads.push_back(std::async(std::launch::async, &Manager::run, this, i));
			}
			speedSync.setStartTime();
			processLoop();
			return true;
		}


		template< typename T >
		int Manager<T>::run(int num)
		{
			coreMessageProcessors[num].putData(filesList, num, numThreads);
			isDone++;
			if (isDone >= numThreads) {
				speedSync.finishAll();
			}
			return 0;
		}


		template< typename T >
		void Manager<T>::processLoop() {
			while (true) {
				speedSync.waitForSavingMomentAndExecutionTimeEstimation();
				if (fileHandler) {
					int flag = 0;
					for (unsigned int i = 0; i < numThreads; i++)
					{
						if (coreMessageProcessors[i].getStorage().empty()) flag++;
						fileHandler->putToStorage(coreMessageProcessors[i].getStorage());
					}
					if (flag == numThreads) break;
					fileHandler->saveToFile();

				}
				if (isDone >= numThreads) break;
				speedSync.doTimeDelayIfNedeedAndRestartParsing();
			}
		}


#define XMANAGER(XTYPE) \
		template\
			Manager<XTYPE>::Manager(unsigned int numThreads);\
		template\
			Manager<XTYPE>::~Manager();\
		template\
			std::optional<std::string> Manager<XTYPE>::init(\
				MessageType messageType,\
				std::shared_ptr<std::vector<fs::path>> files,\
				long long numRequests,\
				double forSecond,\
				const std::string& outputPath);\
		template\
			bool Manager<XTYPE>::start();\
		template\
			int Manager<XTYPE>::run(int num);\
		template\
			void Manager<XTYPE>::processLoop();\


		XMANAGER(unsigned long)
		XMANAGER(char)
		XMANAGER(string)
		XMANAGER(double)

}
