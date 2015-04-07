/*
 * tx.hpp
 *
 *  Created on: 2015年3月4日
 *      Author: Administrator
 */

#ifndef BITLINK_TX_HPP
#define BITLINK_TX_HPP

#include <string>
#include <vector>
#include <map>

#include "model.hpp"
#include "db.hpp"

class transaction_type;

class tx_manager
{
public:
	void load(Db& db);
	tx &get(const std::string &tx_hash);
	bool import(const std::string tx_hex);
	const transaction_type create(const std::string &to_address, const int amount);
	void post_tx(const transaction_type& transaction);
private:
	std::map<std::string, tx> _tx_map;
};

class input_manager
{
public:
	friend class tx_manager;

	void load(Db& db);
	std::vector<input> &get(const std::string &tx_hash);
private:
	std::map<std::string, std::vector<input> > _input_map;
};

class output_manager
{
public:
	friend class tx_manager;

	void load(Db& db);
	std::vector<output> &get(const std::string &tx_hash);
	bool mark_used(output& output_, Db& db);
private:
	std::map<std::string, std::vector<output> > _output_map;
};


#endif /* BITLINK_TX_HPP */
