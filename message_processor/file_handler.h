/**
 * @file file_handler.h
 *
 * Project:	message processor
 *
 * final sorted storage with unique elements for messages and saving storage block by block into file
 */

#pragma once

#include "utils.h"

#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <unordered_set>

using namespace std;

namespace message_processor
{

  template< typename T >
  class FileHandler {
  public:

    FileHandler(string fileName);
    ~FileHandler();
    void putToStorage(const std::unordered_set<T>& accumulator);
    void saveToFile() const;
    void dumpChangingData();//STUB

  private:

    string fileName;
    char* buffer{nullptr};
    set<T> storage;
    //mutable std::mutex mutex;
  };

}
