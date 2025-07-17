add_library(IO INTERFACE)

target_include_directories(IO INTERFACE 
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(IO INTERFACE
  CanIMakeCLookLikeCpp
)

# add_subdirectory(test)


