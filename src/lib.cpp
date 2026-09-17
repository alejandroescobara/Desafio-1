#include "lib.hpp"
#include "utilities.hpp"
#include "menu.hpp"

#include <iostream>
#include <random>


#include <cstdlib>

//#include <bitset>
//std::cout << std::bitset<8>(token) << std::endl;

constexpr short token_size = 3;

constexpr const char *token1 = "₪", *token2 = "▲", *token3 = "☻", *token4 = "☼", *token5 = "♫", *token6 = "♦"; //   INVALID
//bit representation:          000            001            010            011            100            101         111

//                                        token1    token2        token3       token4      token5      token6
constexpr unsigned char token_masks[] = {  0b0,   0b00100000,   0b01000000,  0b01100000, 0b10000000, 0b10100000 };

constexpr unsigned char and_mask = 0b11100000, invalid = 0b00000111;

static std::random_device rd;
static std::mt19937 gen(rd());

void create_board(const size_t char_capacity, const size_t used_tokens, unsigned char* board) {
  
  std::uniform_int_distribution<size_t> dist(0, 5);

  //se llenan todos los bytes de tokens invalidos
  for (size_t index = 0; index < char_capacity; ++index) {
    board[index] = 0xFF;
  }

  //inicializacion de variables de control necesarias
  unsigned char bit_iterator = 0, xor_mask;
  size_t char_index = 0, token_iterator = 0;

  while (token_iterator < used_tokens) {
    
    //nos excedimos mas alla del byte actual?
    if (bit_iterator >= 8) {
      ++char_index; //pasamos al siguiente byte
      bit_iterator -= 8; //queda solo el desfase
                         
      if (bit_iterator > 0) { //si el token quedo incompleto, se termina de formar en el otro byte
        board[char_index] &= ~(and_mask << (token_size-bit_iterator)); 
        board[char_index] ^= (xor_mask << (token_size-bit_iterator));
        ++token_iterator;
      }
    } 
    //todavia faltan tokens?
    if (token_iterator < used_tokens) {
      board[char_index] &= ~(and_mask >> bit_iterator);
      xor_mask = token_masks[dist(gen)];
      board[char_index] ^= (xor_mask >> bit_iterator);

      bit_iterator += 3; //se agregan los bits sensados
      if (bit_iterator <= 8) { //si el token se escribio completo, se cuenta
        ++token_iterator;
      }
    }
  }
}

void print_token(unsigned char token) {
  switch(token) {
    case 0b0:
      std::cout << RED BOLD << ' ' << token1 << ' ' << RESET;
      break;
    case 0b1:
      std::cout << GREEN BOLD << ' ' << token2 << ' ' << RESET;
      break;
    case 0b10:
      std::cout << YELLOW BOLD << ' ' << token3 << ' ' << RESET;
      break;
    case 0b11:
      std::cout << BLUE BOLD << ' ' << token4 << ' ' << RESET;
      break;
    case 0b100:
      std::cout << MAGENTA BOLD << ' ' << token5 << ' ' << RESET;
      break;
    case 0b101:
      std::cout << CYAN BOLD << ' ' << token6 << ' ' << RESET;
      break;
    default:
      std::cout << " E ";
      break;
  }
}

void print_board(const size_t cols, const size_t char_capacity, const size_t used_tokens, const unsigned char* board) {

  unsigned char bit_iterator = 0, token = 0;
  size_t char_index = 0, token_iterator = 0;
  std::cout << "\n";
  while (token_iterator < used_tokens) {
    if (bit_iterator >= 8) {
      ++char_index;
      bit_iterator -= 8;
      if (bit_iterator > 0) {
        token ^= (board[char_index] & (and_mask << (token_size-bit_iterator))) >> (8-bit_iterator);
        print_token(token);
        token = 0;
        ++token_iterator;
        if (token_iterator % cols == 0) std::cout << "\n\n";
      }
    }
    if (token_iterator < used_tokens) {
      token = board[char_index] & (and_mask >> bit_iterator);
      token = ((5-bit_iterator < 0)? token << (bit_iterator-5) : token >> (5-bit_iterator));
      if (bit_iterator <= 5) {
        print_token(token);
        token = 0;
        ++token_iterator;
        if (token_iterator % cols == 0) std::cout << "\n\n";
      }
    }
    bit_iterator += 3;
  }
}

size_t delete_row(unsigned char* board, size_t& rows, const size_t cols, const unsigned char selected_row) {
  
  const size_t token_limit = (rows*cols) - (selected_row*cols);
  size_t token_counter = 0;
  
  --rows;
  
  size_t  start_bit  = selected_row*cols*token_size,
          start_char = start_bit/8,
          end_bit    = (selected_row+1)*cols*token_size, 
          end_char   = end_bit/8;

  unsigned char start_bit_iterator = start_bit%8,
                end_bit_iterator   = end_bit%8,

                end_token = 0;

  while (token_counter < token_limit) {

    end_token = (board[end_char] & (and_mask >> end_bit_iterator)) << end_bit_iterator;
    
    end_bit_iterator += 3;
    if (end_bit_iterator >= 8) {
      ++end_char;
      end_bit_iterator -= 8;
      if (end_bit_iterator > 0) {
        end_token ^= (board[end_char] & (and_mask << (token_size-end_bit_iterator))) >> (token_size-end_bit_iterator);
      }
    } 

    board[start_char] &= ~(and_mask >> start_bit_iterator);
    board[start_char] ^= (end_token >> start_bit_iterator);

    start_bit_iterator += 3;

    if (start_bit_iterator >= 8) {
      ++start_char;
      start_bit_iterator -= 8;
      if (start_bit_iterator > 0) {
        board[start_char] &= ~(and_mask << (token_size-start_bit_iterator));
        board[start_char] ^= end_token << (token_size-start_bit_iterator);
        ++token_counter;
      }
      else ++token_counter;
    }
    else ++token_counter;
  }

  return rows*cols;
}

//size_t delete_column(unsigned char* board, size_t& cols, const size_t rows, size_t used_to, const unsigned char selected_col) {}

size_t upper_replace(unsigned char* board, const size_t cols, size_t initial_bit) {
  size_t initial_byte = initial_bit/8,
         upper_bit    = initial_bit - (cols*token_size),
         next_bit     = upper_bit/8,
         upper_byte   = upper_bit/8;

  initial_bit %= 8;
  upper_bit %= 8;

  unsigned char upper_token = (board[upper_byte] & (and_mask >> upper_bit)) << upper_bit;
  if (upper_bit > 5) {
    ++upper_byte;
    upper_bit -= 5;
    upper_token ^= (board[upper_byte] & (and_mask << (token_size-upper_bit) )) >> (token_size-upper_bit);
  }

  board[initial_byte] &= ~(and_mask >> initial_bit);
  board[initial_byte] ^= upper_token >> initial_bit;

  if (initial_bit > 5) {
    ++initial_byte;
    initial_bit -= 5;
    board[initial_byte] &= ~(and_mask << (token_size-initial_bit));
    board[initial_byte] ^= upper_token << (token_size-initial_bit);
  }
  return next_bit;
}

void cascaded_fall(unsigned char* board, const size_t cols, size_t bit_index) {

  std::uniform_int_distribution<size_t> dist(0, 5);

  size_t first_row_bits = (cols*token_size);
  
  while (bit_index > first_row_bits) {
    bit_index = upper_replace(board, cols, bit_index);
  }

  size_t last_byte = bit_index/8;
  bit_index %= 8;

  unsigned char last_token = token_masks[dist(gen)];

  board[last_byte] &= ~(and_mask >> bit_index);
  board[last_byte] ^= ( last_token >> bit_index);

  if (bit_index > 5) {
    ++last_byte;
    bit_index -= 5;
    board[last_byte] &= ~(and_mask << (token_size-bit_index));
    board[last_byte] ^= last_token << (token_size-bit_index);
  } 
}

bool combo_scanner(unsigned char* board, const size_t cols, size_t bit_index) {
  
}

void create_valid_board() {

}
/*
//a la creacion inicial del tablero, se tiene que crear y despues modificar hasta que ya no quede ningun combo de fichas, esa primera funcion de sensado debe de estar
//incorporada en una funcion grande que contenga combo_scanner y create_board hasta que quede uno valido

bool combo_scanner() {
  //solo para las fichas que tienen posibilidades de haberse alterado de forma en que generen un combo 
}
*/
