/**
 * @file example_class.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-09
 * 
 * @copyright Copyright (c) 2025
 */

#ifndef EXAMPLE_CLASS_H
#define EXAMPLE_CLASS_H

#include "class.h"

// Forward-declaration of the class struct is needed before it's used in function prototypes.
Class(ExampleClass);

ClassHead(ExampleClass)

  // Class method Pointers
  ClassMethodHead(ExampleClass)

  ClassMethodMember(ExampleClass, void, constructor, int A, int B);
  ClassMethodMember(ExampleClass, void, destructor);

  ClassMethodMember(ExampleClass, void, addA, int x);
  ClassMethodMember(ExampleClass, void, subB, int x);

  ClassMethodEnd(ExampleClass)

  int a;
  int b;
ClassEnd(ExampleClass)

#endif // EXAMPLE_CLASS_H

