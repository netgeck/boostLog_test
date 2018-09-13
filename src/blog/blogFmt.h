//
// Created by kostya on 30.07.18.
//


#ifndef __BOOST_LOG_DEBUG_WRAPPER_FORMAT__H__
#define __BOOST_LOG_DEBUG_WRAPPER_FORMAT__H__


#include <boost/format.hpp>
#include <blog.h>


// Вывод дебажного сообщения
#define BLOG_FMT_(formatString) \
	if (BLOG_IS_CONSOLE_OUTPUT_DISABLED) {} \
	else BOOST_LOG(blog::logConsole::get()) << boost::format(formatString)

// Вывод дебажного сообщения продублированный в syslog
#define BLOG_SYSLOG_FMT_(formatString) \
	BOOST_LOG(blog::logSyslog::get()) << boost::format(formatString)

// Вывод дебажного сообщения (с уровнем лога)
#define BLOG_FMT(level, formatString) \
	if (BLOG_IS_CONSOLE_OUTPUT_DISABLED) {} \
	else BOOST_LOG_SEV(blog::logConsole::get(), blog::lvl::level) << boost::format(formatString)

// Вывод дебажного сообщения (с уровнем лога) продублированный в syslog
#define BLOG_SYSLOG_FMT(level, formatString) \
	BOOST_LOG_SEV(blog::logSyslog::get(), blog::lvl::level) << boost::format(formatString)

// Вывод дебажного сообщения (с уровнем лога и каналом)
#define BLOG_CHAN_FMT(channel, level, formatString) \
	if (BLOG_IS_CONSOLE_OUTPUT_DISABLED) {} \
	else BOOST_LOG_CHANNEL_SEV(blog::logConsole::get(), channel, blog::lvl::level) << boost::format(formatString)


#endif //__BOOST_LOG_DEBUG_WRAPPER_FORMAT__H__
