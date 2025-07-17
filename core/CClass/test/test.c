

#include "test_class.h"
#include <string.h>

uint8_t const * const HelloWorld = "HelloWorld";

int main (void)
{
  SomeIoVTableGeneric.write (HelloWorld, strlen(HelloWorld), NULL);

  return 0;
}
