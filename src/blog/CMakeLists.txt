
if (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	# Задаём директивы используемые в публичных хедерах библиотеки
	set(DEFINITIONS_USING_IN_PUBLIC_HEADERS -DBLOG_BUILD_SYSLOG_ONLY) # В Релизе только вывод в syslog
endif()


set(target_name blog)

add_library(${target_name} SHARED blog.cxx)

target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Подробней о сборочных директивах BOOST LOG: 
# https://www.boost.org/doc/libs/1_65_0/libs/log/doc/html/log/installation/config.html
target_compile_definitions(${target_name}
PUBLIC
# Про BOOST_LOG_DYN_LINK: https://www.boost.org/doc/libs/1_65_0/libs/log/doc/html/log/rationale/namespace_mangling.html
	-DBOOST_LOG_DYN_LINK
	-DBOOST_LOG_USE_NATIVE_SYSLOG
	${DEFINITIONS_USING_IN_PUBLIC_HEADERS}
)

#список библиотек для линковки
target_link_libraries(${target_name}
PUBLIC
	boost_log
	boost_log_setup
)

install(TARGETS ${target_name} LIBRARY DESTINATION ${SHARED_LIB_DIR})
