add_library(RobinQueueBuffer STATIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/robin_queue_buffer.c

)
target_include_directories(RobinQueueBuffer PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(RobinQueueBuffer PUBLIC
  IO
  CClass 
  QueueBuffer
  CanIMakeCLookLikeCpp
)

add_subdirectory(test)


