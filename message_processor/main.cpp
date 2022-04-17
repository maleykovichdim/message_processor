/**
 * @file main.cpp
 * 
 * Project:	message processor
 *
 * execution and main tests
 */

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

#include "utils.h"
#include "message_types.h"
#include "manager.h"

namespace fs = std::filesystem;
using namespace message_processor;
using namespace std;

const string INPUT_PATH = "..\\input";
const string OUTPUT_FILE_PATH = "output";

template< typename T >
void test_launch(
  shared_ptr<std::vector<fs::path>>& files,
  const char* addon,
  int numTreads,
  int limitRequests,
  double limitForSecond
) {
  Manager<T> manager(numTreads);
  auto result = manager.init(MessageType::AnyType, files, limitRequests, limitForSecond, OUTPUT_FILE_PATH+addon+".txt");
  if (result.has_value()) {
    cout << result.value() << endl;
    return;
  }
  manager.start();
}

void test_launch_batch(
  shared_ptr<std::vector<fs::path>>& files,
  const string& addon,
  int numTreads,
  int limitRequests,
  double limitForSecond
) {
  test_launch<unsigned long>( files, (addon+"_ul").c_str(), numTreads, limitRequests, limitForSecond );
  test_launch<char>(files, (addon + "_char").c_str(), numTreads, limitRequests, limitForSecond);
  test_launch<string>(files, (addon + "_string").c_str(), numTreads, limitRequests, limitForSecond);
  test_launch<double>(files, (addon + "_double").c_str(), numTreads, limitRequests, limitForSecond);
}


int main()
{
  //get list of file paths from specified folder
  auto files = Utils::getFilesList(INPUT_PATH);

  //TEST block
  test_launch_batch(files, "_1thread_1requestPerSec", 1, 1, 0.001);
  test_launch_batch(files, "_4thread_20requestPerSec", 4, 20, 1.0);
  test_launch_batch(files, "_8thread_2000000requestPerSec", 8, 2000000, 2.0);
  cout << "Done" << endl;
}