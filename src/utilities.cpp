#include <iostream>
#include <limits>

#include "utilities.hpp"

bool invalid_memory_reservation(unsigned char* ptr) {
  if (ptr == nullptr) {
    std::cerr << "\n\nERROR: MEMORY RESERVATION FAILED";
    wait_enter();
    return true;
  }
  return false;
}

bool invalid_input(short& variable) {
  if (!(std::cin >> variable && std::cin.peek() == '\n')) {
    std::cerr << "\n\nERROR: INVALID INPUT";
    std::cin.clear();
    wait_enter();  
    return true;
  }
  return false;
}

bool invalid_input(unsigned short& variable) {
  if (!(std::cin >> variable && std::cin.peek() == '\n')) {
    std::cerr << "\n\nERROR: INVALID INPUT";
    std::cin.clear();
    wait_enter();  
    return true;
  }
  return false;
}

bool invalid_input(unsigned char& variable) {
  if (!(std::cin >> variable && std::cin.peek() == '\n')) {
    std::cerr << "\n\nERROR: INVALID INPUT";
    std::cin.clear();
    wait_enter();  
    return true;
  }
  return false;
}

bool invalid_input(size_t& variable) {
  if (!(std::cin >> variable && std::cin.peek() == '\n')) {
    std::cerr << "\n\nERROR: INVALID INPUT";
    std::cin.clear();
    wait_enter();  
    return true;
  }
  return false;
}


void clear_console() {
  #if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
  #else
    std::system("clear");
  #endif
}

void configureUTF8() {
  #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // 2. (Opcional) Activar secuencias de escape ANSI si usas colores
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
      DWORD dwMode = 0;
      if (GetConsoleMode(hOut, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
      }
    }  
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



