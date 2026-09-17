#pragma once

#include <cstdlib>
#include <iostream>
#include <random>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define BOLD    "\033[1m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

constexpr short token_size = 3;

constexpr const char *token1 = "₪", *token2 = "▲", *token3 = "☻", *token4 = "☼", *token5 = "♫", *token6 = "♦"; //   INVALID
//bit representation:          000            001            010            011            100            101         111

//                                        token1    token2        token3       token4      token5      token6
constexpr unsigned char token_masks[] = {  0b0,   0b00100000,   0b01000000,  0b01100000, 0b10000000, 0b10100000 };

constexpr unsigned char and_mask = 0b11100000, invalid = 0b00000111;

static std::random_device rd;
static std::mt19937 gen(rd());

inline void print_title() {
  std::cout << "WELCOME TO " << RED BOLD     << 'S' << RESET
                             << GREEN BOLD   << 'W' << RESET
                             << YELLOW BOLD  << 'E' << RESET 
                             << BLUE BOLD    << 'E' << RESET
                             << CYAN BOLD    << 'T' << RESET
                             << ' '
                             << MAGENTA BOLD << 'C' << RESET 
                             << RED BOLD     << 'R' << RESET
                             << GREEN BOLD   << 'U' << RESET
                             << YELLOW BOLD  << 'S' << RESET 
                             << BLUE BOLD    << 'H' << RESET;
}


inline unsigned char get_token(const unsigned char* board, const size_t index) {

  size_t end_bit  = index,
         end_char   = end_bit/8;

  unsigned char end_bit_iterator = end_bit%8,
                end_token = 0;

  end_token = (board[end_char] & (and_mask >> end_bit_iterator)) << end_bit_iterator;

  end_bit_iterator += 3;
  if (end_bit_iterator >= 8) {
    ++end_char;
    end_bit_iterator -= 8;
    if (end_bit_iterator > 0) {
      end_token ^= (board[end_char] & (and_mask << (token_size-end_bit_iterator))) >> (token_size-end_bit_iterator);
    }
  }

  return end_token;
}


void create_board(const size_t, const size_t, unsigned char*);

void print_board(const size_t, const size_t, const size_t, const unsigned char*);

size_t delete_row(unsigned char*, size_t&, size_t, const size_t, const unsigned char);

size_t delete_row(unsigned char* board, size_t&, const size_t, const unsigned char);

size_t upper_replace(unsigned char*, const size_t, size_t);

void cascaded_fall(unsigned char*, const size_t, size_t);

size_t horizontal_scanner(const size_t, size_t&);

size_t vertical_scanner(const size_t, const size_t, size_t&);

void combo_scanner(unsigned char*, const size_t, const size_t, const size_t);
