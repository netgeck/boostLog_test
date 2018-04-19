
#include <dbg.hpp>


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
	logInit();
	
	BLOG << "Log to syslog and console!";
	BLOG << "sleep: " << sleep(5);
	BLOG_SYSLOG << "Log to console!";
	
	return 0;
}