
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


BOOST_LOG_GLOBAL_LOGGER_INIT(blog::logSyslog, boost::log::sources::severity_logger<blog::lvl>) {
	boost::log::sources::severity_logger<blog::lvl> logger;
	logger.add_attribute("Tag", attrs::constant< std::string >("syslog"));
	return logger;
}


BOOST_LOG_ATTRIBUTE_KEYWORD(severity_attr, "Severity", blog::lvl)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel_attr, "Channel", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)


void blog::logInit(blog::lvl syslogLvlThreshold /*= blog::lvl::warning*/) {
	// Создаём sink для вывода в консоль
	typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
	boost::shared_ptr<text_sink> sinkConsole;
	if(!BLOG_IS_CONSOLE_OUTPUT_DISABLED) {
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
	}
	
	// Создаём sink для вывода в syslog
	typedef sinks::synchronous_sink<sinks::syslog_backend> syslog_sink;
	boost::shared_ptr<syslog_sink> sinkSyslog = boost::make_shared <syslog_sink>(
			keywords::facility = sinks::syslog::user,
			keywords::use_impl = sinks::syslog::native
		);
	// В syslog пойдут только логи с соответствующей меткой
	sinkSyslog->set_filter(severity_attr >= syslogLvlThreshold && expr::has_attr(tag_attr) && tag_attr == "syslog");
	
	// Регистрируем наши sink'и в core
	logging::core::get()->add_sink(sinkSyslog);
	if(!BLOG_IS_CONSOLE_OUTPUT_DISABLED) {
		logging::core::get()->add_sink(sinkConsole);
	}
	
	// Глобальные атрибуты
	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
}

