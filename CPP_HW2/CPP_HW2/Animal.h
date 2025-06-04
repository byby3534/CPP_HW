#pragma once
#include <iostream>
using namespace std;

class Animal
{
public:
    virtual void makeSound() = 0;
    virtual ~Animal() {};
};

class Dog : public Animal
{
public:
    virtual void makeSound() { cout << "Woof Woof" << endl; }
};

class Cat : public Animal
{
public:
    virtual void makeSound() { cout << "Meow Meow" << endl; }
};

class Cow : public Animal
{
public:
    virtual void makeSound() { cout << "Moo Moo" << endl; }
};
