
set(target_name testLog)

add_executable(${target_name} main.cxx dbg.cxx)

# https://www.boost.org/doc/libs/1_65_0/libs/log/doc/html/log/installation/config.html
add_definitions(-DBOOST_LOG_DYN_LINK)
add_definitions(-DBOOST_LOG_USE_NATIVE_SYSLOG)

#список библиотек для линковки
target_link_libraries(${target_name}
	pthread
	boost_system
	boost_chrono
	boost_thread-mt
	boost_log
	boost_log_setup
)

install(TARGETS ${target_name} RUNTIME DESTINATION CMAKE_BINARY_DIR)