/*
 * coin.cpp
 *
 *  Created on: 2015年3月9日
 *      Author: Administrator
 */



#include <stdexcept>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "SQLiteCpp/SQLiteCpp.h"

#include "ec_key.hpp"
#include "coin.hpp"

void coin_manager::load(Db& db)
{
	SQLite::Statement *query = db.load_coins();

	while (query->executeStep())
	{
		coin coin_;
		coin_ = *query;

		_coin_set.insert(coin_);
		_coin_map[coin_.get_tx_hash()+":"+boost::lexical_cast<std::string>(coin_.get_to_index())] = coin_;
	}

	delete query;
}

void coin_manager::remove(coin &coin_, Db& db)
{
	if(db.remove_coin(coin_, false) == false)
	{
		throw std::runtime_error("remove coin fail 1");
	}

	if(_coin_set.erase(coin_) != 1)
	{
		throw std::runtime_error("remove coin fail 2");
	}

	if(_coin_map.erase(coin_.get_tx_hash()+":"+boost::lexical_cast<std::string>(coin_.get_to_index()))
			!= 1)
	{
		throw std::runtime_error("remove coin fail 3");
	}
}

std::vector<coin> coin_manager::find(const int amount)
{
	std::vector<coin> candidates;
	int total = 0;
	BOOST_FOREACH(const coin &coin_, _coin_set)
	{
		candidates.push_back(coin_);
		total += coin_.get_amount();
		if(total >= amount)
		{
			return candidates;
		}
	}

	return std::vector<coin>();
}

coin &coin_manager::get(const std::string &tx_hash, const int index)
{
	return _coin_map[tx_hash+":"+boost::lexical_cast<std::string>(index)];
}

bool coin_manager::get(const std::string &tx_hash, const int index, coin &coin_)
{
	std::map<std::string, coin>::iterator it = _coin_map.find(tx_hash+":"+boost::lexical_cast<std::string>(index));
	if(it == _coin_map.end())
	{
		return false;
	}

	coin_ = it->second;
	return true;
}

bool coin_manager::add(const coin &coin_)
{
	_coin_set.insert(coin_);
	_coin_map[coin_.get_tx_hash()+":"+boost::lexical_cast<std::string>(coin_.get_to_index())] = coin_;
	return true;
}

bool coin_manager::add(const std::vector<coin> &coins_)
{
	BOOST_FOREACH(const coin &coin_, coins_)
	{
		add(coin_);
	}
	return true;
}


