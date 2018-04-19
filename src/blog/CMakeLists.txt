
set(target_name blog)

add_library(${target_name} blog.cxx)

target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Подробней о сборочных директивах BOOST LOG: 
# https://www.boost.org/doc/libs/1_65_0/libs/log/doc/html/log/installation/config.html
target_compile_definitions(${target_name}
PUBLIC
# Про BOOST_LOG_DYN_LINK: https://www.boost.org/doc/libs/1_65_0/libs/log/doc/html/log/rationale/namespace_mangling.html
	-DBOOST_LOG_DYN_LINK
	-DBOOST_LOG_USE_NATIVE_SYSLOG
)

#список библиотек для линковки
target_link_libraries(${target_name}
PUBLIC
	pthread
	boost_system
	boost_chrono
	boost_thread-mt
	boost_log
	boost_log_setup
)
