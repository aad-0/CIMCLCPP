/**
 * @file example_class.c
 * @author your name (you@domain.com)
 * @brief An example of using the "class" macros.
 * @version 0.1
 * @date 2025-06-09
 * 
 * @copyright Copyright (c) 2025
 */

#include "example_class.h"
#include <stdio.h>

ClassMethod(ExampleClass, void, addA, int x)
{
  this->a += x;
}

ClassMethod(ExampleClass, void, subB, int x)
{
  this->b -= x;
}

ClassMethod(ExampleClass, void, destructor)
{
  CIMCLCPP_Unused(this);
  fprintf(stdout, "ExampleClass destructor called.\n");
}

ClassMethod(ExampleClass, void, constructor, int A, int B)
{
  this->a = A;
  this->b = B;

  this->constructor = ExampleClass_constructor;
  this->destructor = ExampleClass_destructor;
  this->addA = ExampleClass_addA;
  this->subB = ExampleClass_subB;

  fprintf(stdout, "ExampleClass constructor called.\n");
}


/**
 * @brief Main entry point to demonstrate usage of ExampleClass.
 * 
 * @return int Exit code.
 */
int main(void)
{
    // The `Class` macro from `class.h` is used to refer to the struct.
    // `Class(ExampleClass)` expands to `struct ExampleClass`.
    Class(ExampleClass) my_object;

    // Initialize the object using the constructor.
    ExampleClass_constructor(10, 20, &my_object);

    printf("Using ExampleClass:\n");
    printf("  my_object.a = %d\n", my_object.a);
    printf("  my_object.b = %d\n", my_object.b);

    // Call methods via function pointers.
    my_object.addA(10, &my_object);
    my_object.subB(5, &my_object);

    printf("After method calls:\n");
    printf("  my_object.a = %d\n", my_object.a);
    printf("  my_object.b = %d\n", my_object.b);

    // Clean up the object using the destructor.
    my_object.destructor(&my_object);

    return 0;
}
