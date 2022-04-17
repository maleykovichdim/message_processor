/**
 * @file speed_synchronisation.h
 *
 * Project:	util.h
 *
 * Common static function
 */

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;
using namespace std;

namespace message_processor
{
  class Utils {
  public:

    static shared_ptr<std::vector<fs::path>> getFilesList(const std::string& path) {
      std::vector<fs::path> files;
      files.reserve(1000);
      try {
        for (const auto& entry : fs::directory_iterator(path)) {
          if ( entry.is_regular_file() 
               && !entry.path().extension().compare(".txt")
            )
            files.push_back(entry.path());
        }
      }
      catch (...) { std::cout << "Error: Maybe Wrong folder path." << std::endl; }
      std::vector<uint8_t> mVector;
      return std::make_shared<std::vector<fs::path>>(std::move(files));
    }

    static inline std::string getTxtFileData(const fs::path& path) {
      std::ifstream current(path.string());
      std::string output;
      current >> output;
      return output;
    }

    template<class Type>
    static inline void parseStringIntoInternalType(const std::string& str, std::vector<Type>& v) {
      std::stringstream ss(str);
      while (ss.good()) {
        std::string substr;
        getline(ss, substr, ',');
        /// unreachable and give empty vector
      }
    }


    template<>
    static inline void parseStringIntoInternalType(const std::string& str, std::vector<string>& v) {
      std::stringstream ss(str);
      while (ss.good()) {
        std::string substr;
        getline(ss, substr, ',');
        v.push_back(substr);
      }
    }

    template<>
    static inline void parseStringIntoInternalType(const std::string& str, std::vector<unsigned long>& v) {
      std::stringstream ss(str);
      while (ss.good()) {
        std::string substr;
        getline(ss, substr, ',');
        try {
          v.push_back(std::stoul(substr, nullptr, 10));
        }
        catch (...) {};
      }
    }

    template<>
    static inline void parseStringIntoInternalType(const std::string& str, std::vector<char>& v) {
      std::stringstream ss(str);
      while (ss.good()) {
        std::string substr;
        getline(ss, substr, ',');
        try {
          v.push_back(substr[0]);
        }
        catch (...) {};
      }
    }

    template<>
    static inline void parseStringIntoInternalType(const std::string& str, std::vector<double>& v) {
      std::stringstream ss(str);
      while (ss.good()) {
        std::string substr;
        getline(ss, substr, ',');
        try {
          std::string::size_type sz;
          v.push_back(std::stod(substr, &sz));
        }
        catch (...) {};
      }
    }

    static  bool createOrRewriteFile(const std::string& str) {
      FILE* filepoint;
      errno_t err;
      err = fopen_s(&filepoint, str.c_str(), "w");
      if (!filepoint) return false;
      fclose(filepoint);
      return true;
    }

    static  FILE* openFile(const std::string& str) {
      FILE* filepoint;
      errno_t err;
      err = fopen_s(&filepoint, str.c_str(), "rb+");
      return filepoint;
    }

    static inline long fileSize(FILE* file) {
      long currentPosition = ftell(file);
      fseek(file, 0, SEEK_SET);
      fseek(file, 0, SEEK_END);
      long size = ftell(file);
      fseek(file, currentPosition, SEEK_SET);
      return size;     
    }

    static inline long fileReadBlock(FILE* file, long fromPosition, char* externalBuffer, long byteToRead) {
      fseek(file, fromPosition, SEEK_SET);
      long take = fread((void*)externalBuffer, byteToRead, 1, file);    
      return take;
    }

    static inline long fileWriteBlock(FILE* file, long fromPosition, const char* externalBuffer, long byteToWrite) {
      fseek(file, fromPosition, SEEK_SET);
      long written = fwrite((void*)externalBuffer, 1, byteToWrite, file);
      return written;
    }

    static void closeFile(FILE* f) {
      fclose(f);
    }
  };
}
