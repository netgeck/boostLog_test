
#include <blog.h>


int main(int, char*[]) {
	blog::logInit();
	
	BLOG << "Log console only!";
//	BLOG << "sleep: " << sleep(5);
	BLOG_SYSLOG << "Log to syslog and console!";
	
	BLOG_SEV(blog::lvl::trace)	<< "trace message (console only!)";
	BLOG_SEV(blog::lvl::debug)	<< "debug message (console only!)";
	BLOG_SEV(blog::lvl::info)	<< "info message (console only!)";
	BLOG_SEV(blog::lvl::warning)	<< "warning message (console only!)";
	BLOG_SEV(blog::lvl::error)	<< "error message (console only!)";
	BLOG_SEV(blog::lvl::fatal)	<< "fatal message (console only!)";
	
	BLOG_SYSLOG_SEV(blog::lvl::trace)	<< "trace message";
	BLOG_SYSLOG_SEV(blog::lvl::debug)	<< "debug message";
	BLOG_SYSLOG_SEV(blog::lvl::info)	<< "info message";
	BLOG_SYSLOG_SEV(blog::lvl::warning)	<< "warning message";
	BLOG_SYSLOG_SEV(blog::lvl::error)	<< "error message";
	BLOG_SYSLOG_SEV(blog::lvl::fatal)	<< "fatal message";
	
	return 0;
}
