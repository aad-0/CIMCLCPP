add_library(QueueBuffer STATIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/queue_buffer.c

)
target_include_directories(QueueBuffer PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(QueueBuffer PUBLIC
  IO
  CClass 
  CanIMakeCLookLikeCpp
)

add_subdirectory(test)


