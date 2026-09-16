#pragma once

#include <iostream>
#include <cassert>
#include <type_traits>

void clear_console();

void wait_enter();

size_t expand_cchain(unsigned char*&, size_t, size_t);

template <typename T>
requires std::is_pointer_v<T>
bool invalid_memory_reservation(T ptr) {
  if (ptr == nullptr) {
    std::cerr << "\n\nERROR: MEMORY RESERVATION FAILED";
    wait_enter();
    return true;
  }
  return false;
}

template <typename T>
bool invalid_input(T& variable) {
  if (!(std::cin >> variable && std::cin.peek() == '\n')) {
    std::cerr << "\n\nERROR: INVALID INPUT";
    std::cin.clear();
    wait_enter();  
    return true;
  }
  return false;
}
