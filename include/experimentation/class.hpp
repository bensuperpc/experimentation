#ifndef BENLIB_CLASS_EXPERIMENTATION_HPP
#define BENLIB_CLASS_EXPERIMENTATION_HPP

#include "experimentation/string_external.hpp"

namespace benlib
{

namespace class
{

  namespace experimentation
  {

  class Experimentation
  {
  public:
    Experimentation()
    {
      // Do nothing
    }

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
  };

  }  // namespace experimentation

}  // namespace class

}  // namespace benlib

#endif  // BENLIB_CLASS_EXPERIMENTATION_HPP
