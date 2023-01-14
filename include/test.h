#ifndef TEST_HPP
#define TEST_HPP
#include <iostream>
#include <concepts>
#include <string>

template<typename T, typename U>
requires std::equality_comparable_with<T,U>
static void equals(T const& expected, U const& value) {
    if(not (expected == value)) throw std::runtime_error(std::to_string(expected) + " does not equal " + std::to_string(value));
 }

#endif
