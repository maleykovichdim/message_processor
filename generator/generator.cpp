#include "pch.h"
#include <iostream>
#include <iomanip>
#include <iostream>
#include <random>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <string>

const std::string OUTPUT_PATH = "..\\input\\infile";

const int NUM_FILES = 1000;
const int MIN_NUMBER_OF_DIGITS = 5;
const int MAX_NUMBER_OF_DIGITS = 10;

int main()
{ 
  int numElements = NUM_FILES * MAX_NUMBER_OF_DIGITS;
  std::vector<int> v(numElements);
  std::iota(v.begin(), v.end(), 1);//todo
  int endPoint = numElements * 0.7;
  for (int i = endPoint; i < numElements; i++) {
    v[i] = rand() % (endPoint - 3) + 1;
  }
  for (int i = endPoint; i < numElements; i++) {
    int index = (i - endPoint) * 2;
    int temp = v[index];
    v[index] = v[i];
    v[i] = temp;
  }
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(v.begin(), v.end(), g);

  int counter = 0;
  int fileNumber = 1;
  while (counter < numElements) {
    int currentCounter = MIN_NUMBER_OF_DIGITS +
      rand() % (MAX_NUMBER_OF_DIGITS - MIN_NUMBER_OF_DIGITS);
    if (counter + currentCounter > numElements)
      currentCounter = numElements - counter; 

    std::string str = std::to_string(fileNumber++);
    int length = str.length();
    if (length < 6) {
      int a = 6 - length;
      while (a--) str = '0' + str;
    }
    str = OUTPUT_PATH + str + ".txt";
    std::ofstream myfile(str);
    if (myfile.is_open())
    {
      for (int i = counter; i < counter + currentCounter; i++) {
        myfile << v[i];
        if (i != counter + currentCounter - 1) myfile << ',';
      }
      myfile.close();
    }
    else std::cout << "Unable to open file";
    counter += currentCounter;
  }
}

