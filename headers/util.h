#include <time.h>

#pragma once

#ifdef __cplusplus 
extern "C" {
#endif

unsigned int _rotr(unsigned int data, unsigned int bits);
unsigned int _rotl(unsigned int data, unsigned int bits);
timespec diff(timespec start, timespec end);

#ifdef __cplusplus 
}
#endif
