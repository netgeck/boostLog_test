
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/keywords/facility.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/support/date_time.hpp>

#include "blog.h"


#define SEVERITY_WIDTH	7


namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
typedef sinks::synchronous_sink<sinks::syslog_backend> syslog_sink;


BOOST_LOG_ATTRIBUTE_KEYWORD(severity_attr, "Severity", blog::lvl)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel_attr, "Channel", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)


BOOST_LOG_GLOBAL_LOGGER_INIT(blog::logSyslog, boost::log::sources::severity_logger<blog::lvl>) {
	boost::log::sources::severity_logger<blog::lvl> logger;
	logger.add_attribute("Tag", attrs::constant< std::string >("syslog"));
	return logger;
}

/* Уровни логирования:
Уровни в Syslog'е:	(подробней тут https://en.wikipedia.org/wiki/Syslog#Severity_level/)
	emergency	0   System is unusable.
	alert		1   Action must be taken immediately. (such as a corrupted system database)
	critical 	2   Critical conditions, such as hard device errors.[8]
	error		3   Error conditions.
	warning 	4   Warning conditions.
	notice		5   Normal but significant conditions.
	info		6   Informational messages.
	debug		7   ...information normally of use only when debugging a program.
Наши уровни:
	trace		0
	debug		1
	info		2
	warning		3
	error		4
	fatal		5
*/


/**
 * @brief Создание sink'а для консольных логов
 * @param consoleLvlThreshold - минимальный уровень выводимых логов
 * @return
 */
boost::shared_ptr<text_sink> initConsoleSink(blog::lvl consoleLvlThreshold) {
	// Создаём sink для вывода в консоль
	boost::shared_ptr<text_sink> sinkConsole;
	
	sinkConsole = boost::make_shared<text_sink>();
	// We have to provide an empty deleter to avoid destroying the global stream object
	boost::shared_ptr<std::ostream> stream(&std::clog, boost::null_deleter());
	// Add a stream to write log to
	sinkConsole->locked_backend()->add_stream(stream);
	// Задаём формат вывода
	logging::formatter fmt = expr::stream
		<< "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
		<< "] ["
		<< expr::max_size_decor<char>(SEVERITY_WIDTH)
		[
			expr::stream << std::left << std::setw(SEVERITY_WIDTH)  << severity_attr
		]
		<< "] ["
		<< expr::if_(expr::has_attr(channel_attr))
		[
			expr::stream << channel_attr
		]
		<< "]: "
		<< expr::smessage;
	sinkConsole->set_formatter(fmt);

	sinkConsole->set_filter(severity_attr >= consoleLvlThreshold);
	
	// Регистрируем наш sink в core
	logging::core::get()->add_sink(sinkConsole);
	
	return sinkConsole;
}


/**
 * @brief Создание sink'а для логов в syslog
 * @param consoleLvlThreshold - минимальный уровень выводимых логов
 * @return
 */
boost::shared_ptr<syslog_sink> initSyslogSink(blog::lvl syslogLvlThreshold) {
	// backend для syslog'а
	boost::shared_ptr<sinks::syslog_backend> syslogBackend = boost::make_shared<sinks::syslog_backend>(
		keywords::facility = sinks::syslog::user,
		keywords::use_impl = sinks::syslog::native
	);
	
	// Заполняем таблицу соответствия наших уровней логирования с syslog'овскими.
	sinks::syslog::custom_severity_mapping<blog::lvl> mapping("Severity");
	mapping[blog::lvl::trace]   = sinks::syslog::debug;	//[0]=7
	mapping[blog::lvl::debug]   = sinks::syslog::debug;	//[1]=7
	mapping[blog::lvl::info]    = sinks::syslog::info;	//[2]=6
	// Пропускаем sinks::syslog::notice
	mapping[blog::lvl::warning] = sinks::syslog::warning;	//[3]=4
	mapping[blog::lvl::error]   = sinks::syslog::error;	//[4]=7
	// Пропускаем sinks::syslog::critical и sinks::syslog::alert
	mapping[blog::lvl::fatal]   = sinks::syslog::emergency;	//[5]=7
	
	// Задаём таблицу соответствия backend'у.
	syslogBackend->set_severity_mapper(mapping);
	
	// Создаём sink для вывода в syslog
	boost::shared_ptr<syslog_sink> sinkSyslog = boost::make_shared<syslog_sink>(syslogBackend);
	
	// В syslog пойдут только логи с соответствующей меткой
	sinkSyslog->set_filter(severity_attr >= syslogLvlThreshold && expr::has_attr(tag_attr) && tag_attr == "syslog");
	
	// Регистрируем наш sink в core
	logging::core::get()->add_sink(sinkSyslog);
	
	return sinkSyslog;
}


void blog::logInit(blog::lvl syslogLvlThreshold /*= blog::lvl::warning*/, blog::lvl consoleLvlThreshold /*= blog::lvl::info*/) {
	if(!BLOG_IS_CONSOLE_OUTPUT_DISABLED) {
		initConsoleSink(consoleLvlThreshold);
	}
	initSyslogSink(syslogLvlThreshold);
	
	// Глобальные атрибуты
	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
}

