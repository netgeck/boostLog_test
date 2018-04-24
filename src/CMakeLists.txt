
add_subdirectory(blog)


set(target_name testLog)

add_executable(${target_name} main.cxx)

target_link_libraries(${target_name}
	blog
)

install(TARGETS ${target_name} RUNTIME DESTINATION ${CMAKE_BINARY_DIR}) # в корень сборочной директории