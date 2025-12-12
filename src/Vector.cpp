#include "Vector.hpp"
#include <iostream>

void Vector::push_back(int val) {
    std::lock_guard<std::mutex> lock(mtx);
    vec.push_back(val);
}

std::vector<int> Vector::get() const {
    return vec;
}

void Vector::read_vec() const {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "\nVector elements:\n";
    for (const auto& i : vec) {
        std::cout << i << ", ";
    }
    std::cout << "\n";
}
int Vector::pop_back() {
    std::lock_guard<std::mutex> lock(mtx);
    if (vec.empty()) {
        throw VectorPopBackException();
    }
    int val = vec.back();
    vec.pop_back();

    return val;
};
int Vector::at(size_t index) {
    if (index > vec.size()) {
        throw InvalidIndexException(index, vec.size());
    }
    return vec[index];
}
