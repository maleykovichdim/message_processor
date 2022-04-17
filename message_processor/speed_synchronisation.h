/**
 * @file speed_synchronisation.h
 *
 * Project:	message processor
 *
 * Class is created to solving problem with receiving rate i.e. module should not process more than X requests in Y seconds.
 */

#pragma once

#include <condition_variable>
#include <mutex>
#include <chrono>
using namespace std;
using namespace std::chrono;

namespace message_processor
{

	class SpeedSync {
	public:

		void init(int numThreads, long long numRequests, double forSeconds);
		void setStartTime();
		bool askToContinueParsing();
		void tryForcedStopAllBecauseTimeout();//
		void waitForSavingMomentAndExecutionTimeEstimation();
		void doTimeDelayIfNedeedAndRestartParsing();
		void finishAll();

	private:
		std::atomic<int> processedFileQuantity{ 0 };
		std::atomic<int> forsedStopCounter{ 0 };
		std::atomic<bool> active;
		bool isDone = false;

		mutable std::condition_variable condVar;
		mutable std::mutex mutex;
		steady_clock::time_point startTime;
		long long numRequests = 1;
		double forSeconds = 1;
		int numThreads = 1;
	};
}
