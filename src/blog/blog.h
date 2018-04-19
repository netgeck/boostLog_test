
#ifndef __BOOST_LOG_DEBUG_WRAPPER__H__
#define __BOOST_LOG_DEBUG_WRAPPER__H__

#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/record_ostream.hpp>


namespace blog {

typedef boost::log::trivial::severity_level lvl; //!< сокращение для уровней определённых в boost'е.

/**
 * @brief Инициализация библиотеки логирования
 * @param syslogLvlThreshold - минимальный уровень для вывода логов в syslog
 */
void logInit(blog::lvl syslogLvlThreshold = blog::lvl::warning);

} // namespace blog


// логер по умолчанию
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(logDefault, boost::log::sources::severity_logger<blog::lvl>)
// логер дублирующий вывод в syslog
BOOST_LOG_GLOBAL_LOGGER(logSyslog, boost::log::sources::severity_logger<blog::lvl>);


/// Отключаем вывод логов в зависимости от типа сборки
#ifndef NDEBUG
#define IS_BLOG_DISABLED	false
#else
#define IS_BLOG_DISABLED	true
#endif


/*
Пояснение по работе отключаемых макросов.
Макрос
	#define DBG_OUT \
		if (condition) {} \
		else std::cout
При вызове
	DBG_OUT << "дебажные сообщения";
Развернётся в:
	if (condition) {} else std::cout << "дебажные сообщения";
*/

// Вывод дебажного сообщения
#define BLOG \
	if (IS_BLOG_DISABLED) {} \
	else BOOST_LOG(logDefault::get())

// Вывод дебажного сообщения продублированный в syslog
#define BLOG_SYSLOG \
	if (IS_BLOG_DISABLED) {} \
	else BOOST_LOG(logSyslog::get())

// Вывод дебажного сообщения (с уровнем лога)
#define BLOG_SEV(lvl) \
	if (IS_BLOG_DISABLED) {} \
	else BOOST_LOG_SEV(logDefault::get(), lvl)

// Вывод дебажного сообщения (с уровнем лога) продублированный в syslog
#define BLOG_SYSLOG_SEV(lvl) \
	if (IS_BLOG_DISABLED) {} \
	else BOOST_LOG_SEV(logSyslog::get(), lvl)


#endif /* __BOOST_LOG_DEBUG_WRAPPER__H__ */
