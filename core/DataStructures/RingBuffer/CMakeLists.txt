add_library(RingBuffer STATIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ring_buffer.c

)
target_include_directories(RingBuffer PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(RingBuffer PUBLIC
  IO
  CClass 
  CanIMakeCLookLikeCpp
)

add_subdirectory(test)


