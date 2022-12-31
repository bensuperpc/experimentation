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

namespace classes
{

class Base
{
    public:
        Base() { std::cout << "Empty Base constructor" << std::endl; }

        ~Base() { std::cout << "Base destructor" << std::endl; }

        Base(const Base&) { std::cout << "Base copy constructor" << std::endl; }
        Base(Base&&) { std::cout << "Base move constructor" << std::endl; }

        Base& operator=(const Base&)
        {
            // Do nothing
            return *this;
        }
        Base& operator=(Base&&)
        {
            // Do nothing
            return *this;
        }

        virtual std::string get_string() const { return "experimentation"; }

        virtual void function1(std::string_view str) { std::cout << "From function1: " << str << std::endl; }

        virtual void function2(std::string_view str) { std::cout << "From function2: " << str << std::endl; }

        virtual void function3(std::string_view str) { std::cout << "From function3: " << str << std::endl; }

        void print() const { std::cout << "Base" << std::endl; }
        virtual void print_virtual() const { std::cout << "Base virtual" << std::endl; }
};

class Derived : public Base
{
    public:
        Derived() { std::cout << "Empty Derived constructor" << std::endl; }

        ~Derived() { std::cout << "Derived destructor" << std::endl; }

        Derived(const Derived&) { std::cout << "Derived copy constructor" << std::endl; }
        Derived(Derived&&) { std::cout << "Derived move constructor" << std::endl; }

        Derived& operator=(const Derived&)
        {
            // Do nothing
            return *this;
        }
        Derived& operator=(Derived&&)
        {
            // Do nothing
            return *this;
        }

        std::string get_string() const override { return "experimentation derived"; }

        void function1(std::string_view str) override { std::cout << "From function1 derived: " << str << std::endl; }

        void function2(std::string_view str) override { std::cout << "From function2 derived: " << str << std::endl; }

        void function3(std::string_view str) override { std::cout << "From function3 derived: " << str << std::endl; }

        void print() const { std::cout << "Derived" << std::endl; }

        void print_virtual() const { std::cout << "Derived virtual" << std::endl; }
};

}  // namespace classes

}  // namespace experimentation

}  // namespace object

}  // namespace benlib

#endif  // BENLIB_CLASS_EXPERIMENTATION_HPP
