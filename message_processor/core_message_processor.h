/**
 * @core_message_processor.h
 *
 * Project:	message processor
 *
 * read files, parse them. Class has it`s own local storage with unique elements. Class is used in Multithread mode.
 */


#pragma once

#include "speed_synchronisation.h"
#include "utils.h"

#include <unordered_set>
#include <string>

namespace fs = std::filesystem;

namespace message_processor
{

  template< typename T >
  class CoreMessageProcessor 
  {
  public:

    void init(SpeedSync* speedSync);
    void putData(const std::shared_ptr<std::vector<fs::path>>& files, int startPosition, int offset);
    const std::unordered_set<T>& getStorage() const;
    void clearStorage();

  private:
    std::unordered_set<T> accumulator;
    SpeedSync* speedSync{ nullptr };
    mutable std::atomic<int> status{ 0 };
  };
}
