/**
 * @file class.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-09
 * 
 * @copyright Copyright (c) 2025
 */

#ifndef CLASS_H
#define CLASS_H

#include "project.h"
#include "cimclcpp.h"

/** Declare Instance of Class */
#define Class(__CLASS_NAME) struct __CLASS_NAME

/** Make Headers More Readable */
#define ClassMethodHead(__CLASS_NAME)
#define ClassMethodEnd(__CLASS_NAME)

/** Define Class Methods */
#define ClassMethod(__CLASS_NAME, __TYPE, __NAME, ...) __TYPE __CLASS_NAME##_##__NAME(__VA_OPT__(__VA_ARGS__,) Class(__CLASS_NAME) * this)
#define ClassMethodMember(__CLASS_NAME, __TYPE, __NAME, ...) __TYPE (*__NAME)(__VA_OPT__(__VA_ARGS__,) Class(__CLASS_NAME) * this) /* Function Pointer*/

// #define ClassMethodPointer(__CLASS_NAME, __TYPE, __NAME, ...) __TYPE (*__CLASS_NAME##_##__NAME)(__VA_ARGS__, Class(__CLASS_NAME) * this)
// #define ClassMember(__TYPE, __NAME)

#define ClassHead(__CLASS_NAME) \
  struct __CLASS_NAME {

#define ClassEnd(__CLASS_NAME)  };


// #define ClassConstructorMember(__CLASS_NAME, ...) ClassMethodMember(__CLASS_NAME, void, constructor, __VA_ARGS__)
// #define ClassConstructor(__CLASS_NAME, ...) ClassMethod(__CLASS_NAME, void, CLASS_NAME##_constructor, __VA_ARGS__)
// #define ClassConstructorDefault ClassConstructorMember
// 
// 
// #define ClassDestructorMember(__CLASS_NAME, ...) ClassMethodMember(__CLASS_NAME, void, destructor, __VA_ARGS__)
// #define ClassDestructor(__CLASS_NAME) ClassMethod(__CLASS_NAME, void, CLASS_NAME##_destructor, __VA_ARGS__)
// #define ClassDestructorDefault ClassDestructorMember



#endif // CLASS_H