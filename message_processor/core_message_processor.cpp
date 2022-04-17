/**
 * @core_message_processor.h
 *
 * Project:	message processor
 *
 * read files, parse them. It is used in Multithread mode.
 */

#include "core_message_processor.h"

#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace message_processor
{

  const unsigned long long FIRST_DELAY_MS = 1;
  const unsigned long long MAXIMUM_DELAY_MS = 5000;//10 sec -  stop waiting. it will be two times.

    template<typename T>
    void CoreMessageProcessor<T>::init(SpeedSync* speedSync)
    {
      this->speedSync = speedSync;
      status = 1;
    }

    template<typename T>
    void CoreMessageProcessor<T>::putData(const std::shared_ptr<std::vector<fs::path>>& files, int startPosition, int offset)
    {
      int currentPosition = startPosition;
      int status = 1;
      unsigned long long delayMS = FIRST_DELAY_MS;
      unsigned int delayCounter = 0;
      std::vector<T> elements;
      elements.reserve(10);
      while (currentPosition < files.get()->size()) {
        if (!speedSync->askToContinueParsing()) status = 0; else status = 1;
        if (status) {
          std::string str = message_processor::Utils::getTxtFileData((files.get())->at(currentPosition));
          //type
          message_processor::Utils::parseStringIntoInternalType<T>(str, elements);
          for (const auto& element : elements)
            accumulator.insert(element);
          elements.clear();
          currentPosition += offset;
          if (currentPosition >= files.get()->size()) break;//
          if (delayCounter > 4) delayMS = delayMS * (delayCounter - 2);
          if (delayCounter == 1) delayMS /= 2;
          if (delayMS < 1) delayMS = 1;
          delayCounter = 0;
        }
        else {
          delayCounter++;
          std::this_thread::sleep_for(std::chrono::milliseconds(delayMS));
        }
      }
    }

    template<typename T>
    const std::unordered_set<T>& CoreMessageProcessor<T>::getStorage() const {
      return accumulator;
    }

    template<typename T>
    void CoreMessageProcessor<T>::clearStorage() { accumulator.clear(); }


#define XCORE(XTYPE) \
    template\
      void CoreMessageProcessor<XTYPE>::init(SpeedSync* speedSync);\
    template\
      void CoreMessageProcessor<XTYPE>::putData(const std::shared_ptr<std::vector<fs::path>>& files, int startPosition, int offset);\
    template\
      const std::unordered_set<XTYPE>& CoreMessageProcessor<XTYPE>::getStorage() const;\
    template\
      void CoreMessageProcessor<XTYPE>::clearStorage();\

    XCORE(unsigned long)
    XCORE(char)
    XCORE(string)
    XCORE(double)

}
