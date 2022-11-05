#ifndef BENLIB_CLASS_EXPERIMENTATION_HPP
#define BENLIB_CLASS_EXPERIMENTATION_HPP

#include <iostream>
#include <string>

#include "experimentation/string_external.hpp"

namespace benlib
{

namespace object
{

namespace experimentation
{

class Experimentation
{
public:
  Experimentation() { std::cout << "Empty Experimentation constructor" << std::endl; }

  ~Experimentation()
  {
    // Do nothing
  }

  Experimentation(const Experimentation&)
  {
    // Do nothing
  }
  Experimentation(Experimentation&&)
  {
    // Do nothing
  }

  Experimentation& operator=(const Experimentation&)
  {
    // Do nothing
    return *this;
  }
  Experimentation& operator=(Experimentation&&)
  {
    // Do nothing
    return *this;
  }
  std::string get_string() const { return "experimentation"; }

  void function1(std::string_view str) { std::cout << "function1: " << str << std::endl; }

  void function2(std::string_view str) { std::cout << "function2: " << str << std::endl; }

  void function3(std::string_view str) { std::cout << "function3: " << str << std::endl; }
};

}  // namespace experimentation

}  // namespace object

}  // namespace benlib

#endif  // BENLIB_CLASS_EXPERIMENTATION_HPP
