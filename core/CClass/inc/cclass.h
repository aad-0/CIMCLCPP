/**
 *
 */

#ifndef CLASS_H
#define CLASS_H

#include "project.h"
#include "cimclcpp.h"


/** CLASS **/

///
#define CLASS_PTR void *
#define VTABLE_PTR void *

///
#define PARENT_PTR  CLASS_PTR

///
#define __VTABLE_DONTUSE_FUNC_PTR(__FUNC_NAME, __RETURN_TYPE, ...) \
  __RETURN_TYPE ( * __FUNC_NAME) (CLASS_PTR pSelf __VA_OPT__(, __VA_ARGS__))

///
#define __CLASS_DONTUSE_FUNC_PTR(__FUNC_NAME, __RETURN_TYPE, ...) \
  __RETURN_TYPE ( * __FUNC_NAME) ( CLASS_PTR pSelf __VA_OPT__(, __VA_ARGS__))

///
#define __VTABLE_DONTUSE_FUNC_DECLR(__VTABLE_NAME, __FUNC_NAME, __RETURN_TYPE, ...) \
  __RETURN_TYPE  __VTABLE_NAME##_##__FUNC_NAME (CLASS_PTR pSelf __VA_OPT__(, __VA_ARGS__))

///
#define __CLASS_DONTUSE_FUNC_DECLR(__CLASS_NAME, __FUNC_NAME, __RETURN_TYPE, ...) \
  __RETURN_TYPE  __CLASS_NAME##_##__FUNC_NAME (CLASS_PTR pSelf __VA_OPT__(, __VA_ARGS__))


///
#define CLASS_ASSIGN_VTABLE(__CLASS_NAME, __DEST_FUNC_PTR, __SRC_PTR)  \
  __DEST_FUNC_PTR = __CLASS_NAME##_##__SRC_PTR

#define VTABLE_ASSIGN_VTABLE(__VTABLE_NAME, __DEST_FUNC_PTR, __SRC_PTR)  \
  __DEST_FUNC_PTR = __VTABLE_NAME##_##__SRC_PTR

// 
// #define CLASS_ASSIGN_VTABLE(__CLASS_NAME, __DEST_FUNC_PTR, __SRC_PTR)  \
//   __DEST_FUNC_PTR = __CLASS_NAME##_##__SRC_PTR

//////////


#define VTABLE(__VTABLE_NAME) \
  typedef struct __VTABLE_NAME##_s


////////////

///
#define CLASS(__CLASS_NAME) \
  typedef struct __CLASS_NAME##_s


#define CLASS_PROTO(__CLASS_NAME) \
  typedef struct __CLASS_NAME##_s

#define VTABLE_PROTO(__VTABLE_NAME) CLASS_PROTO(__VTABLE_NAME)


///
#define CLASS_METHOD_PROTO  __CLASS_DONTUSE_FUNC_PTR
#define VTABLE_METHOD_PROTO  __VTABLE_DONTUSE_FUNC_PTR

///
#define CLASS_METHOD_DECLR  __CLASS_DONTUSE_FUNC_DECLR
#define VTABLE_METHOD_DECLR  __VTABLE_DONTUSE_FUNC_DECLR

///
#define CLASS_MEMBER(__TYPE, __MEMBER_NAME)  \
  __TYPE __MEMBER_NAME


#endif // CLASS_H
