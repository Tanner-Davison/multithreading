#pragma once
#include <mutex>
#include <source_location>
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
    size_t               index;
    size_t               size;
    std::source_location location;

    InvalidIndexException(size_t               idx,
                          size_t               sz,
                          std::source_location loc = std::source_location::current())
        : std::out_of_range("[ERROR][::at()] Index Out of Range!")
        , index(idx)
        , size(sz)
        , location(loc) {};
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
