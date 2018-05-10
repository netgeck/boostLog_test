/* При использовании этой библиотеки размер объектника в дебажной сборке существенно возрастает.
 * На релизную сборку это не влияет!
 */

#ifndef __BOOST_LOG_DEBUG_WRAPPER__H__
#define __BOOST_LOG_DEBUG_WRAPPER__H__

#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/record_ostream.hpp>


namespace blog {

typedef boost::log::trivial::severity_level lvl; //!< сокращение для уровней определённых в boost'е.

/**
 * @brief Инициализация библиотеки логирования
 * Функция создаёт sink'и для вывода логов. Без неё используется Boost-Log'овский sink по умолчанию: не будет syslog'а!
 * @param syslogLvlThreshold - минимальный уровень для вывода логов в syslog
 */
void logInit(blog::lvl syslogLvlThreshold = blog::lvl::warning);


/// Логер по умолчанию
BOOST_LOG_INLINE_GLOBAL_LOGGER_CTOR_ARGS(
	logConsole,
	boost::log::sources::severity_channel_logger<blog::lvl>,
	(boost::log::keywords::severity = blog::lvl::trace)(boost::log::keywords::channel = ""))
/// Логер дублирующий вывод в syslog
BOOST_LOG_GLOBAL_LOGGER(logSyslog, boost::log::sources::severity_logger<blog::lvl>);

} // namespace blog


/// Отключаем вывод логов в зависимости от типа сборки
#ifdef BLOG_BUILD_SYSLOG_ONLY
#define BLOG_IS_CONSOLE_OUTPUT_DISABLED true
#else
#define BLOG_IS_CONSOLE_OUTPUT_DISABLED false
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
#define BLOG_ \
	if (BLOG_IS_CONSOLE_OUTPUT_DISABLED) {} \
	else BOOST_LOG(blog::logConsole::get())

// Вывод дебажного сообщения продублированный в syslog
#define BLOG_SYSLOG_		BOOST_LOG(blog::logSyslog::get())

// Вывод дебажного сообщения (с уровнем лога)
#define BLOG(lvl) \
	if (BLOG_IS_CONSOLE_OUTPUT_DISABLED) {} \
	else BOOST_LOG_SEV(blog::logConsole::get(), lvl)

// Вывод дебажного сообщения (с уровнем лога) продублированный в syslog
#define BLOG_SYSLOG(lvl)	BOOST_LOG_SEV(blog::logSyslog::get(), lvl)

// Вывод дебажного сообщения (с уровнем лога и каналом) 
#define BLOG_CHAN(channel, lvl) \
	if (BLOG_IS_CONSOLE_OUTPUT_DISABLED) {} \
	else BOOST_LOG_CHANNEL_SEV(blog::logConsole::get(), channel, lvl)



#endif /* __BOOST_LOG_DEBUG_WRAPPER__H__ */
