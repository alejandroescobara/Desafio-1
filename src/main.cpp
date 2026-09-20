#include <iostream>
#include <cstdlib>

#include "lib.hpp"
#include "utilities.hpp"

int main() {
  #ifdef _WIN32
    system("chcp 65001 > nul"); // Configura UTF-8 en Windows
    system("color");            // Activa los colores ANSI en Windows
  #endif
  while (true) {
    clear_console();
    print_title();

    size_t rows, columns;
    unsigned char print_bits = 0;

    std::cout << "[ENTER 0 TO EXIT]";
    do {
      std::cout << "\n\nEnter the number of rows of the board: ";
    } while(invalid_input(rows));

    if (rows == 0) break;
    
    do {
      std::cout << "\n\nEnter the number of columns of the board: ";
    } while(invalid_input(columns));
    
    if (columns == 0) break;

    std::cout << "\n\nDo you want to print the tokens as their bit value? (y/n): ";

    while(invalid_input(print_bits) || (print_bits != 'y' && print_bits != 'n')) {
      std::cout << "\n\nDo you want to print the tokens as their bit value? (y/n): ";
    }

    size_t used_tokens = rows*columns;
    size_t char_capacity = (( (used_tokens*token_size) + 7)/8);

    unsigned char* board = nullptr;
    board = new (std::nothrow) unsigned char[char_capacity];
    if (invalid_memory_reservation(board)) return 1;

    create_board(char_capacity, used_tokens, board);

    short selection = 0;

    while (true) {
      //### COMBO SCANNER |

      bool combo_found = true;

      while (combo_found) {
        combo_found = false;
        size_t token_counter = 0;

        while (token_counter < used_tokens) {

          const size_t c_bit_index = token_counter * token_size;
          const unsigned char token = get_token(board, c_bit_index);

          if (token != 0b11000000) {
            if (combo_scanner(board, columns, rows, c_bit_index)) {
              ++global_combo_counter;
              ++cascaded_counter;
              combo_found = true;
              break;
            }
          }

          ++token_counter;
        }
      }

      if (cascaded_counter > 0) {
        --cascaded_counter;
        score += 20+(cascaded_counter*30);
      }

      if (selection == 0) {
        global_combo_counter = 0;
        del_counter          = 0;
        score                = 0;
        cascaded_counter     = 0;
      }

      //###################

      selection = 0;
      clear_console();
      print_options();
      print_history();
      cascaded_counter = 0;

      if (print_bits == 'y') print_bit_board(columns, used_tokens, board);
      std::cout << "\n";
      print_board(columns, used_tokens, board);

      std::cout << "\n\nSelect one of the options from above: ";
      if (invalid_input(selection)) continue;
          
      
      if ((selection < 0) || (selection > option_limit)) {
          std::cout << "\n\nERROR: OPTION OUT OF RANGE";
          wait_enter();
          continue;
      } 
      
      if (selection == 6) break;

      switch(selection) {
        case 1: {
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
          if (invalid_input(y)) break;

          if (y < 0 || y > (rows-1)) {
            std::cout << "\n\nERROR: INVALID COORDENATE";
            wait_enter();
            break;
          }

          size_t global_index = ((y*columns) + x)*token_size;
          cascaded_fall(board, columns, global_index);
          ++del_counter;
          score += 10;

          break;
        }
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
          ++del_counter;

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
          ++del_counter;

          break;

        case 4:
          unsigned short added_row;
          std::cout << "\n\n| ADD ROW |\n\n"
                    << "[Enter the Coordenates of the row]: ";
          if (invalid_input(added_row)) break;

          if (added_row < 0 || added_row >= rows) {
            std::cout << "\n\nERROR INVALID ROW";
            wait_enter();
            break;
          }
          agregarfila(board, rows, columns, added_row, char_capacity);
          used_tokens = rows*columns;
          break;

        case 5:
          unsigned short added_column;

          std::cout << "\n\n| ADD COLUMN |\n\n"
                    << "[Enter the Coordenates of the column]: ";
          if (invalid_input(added_column)) break;
          
          if (added_column < 0 || added_column >= columns) {
            std::cout << "\n\nERROR INVALID COLUMN";
            wait_enter();
            break;
          }
          agregarcolumna(board, rows, columns, added_column, char_capacity);
          used_tokens = rows*columns;

          break;
      }
    }
  }
  return 0;
}
