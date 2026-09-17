#include "lib.hpp"
#include "utilities.hpp"

#include <iostream>

constexpr short token_size = 3, option_limit = 2;

int main() {
  while (true) {
    clear_console();
    //print_title();

    size_t rows, columns;

    do {
      std::cout << "\n\nEnter the number of rows of the board: ";
    } while(invalid_input(rows));

    if (rows == 0) break;
    
    do {
      std::cout << "\n\nEnter the number of columns of the board: ";
    } while(invalid_input(columns));
    
    if (columns == 0) break;

    size_t used_tokens = rows*columns;
    size_t char_capacity = (( (used_tokens*token_size) + 7)/8);
    //size_t token_capacity = (char_capacity*8)/token_size;

    unsigned char* board = nullptr;
    board = new (std::nothrow) unsigned char[char_capacity];
    if (invalid_memory_reservation(board)) return 1;

    create_board(char_capacity, used_tokens, board);

    while (true) {
      char selection;
      clear_console();
      print_board(columns, char_capacity, used_tokens, board);
      do {
        std::cout << "\n\n "; 
      } while (invalid_input(selection) || (selection < 0 || selection > option_limit) );

      switch(selection) {
        case '1': 
          break;
      }

      
    }
  }
  return 0;
}
