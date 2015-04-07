/*
 * label.hpp
 *
 *  Created on: 2015年3月9日
 *      Author: Administrator
 */

#ifndef BITLINK_LABEL_HPP
#define BITLINK_LABEL_HPP


#include <map>

#include "model.hpp"
#include "db.hpp"


class label_manager
{
public:
	void load(Db &db);
	address &get(const std::string& address_str);
	address add(const std::string& address_str, const std::string &label);
	void update(const std::string& address_str, const std::string &label);
	void remove(const std::string& address_str);
private:
	std::map<std::string, address> _label_map;
};


#endif /* BITLINK_LABEL_HPP */
