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



void create_board(const size_t char_capacity, const size_t used_tokens, unsigned char* board) {
  
  //generacion de motor random
  static std::random_device rd;
  static std::mt19937 gen(rd());
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
