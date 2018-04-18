
#ifndef __MY_BOOST_LOG_DEBUG__H__
#define __MY_BOOST_LOG_DEBUG__H__

#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>



BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(logDefault, boost::log::sources::logger_mt)

BOOST_LOG_GLOBAL_LOGGER(logSyslog, boost::log::sources::logger_mt);


#ifndef NDEBUG
#define IS_BLOG_DISABLED	false
#else
#define IS_BLOG_DISABLED	true
#endif

/*
Макрос
	#define DBG_OUT \
		if (condition) {} \
		else std::cout
При вызове
	DBG_OUT << "дебажные сообщения";
Развернётся в:
	if (condition) {} else std::cout << "дебажные сообщения";
*/

#define BLOG \
	if (IS_BLOG_DISABLED) {} \
	else BOOST_LOG(logDefault::get())

#define BLOG_SYSLOG \
	if (IS_BLOG_DISABLED) {} \
	else BOOST_LOG(logSyslog::get())

void logInit();


#endif /* __MY_BOOST_LOG_DEBUG__H__ */