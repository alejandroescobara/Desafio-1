#pragma once


#include <cstdlib>
#include <cassert>

void clear_console();

void wait_enter();

bool invalid_memory_reservation(unsigned char*);

bool invalid_input(short&);
bool invalid_input(unsigned short&);
bool invalid_input(unsigned char&);
bool invalid_input(size_t&);

void resize_cchain(unsigned char*&, const size_t, const size_t);
