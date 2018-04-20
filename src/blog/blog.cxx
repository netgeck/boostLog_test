
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


namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;


BOOST_LOG_GLOBAL_LOGGER_INIT(logSyslog, boost::log::sources::severity_logger<blog::lvl>) {
	boost::log::sources::severity_logger<blog::lvl> logger;
	logger.add_attribute("Tag", attrs::constant< std::string >("syslog"));
	return logger;
}


BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", logging::trivial::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)


void blog::logInit(blog::lvl syslogLvlThreshold /*= blog::lvl::warning*/) {
	// Создаём sink для вывода в консоль
	typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
	boost::shared_ptr<text_sink> sinkConsole;
	if(!BLOG_TO_CONSOLE_DISABLED) {
		sinkConsole = boost::make_shared<text_sink>();
		// We have to provide an empty deleter to avoid destroying the global stream object
		boost::shared_ptr<std::ostream> stream(&std::clog, boost::null_deleter());
		// Add a stream to write log to
		sinkConsole->locked_backend()->add_stream(stream);
		// Задаём формат вывода
		sinkConsole->set_formatter(
			expr::format("[%1%] (%2%): %3%")
				//Дефолтный: expr::attr<boost::posix_time::ptime>("TimeStamp")
				% expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
				% expr::attr<blog::lvl>("Severity")
				% expr::smessage
			);
	}
	
	// Создаём sink для вывода в syslog
	typedef sinks::synchronous_sink<sinks::syslog_backend> syslog_sink;
	boost::shared_ptr<syslog_sink> sinkSyslog = boost::make_shared <syslog_sink>(
			keywords::facility = sinks::syslog::user,
			keywords::use_impl = sinks::syslog::native
		);
	// В syslog пойдут только логи с соответствующей меткой
	sinkSyslog->set_filter(severity >= syslogLvlThreshold && expr::has_attr(tag_attr) && tag_attr == "syslog");
	
	// Регистрируем наши sink'и в core
	logging::core::get()->add_sink(sinkSyslog);
	if(!BLOG_TO_CONSOLE_DISABLED) {
		logging::core::get()->add_sink(sinkConsole);
	}
	
	// Глобальные атрибуты
	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
}

