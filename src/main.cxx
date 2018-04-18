
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

#include <dbg.hpp>


namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

#if 0
void init() {
//	logging::add_file_log("sample.log");
	logging::add_file_log(
//		keywords::file_name = "sample_%N.log",
		keywords::file_name = "sample.log",
//		keywords::rotation_size = 10 * 1024 * 1024,
//		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
		keywords::format = "[%TimeStamp%]: %Message%"
	);
//	logging::add_common_attributes();
	logging::core::get()->add_global_attribute("TimeStamp", logging::attributes::local_clock());
	
	logging::core::get()->set_filter(
		logging::trivial::severity >= logging::trivial::info
	);
}
#endif 
#if 0	// равзёрнутый эквивалент для  logging::add_file_log("sample.log");
void init() {
	// Construct the sink
	typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
	boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

	// Add a stream to write log to
	sink->locked_backend()->add_stream(
		boost::make_shared< std::ofstream >("sample.log"));

	// Register the sink in the logging core
	logging::core::get()->add_sink(sink);
}
#endif
#if 0	// adding output to console could look as follows: 
void init() {
	// Construct the sink
	typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
	boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

	// We have to provide an empty deleter to avoid destroying the global stream object
	boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());
	// Add a stream to write log to
	sink->locked_backend()->add_stream(stream);

	// Register the sink in the logging core
	logging::core::get()->add_sink(sink);
}
#endif
#if 0	// выводим в консоль и в файл. 
//text_ostream_backend supports adding several streams
// Разница между несколькими синками и одним синком с разными целевыми потоками: 
// первый вариант более кастомизируемый, в то время как второй значительно более быстрый.
void init() {
	// Construct the sink
	typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
	boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

	// Add a stream to write log to
	sink->locked_backend()->add_stream(
		boost::make_shared< std::ofstream >("sample.log"));

	// We have to provide an empty deleter to avoid destroying the global stream object
	boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());
	// Add a stream to write log to
	sink->locked_backend()->add_stream(stream);

	// Register the sink in the logging core
	logging::core::get()->add_sink(sink);
}
#endif	//$ rm sample.log && ./testApp && echo "--- cat ./sample.log: ---" && cat ./sample.log

#if 0	// вывод в syslog и консоль
void init() {
	// Создаём sink для вывода в консоль
	typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;
	boost::shared_ptr<text_sink> sinkConsole = boost::make_shared<text_sink>();
	// We have to provide an empty deleter to avoid destroying the global stream object
	boost::shared_ptr<std::ostream> stream(&std::clog, boost::null_deleter());
	// Add a stream to write log to
	sinkConsole->locked_backend()->add_stream(stream);
	
	// Создаём sink для вывода в syslog
	typedef sinks::synchronous_sink<sinks::syslog_backend> syslog_sink;
	boost::shared_ptr<syslog_sink> sinkSyslog = boost::make_shared <syslog_sink>(
			keywords::facility = sinks::syslog::user,
			keywords::use_impl = sinks::syslog::native
		);
	
	// Регистрируем наши sink'и в core
	logging::core::get()->add_sink(sinkConsole);
	logging::core::get()->add_sink(sinkSyslog);
}
#endif	//$ rm sample.log && ./testApp && echo "--- cat ./sample.log: ---" && cat ./sample.log



int main(int, char*[]) {
#if 0
	init();
	BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
	BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
	BOOST_LOG_TRIVIAL(info) << "An informational severity message";
	BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
	BOOST_LOG_TRIVIAL(error) << "An error severity message";
	BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
#endif
#if 1
	logInit();
//	BOOST_LOG(logSyslog::get()) << "Log to syslog and console!";
//	BOOST_LOG(logDefault::get()) << "Log to console!";
	BLOG << "Log to syslog and console!";
	BLOG << "sleep: " << sleep(5);
	BLOG_SYSLOG << "Log to console!";
#endif

	return 0;
}