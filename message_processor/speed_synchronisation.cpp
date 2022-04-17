/**
 * @file speed_synchronisation.cpp
 *
 * Project:	message processor
 *
 * Class is created to solving problem with receiving rate i.e. module should not process more than X requests in Y seconds.
 */

#include "speed_synchronisation.h"

namespace message_processor
{

		void SpeedSync::init(int numThreads, long long numRequests, double forSeconds) {
			this->numThreads = numThreads;
			this->numRequests = numRequests;
			this->forSeconds = forSeconds;
			active = true;
		}

		void SpeedSync::setStartTime() { startTime = steady_clock::now(); }

		bool SpeedSync::askToContinueParsing() {
			if (!active.load()) {
				condVar.notify_one();
				return false;
			}
			if (++processedFileQuantity > numRequests) {
				active = false;
				condVar.notify_one();
				return false;
			}
			return true;
		}

		void SpeedSync::tryForcedStopAllBecauseTimeout() {
			forsedStopCounter++;
			if (forsedStopCounter >= numThreads) {
				active = false;
				condVar.notify_one();
				forsedStopCounter = 0;
			}
		}

		void SpeedSync::waitForSavingMomentAndExecutionTimeEstimation() {
			std::unique_lock<std::mutex> lck(mutex);
			condVar.wait(lck, [&] { return !active.load() || isDone; });
		}

		void SpeedSync::doTimeDelayIfNedeedAndRestartParsing() {

			auto currentTime = steady_clock::now();
			auto duration = duration_cast<milliseconds>(currentTime - startTime).count();
			long long difference = (long long)(forSeconds * 1000) - duration;

			if (difference > 0) {
				std::this_thread::sleep_for(std::chrono::milliseconds(difference));
			}
			startTime = steady_clock::now();
			processedFileQuantity = 0;
			active = true;
		}

		void SpeedSync::finishAll() {
			isDone = true;
			condVar.notify_one();
		}

}
