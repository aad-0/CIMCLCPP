# set minimum cmake version is required
cmake_minimum_required(VERSION 3.18)

# Set prokect name, version and description
project(Test
	VERSION 0.1
  DESCRIPTION "CanIMakeCLikeCpp"
	LANGUAGES C)

# set which compiler will be used
set(CMAKE_C_COMPILER "/usr/bin/gcc")

## set(CMAKE_CXX_FLAGS	
## 	"${CMAKE_CXX_FLAGS} -Og -ggdb")#-Wpedantic")
## 
## set(CMAKE_C_FLAGS	
##   "${CMAKE_C_FLAGS} -Og -ggdb")#-Wpedantic")


set(CMAKE_C_FLAGS	
  "${CMAKE_C_FLAGS} --std=gnu99 -Og -ggdb -Wextra -Wall -Wpedantic ")

# cmake_binary_dir is actually where you set to build ect ./build
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


#add_subdirectory(third_party)
# add_subdirectory(src)
add_subdirectory(core)
#add_subdirectory(app)
