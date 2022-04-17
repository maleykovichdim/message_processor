/**
 * @file file_handler.cpp
 *
 * Project:	message processor
 *
 * final sorted storage with unique elements for messages and saving storage block by block into file
 */


#include "file_handler.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <condition_variable>
#include <mutex>
#include <shared_mutex >

namespace message_processor
{

  const int BUFFER_SIZE = 1024;//todo: analyse it

  template < typename T>
  FileHandler<T>::FileHandler(string fileName) {
    this->fileName = fileName;
    buffer = new char[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
  };

  template < typename T>
  FileHandler<T>::~FileHandler() {
    delete[] buffer;
  };

  template < typename T>
  void FileHandler<T>::putToStorage(const std::unordered_set<T>& accumulator) {
    //std::lock_guard<std::mutex> lock(mutex);
    for (const auto& element : accumulator) {
      storage.insert(element);
    }
  };

  template < typename T>
  void FileHandler<T>::saveToFile() const {
    auto file = message_processor::Utils::openFile(fileName);
    if (file == nullptr) return;
    long fileSize = Utils::fileSize(file);
    int currentFilePosition = 0;
    std::stringstream ss;
    for (auto& element : storage) {
      ss << element << ',';
      long currentLength = ss.str().length();
      if (currentLength > BUFFER_SIZE / 2) {
        if (currentLength > BUFFER_SIZE) currentLength = BUFFER_SIZE;
        string temp = ss.str();
        const char* inMemory = temp.c_str();
        long taken = Utils::fileReadBlock(file, currentFilePosition, buffer, currentLength);
        if (taken < currentLength) {
          //dumpChangingData();//STUB
          //try to write block
          long written = Utils::fileWriteBlock(file, currentFilePosition, inMemory, currentLength);
          if (written < currentLength) {
            return;
          }
          currentFilePosition += written;
          ss.str("");
          continue;
        }
        else {
          if (!memcmp(inMemory, buffer, taken)) {
            //pass block without changes
            currentFilePosition += taken;
          }
          else
          {
            //dumpChangingData();//STUB
            //try to rewrite block
            long written = Utils::fileWriteBlock(file, currentFilePosition, inMemory, currentLength);
            if (written < currentLength) {
              return;
            }
            currentFilePosition += written;
          }
          ss.str("");
          continue;
        }
      }
    };

    string temp = ss.str();
    if (temp.size()) {
      const char* inMemory = temp.c_str();
      //dumpChangingData();//STUB
      long written = Utils::fileWriteBlock(file, currentFilePosition, inMemory, temp.size());
    }
    fclose(file);
  };

  template < typename T>
  void FileHandler<T>::dumpChangingData() {
    //STUB for TRANSACTION approach
    //function to saveToFile changed fragments of files 
    //in case of hardware or software failure
    //to restore the file on disk
  };




//
#define HANDLER(XTYPE)\
  template\
    FileHandler<XTYPE>::FileHandler(string fileName);\
  template\
    FileHandler<XTYPE>::~FileHandler();\
  template\
    void FileHandler<XTYPE>::putToStorage(const std::unordered_set<XTYPE>& accumulator);\
  template\
    void FileHandler<XTYPE>::saveToFile() const;\
  template\
    void FileHandler<XTYPE>::dumpChangingData();\

  HANDLER(unsigned long)
  HANDLER(char)
  HANDLER(string)
  HANDLER(double)

}