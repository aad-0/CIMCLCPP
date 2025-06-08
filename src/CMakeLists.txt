add_executable(example_class example_class.c)

target_include_directories(example_class PRIVATE
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)
