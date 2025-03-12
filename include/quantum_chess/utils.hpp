#pragma once
#include <iostream>
#include <vector>

// Utility function to check if an element exists in a vector
template <typename T>
bool contains(const std::vector<T>& vec, const T& element) {
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}