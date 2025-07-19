# add_library(CCLASS_TestClass STATIC test_class.c)
# target_include_directories(CCLASS_TestClass PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)

# target_link_libraries(CCLASS_TestClass PUBLIC CClass)


add_executable(robinQueueBufferTest test.c)
target_link_libraries(robinQueueBufferTest PUBLIC 
  RobinQueueBuffer
)


