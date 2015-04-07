/*
 * coin.hpp
 *
 *  Created on: 2015年3月9日
 *      Author: Administrator
 */

#ifndef BITLINK_COIN_HPP
#define BITLINK_COIN_HPP


#include <set>
#include <vector>
#include <map>

#include "model.hpp"
#include "db.hpp"

inline bool operator<(const coin &A, const coin &B)
{
	return A.get_coin_id() < B.get_coin_id();
}

class coin_manager
{
public:
	void load(Db& db);
	void remove(coin &coin, Db& db);
	std::vector<coin> find(const int amount);
	coin &get(const std::string &tx_hash, const int index);
	bool get(const std::string &tx_hash, const int index, coin &coin_);
	bool add(const coin &coin_);
	bool add(const std::vector<coin> &coins_);
private:
	std::set<coin> _coin_set;
	std::map<std::string, coin> _coin_map;
};


#endif /* BITLINK_COIN_HPP */
