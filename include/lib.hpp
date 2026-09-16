#pragma once

#include <cstdlib>

void create_board(const size_t, const size_t, unsigned char*);

void print_board(const size_t, const size_t, const size_t, const unsigned char*);

size_t delete_row(unsigned char*, size_t&, size_t, const size_t, const unsigned char);

