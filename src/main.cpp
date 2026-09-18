#include "lib.hpp"
#include "utilities.hpp"

#include <iostream>



int main() {
  while (true) {
    clear_console();
    print_title();

    size_t rows, columns;

    std::cout << "[ENTER 0 TO EXIT]";
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

    unsigned char* board = nullptr;
    board = new (std::nothrow) unsigned char[char_capacity];
    if (invalid_memory_reservation(board)) return 1;

    create_board(char_capacity, used_tokens, board);

    while (true) {
      short selection = 0;
      clear_console();
      print_options();
      print_board(columns, char_capacity, used_tokens, board);

      std::cout << "\n\nSelect one of the options from above: ";
      if (invalid_input(selection)) continue;
          
      
      if ((selection <= 0) || (selection > option_limit)) {
          std::cout << "\n\nERROR: OPTION OUT OF RANGE";
          wait_enter();
          continue;
      } 
      

      switch(selection) {
        case 1:
          unsigned short x, y;
          std::cout << "\n\n| DELETE TOKEN |\n\n"
                    << "[Enter the Coordenates of the token] X: ";
          if (invalid_input(x)) break;

          if (x < 0 || x >= columns) {
            std::cout << "\n\nERROR: INVALID COORDENATE";
            wait_enter();
            break;
          }
          std::cout << "\n\n                                     Y: ";
          if (invalid_input(x)) break;

          if (x < 0 || x > (rows-1)) {
            std::cout << "\n\nERROR: INVALID COORDENATE";
            wait_enter();
            break;
          }

          //cascaded_fall();

          break;

        case 2:
          unsigned short deleted_row;
          std::cout << "\n\n| DELETE ROW |\n\n"
                    << "[Enter the Coordenates of the row]: ";
          if (invalid_input(deleted_row)) break;

          if (deleted_row < 0 || deleted_row >= rows) {
            std::cout << "\n\nERROR INVALID ROW";
            wait_enter();
            break;
          }

          used_tokens = delete_row(board, rows, columns, deleted_row, char_capacity);

          break;

        case 3:
          unsigned short deleted_column; 
          std::cout << "\n\n| DELETE COLUMN |\n\n"
                    << "[Enter the Coordenates of the column]: ";
          if (invalid_input(deleted_column)) break;
          
          if (deleted_column < 0 || deleted_column >= columns) {
            std::cout << "\n\nERROR INVALID COLUMN";
            wait_enter();
            break;
          }

          eliminarcolumna(board, rows, columns, deleted_column, char_capacity);
          used_tokens = rows*columns;
          //funcion

          break;

        case 4:
          unsigned short added_row;
          std::cout << "\n\n| DELETE ROW |\n\n"
                    << "[Enter the Coordenates of the row]: ";
          if (invalid_input(added_row)) break;

          if (added_row < 0 || added_row >= rows) {
            std::cout << "\n\nERROR INVALID ROW";
            wait_enter();
            break;
          }

          //funcion

          break;

        case 5:
          unsigned short added_column;

          std::cout << "\n\n| DELETE COLUMN |\n\n"
                    << "[Enter the Coordenates of the column]: ";
          if (invalid_input(added_column)) break;
          
          if (added_column < 0 || added_column >= columns) {
            std::cout << "\n\nERROR INVALID COLUMN";
            wait_enter();
            break;
          }

          //funcion

          break;
      }

      
    }
  }
  return 0;
}
