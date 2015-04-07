#include <stdexcept>

#include "SQLiteCpp/SQLiteCpp.h"

#include "ec_key.hpp"

#include "key.hpp"

#include "model.hpp"
#include "db.hpp"

#define EC_KEY_ADDRESS 2
#define EC_KEY_PUBLIC 3
#define EC_KEY_PRIVATE 4
#define RECEIVED 5
#define SENT 6
#define AMOUNT 7


void key_manger::load(Db& db)
{
	 SQLite::Statement *query = db.load_keys();

	 while(query->executeStep())
	 {
		 key key_;
		 key_ = *query;
		 key_.init();

		 _key_map[key_.get_ec_key_address()] = key_;
	 }

	 delete query;
}

key &key_manger::get(const std::string& address)
{
	return _key_map[address];
}

bool key_manger::get(const std::string& address, key &key_)
{
	std::map<std::string, key>::iterator it = _key_map.find(address);
	if(it == _key_map.end())
	{
		return false;
	}

	key_ = it->second;
	return true;
}

key &key_manger::create()
{
	key key_;
	key_.create();

	Db db;
	if(!db.write_key(key_))
	{
		throw std::runtime_error("create key fail");
	}

	_key_map[key_.get_ec_key_address()] = key_;
	return _key_map[key_.get_ec_key_address()];
}

bool key_manger::import(const std::string &priv_key_)
{
	key key_;
	if(!key_.import(priv_key_))
	{
		return false;
	}

	Db db;
	if(!db.write_key(key_))
	{
		throw std::runtime_error("create key fail");
	}

	_key_map[key_.get_ec_key_address()] = key_;

	return true;
}

bool key_manger::remove(key &key_)
{
	Db db;
	if(!db.update_key_state(key_, 1))
	{
		return false;
	}

	key_.set_state(1);
	_key_map.erase(key_.get_ec_key_address());

	return true;
}

bool key_manger::add_received(key &key_, int received, Db &db)
{
	if(!db.add_key_received(key_, received, false))
	{
		return false;
	}

	key_.set_received(key_.get_received() + received);
	key_.set_amount(key_.get_amount() + received);

	return true;
}

bool key_manger::add_sent(key &key_, int sent, Db &db)
{
	if(!db.add_key_sent(key_, sent, false))
	{
		return false;
	}

	key_.set_sent(key_.get_sent() + sent);
	key_.set_amount(key_.get_amount() - sent);

	return true;
}

