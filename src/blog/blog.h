/*
Обоснование написания обёртки над Boost Log:

* В сравнении с BoostLog, обёртка по использованию приближена к макросам dmsg, что облегчает переход. 
  Отличия от "debug-core": 1) необходимость линковки с библиотекой обёртки 
                           2) необходимость вызова функции инициализации
                           3) вывод сообщений в стиле плюсовых потоков.
* Для вывод сообщений используются макросы
* Вывод сообщений в плюсовом стиле!
* Сообщения предназначенные для syslog'а дублируются и в основной лог.
* Логи выводятся с timestamp'ами и уровнями важности сообщений.
* Сообщения можно маркировать разными каналами.
* Облегчение использования Boost Log через обёртку:
	1) Обёртка скрывает сложность сборки с Boost Log'ом	
	2) При этом реализован функционал dmsg и плюс некоторые фичи Boost Log'а.
*/

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
 * @param consoleLvlThreshold - минимальный уровень для вывода логов в консоль
 */
void logInit(blog::lvl syslogLvlThreshold = blog::lvl::warning, blog::lvl consoleLvlThreshold = blog::lvl::info);


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
#define BLOG(level) \
	if (BLOG_IS_CONSOLE_OUTPUT_DISABLED) {} \
	else BOOST_LOG_SEV(blog::logConsole::get(), blog::lvl::level)

// Вывод дебажного сообщения (с уровнем лога) продублированный в syslog
#define BLOG_SYSLOG(level)	BOOST_LOG_SEV(blog::logSyslog::get(), blog::lvl::level)

// Вывод дебажного сообщения (с уровнем лога и каналом) 
#define BLOG_CHAN(channel, level) \
	if (BLOG_IS_CONSOLE_OUTPUT_DISABLED) {} \
	else BOOST_LOG_CHANNEL_SEV(blog::logConsole::get(), channel, blog::lvl::level)



#endif /* __BOOST_LOG_DEBUG_WRAPPER__H__ */
