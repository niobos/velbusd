#ifndef __UTILS_OUTPUT_HPP__
#define __UTILS_OUTPUT_HPP__

#include <string>

std::string hex(int number);
std::string hex(char num);

std::string bin(int bitmap);

std::string format_fixed_point(float f, int decimals=2);

int bitnum(int bitmap);

#endif // __UTILS_OUTPUT_HPP__
