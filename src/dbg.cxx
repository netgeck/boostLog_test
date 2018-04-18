/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "dbg.hpp"




#include <fstream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#   include <boost/shared_ptr.hpp>
#   include <boost/date_time/posix_time/posix_time_types.hpp>
#   include <boost/log/trivial.hpp>
#   include <boost/log/core.hpp>
#   include <boost/log/expressions.hpp>
#   include <boost/log/sources/logger.hpp>
#   include <boost/log/utility/setup/file.hpp>
#   include <boost/log/utility/setup/console.hpp>
#   include <boost/log/utility/setup/common_attributes.hpp>
#   include <boost/log/support/date_time.hpp>
#   include <boost/log/sinks/sync_frontend.hpp>
#   include <boost/log/sinks/text_file_backend.hpp>
#   include <boost/log/sinks/text_ostream_backend.hpp>
# include <boost/log/attributes/named_scope.hpp>

#include <dbg.hpp>
namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;




BOOST_LOG_GLOBAL_LOGGER_INIT(logSyslog, boost::log::sources::logger_mt) {
	boost::log::sources::logger_mt logger;
	logger.add_attribute("Tag", attrs::constant< std::string >("syslog"));
	return logger;
}

BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)

void logInit() {
	// Создаём sink для вывода в консоль
	typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
	boost::shared_ptr<text_sink> sinkConsole = boost::make_shared<text_sink>();
	// We have to provide an empty deleter to avoid destroying the global stream object
	boost::shared_ptr<std::ostream> stream(&std::clog, boost::null_deleter());
	// Add a stream to write log to
	sinkConsole->locked_backend()->add_stream(stream);
	// Set the log record formatter
        sinkConsole->set_formatter(
		expr::format("[%1%]: %2%")
			//Дефолтный: expr::attr<boost::posix_time::ptime>("TimeStamp")
			% expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
			% expr::smessage
		);
	
	// Создаём sink для вывода в syslog
	typedef sinks::synchronous_sink<sinks::syslog_backend> syslog_sink;
	boost::shared_ptr<syslog_sink> sinkSyslog = boost::make_shared <syslog_sink>(
			keywords::facility = sinks::syslog::user,
			keywords::use_impl = sinks::syslog::native
		);
	// В syslog пойдут только логи с соответствующей меткой
	sinkSyslog->set_filter(/*severity >= warning &&*/ (expr::has_attr(tag_attr) && tag_attr == "syslog"));
	
	// Регистрируем наши sink'и в core
	logging::core::get()->add_sink(sinkConsole);
	logging::core::get()->add_sink(sinkSyslog);
	
	// Глобальные атрибуты
	logging::core::get()->add_global_attribute("TimeStamp", attrs::local_clock());
}

