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

void resize_cchain(unsigned char*& c_chain, const size_t new_capacity, const size_t bytes_to_copy) {
    unsigned char* array_aux = new (std::nothrow) unsigned char[new_capacity];
    if (invalid_memory_reservation(array_aux)) return;

    // Copiar solo hasta el límite seguro (el menor entre lo nuevo y lo viejo)
    size_t limit = (bytes_to_copy < new_capacity) ? bytes_to_copy : new_capacity;

    for (size_t index = 0; index < limit; ++index) {
        array_aux[index] = c_chain[index];
    }

    delete[] c_chain;
    c_chain = array_aux;
}



