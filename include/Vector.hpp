#pragma once
#include <mutex>
#include <stdexcept>
#include <vector>

class VectorPopBackException : public std::runtime_error {
  public:
    VectorPopBackException()
        : std::runtime_error(
              "\nVectorPopBackException_Error_ Cannot attempted to pop empty "
              "vector.") {};
};
class InvalidIndexException : public std::out_of_range {
  public:
    size_t index;
    size_t size;

    InvalidIndexException(size_t idx, size_t sz)
        : std::out_of_range("\n::at() ERROR Index Out of Range!")
        , index(idx)
        , size(sz) {};
};
class Vector {
    mutable std::mutex       mtx;
    mutable std::vector<int> vec;

  public:
    void             push_back(int val);
    int              pop_back();
    void             read_vec() const;
    int              at(size_t index);
    std::vector<int> get() const;
};
