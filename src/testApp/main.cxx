/* 
 * File:   main.cxx
 * Author: kostya
 *
 * Created on 27 апреля 2018 г., 14:47
 */


#include <blog.h>


/*
 * 
 */
int main(int /*argc*/, char** /*argv*/) {
	blog::logInit();
	
	// используется уровень логирования по умолчанию (trace):
	BLOG_				<< "Простой вывод сообщения.";
	BLOG_SYSLOG_			<< "Вывод сообщения в syslog и консоль";
	
	// вывод с заданным уровнем логирования:
	BLOG(blog::lvl::warning)	<< "Простой вывод сообщения.";
	BLOG_SYSLOG(blog::lvl::error)	<< "Вывод сообщения в syslog и консоль";
	
	// вывод сообщения с указанием логического канала
	BLOG_CHAN("main", blog::lvl::info) << "Начинаем вычисление";
	auto calc([](){
		BLOG_CHAN("calculation", blog::lvl::error) << "Ошибка вычисления";
	});
	calc();

	return 0;
}

