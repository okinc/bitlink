/*
 * label.cpp
 *
 *  Created on: 2015年3月9日
 *      Author: Administrator
 */


#include <stdexcept>

#include "SQLiteCpp/SQLiteCpp.h"

#include "ec_key.hpp"
#include "label.hpp"


void label_manager::load(Db &db)
{
	SQLite::Statement *query = db.load_addresses();

	while(query->executeStep())
	{
		address address_;
		address_ = *query;

		_label_map[address_.get_address_str()] = address_;
	}

	delete query;
}

address &label_manager::get(const std::string& address_str)
{
	return _label_map[address_str];
}

address label_manager::add(const std::string& address_str, const std::string &label)
{
	Db db;

	address address_;
	address_.set_address_str(address_str);
	address_.set_label(label);

	if(!db.write_address(address_))
	{
		throw std::runtime_error("add label fail");
	}

	_label_map[address_str] = address_;

	return address_;
}

void label_manager::update(const std::string& address_str, const std::string &label)
{
	Db db;

	address address_ = get(address_str);

	if(!db.update_label(address_, label))
	{
		throw std::runtime_error("update label fail");
	}

	address_.set_label(label);
}

void label_manager::remove(const std::string& address_str)
{
	Db db;

	address address_ = get(address_str);

	if(!db.remove_label(address_))
	{
		throw std::runtime_error("remove label fail");
	}

	_label_map.erase(address_str);
}
