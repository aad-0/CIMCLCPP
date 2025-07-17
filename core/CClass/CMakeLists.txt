add_library(CClass STATIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/cclass.c

)
target_include_directories(CClass PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(CClass PUBLIC
  CanIMakeCLookLikeCpp

)

add_subdirectory(test)


