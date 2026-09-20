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

constexpr char token_size = 3;

constexpr char option_limit = 6;

constexpr const char *token1 = "₪", *token2 = "▲", *token3 = "☻", *token4 = "☼", *token5 = "♫", *token6 = "♦"; //   INVALID
//bit representation:          000            001            010            011            100            101         111

//                                        token1    token2        token3       token4      token5      token6
constexpr unsigned char token_masks[] = {  0b0,   0b00100000,   0b01000000,  0b01100000, 0b10000000, 0b10100000 };

constexpr unsigned char and_mask = 0b11100000, invalid = 0b00000111;

static std::random_device rd;
static std::mt19937 gen(rd());

inline void print_title() {
  std::cout << "  WELCOME TO\n|" << RED BOLD     << 'S' << RESET
                                 << GREEN BOLD   << 'W' << RESET
                                 << YELLOW BOLD  << 'E' << RESET 
                                 << BLUE BOLD    << 'E' << RESET
                                 << CYAN BOLD    << 'T' << RESET
                                 << ' '
                                 << MAGENTA BOLD << 'C' << RESET 
                                 << RED BOLD     << 'R' << RESET
                                 << GREEN BOLD   << 'U' << RESET
                                 << YELLOW BOLD  << 'S' << RESET 
                                 << BLUE BOLD    << 'H' << RESET
                                 << "|\n\n\n";
}

inline size_t global_combo_counter = 0,
              del_counter          = 0,
              score                = 0,
              cascaded_counter     = 0;


inline void print_options() {
  std::cout << "| 0. CLEAR SCORE | 1. DELETE TOKEN | 2. DELETE ROW | 3. DELETE COLUMN | 4. ADD ROW | 5. ADD COLUMN | 6. EXIT |\n\n";
}

inline void print_history() {
  std::cout <<"  SCORE [" << score << "] MANUAL DELETIONS [" << del_counter << "] TOTAL COMBOS [" << global_combo_counter << "] CASCADES [" << cascaded_counter << "]\n\n";
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

void print_token(unsigned char token);

void print_bit_token(unsigned char token);

void print_board(const size_t, const size_t, const unsigned char*);

void print_bit_board(const size_t, const size_t, const unsigned char*);


size_t delete_row(unsigned char*& board, size_t&, const size_t, const unsigned char, size_t& );


size_t calcularbytesnecesarios(size_t, size_t);

unsigned char ver_ficha(unsigned char*, size_t, size_t, size_t);

unsigned char ver_ficha(unsigned char*, size_t);


void colocarfichaindividual(unsigned char*, size_t, size_t, size_t, unsigned short);

void eliminarcolumna(unsigned char*&, size_t, size_t&, size_t, size_t&);

void agregarfila(unsigned char*&, size_t &, size_t, unsigned short, size_t&);

void agregarcolumna(unsigned char*&, size_t, size_t&, size_t, size_t&);

//funciones para eliminar ficha

size_t upper_replace(unsigned char*, const size_t, size_t);

void cascaded_fall(unsigned char*, const size_t, size_t);

// funciones para escaneo de combos

bool horizontal_scanner(const unsigned char*, const size_t, const size_t, int&, size_t&, int&, size_t&, const unsigned char, bool);

bool vertical_scanner(const unsigned char*, const size_t, const size_t, int&, size_t&, int&, size_t&, const unsigned char, bool);

bool combo_scanner(unsigned char*, const size_t, const size_t, const size_t);

