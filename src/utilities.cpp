#include <cstdlib>
#include <iostream>
#include <limits>

#include "utilities.hpp"

void clear_console() {
  #if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
  #else
    std::system("clear");
  #endif
}

void wait_enter() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cin.get();
}

size_t expand_cchain(unsigned char*& c_chain, size_t capacity, const size_t size_chain) {
  capacity *= 2;
  unsigned char* array_aux = new (std::nothrow) unsigned char[capacity];
  if (invalid_memory_reservation(array_aux)) {
    delete[] c_chain;
    return 0;
  }
  for (size_t index = 0; index < size_chain; ++index) array_aux[index] = c_chain[index];
  
  delete[] c_chain;
  c_chain = array_aux;
  return capacity;
}
/*
size_t shrink_cchain(unsigned char*& c_chain, size_t capacity, const size_t initial_size) {
  capacity = (capacity*0.70) + 1;
  unsigned char* array_aux = new (std::nothrow) unsigned char[capacity];
    if (invalid_memory_reservation(array_aux)) {
    delete[] c_chain;
    return 0;
  }
}
*/
