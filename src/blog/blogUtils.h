/* 
 * File:   blogUtility.h
 * Author: kostya
 *
 * Created on 15 мая 2018 г., 17:05
 */

#ifndef BLOGUTILITY_H
#define BLOGUTILITY_H


#include <sstream>


namespace blog {

/**
 * @brief Вывод элементов контейнера в строку
 * @param list - контейнер
 * @param separator - разделитель
 * @return строка
 */	
template <typename CONTAINER>
std::string listOut(const CONTAINER& list, char separator) {
	std::stringstream ss;
	for(auto it = list.begin(); it != list.end(); ++it) {
		ss << *it;
		if(std::next(it) != list.end()) {
			ss << separator;
		}
	}
	return ss.str();
}

}


#endif /* BLOGUTILITY_H */

