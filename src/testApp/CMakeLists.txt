set(target_name testBoostLog)

add_executable(${target_name} main.cxx)

target_link_libraries(${target_name}
	blog
)
