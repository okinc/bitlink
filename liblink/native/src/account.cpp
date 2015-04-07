/*
 * account.cpp
 *
 *  Created on: 2015年3月9日
 *      Author: Administrator
 */

#include <stdexcept>
#include <vector>

#include "SQLiteCpp/SQLiteCpp.h"

#include "ec_key.hpp"
#include "account.hpp"

void account_manager::load(Db &db)
{
	SQLite::Statement *query = db.load_account();

	std::vector<account> accounts;

	while(query->executeStep())
	{
		account account_;
		account_ = *query;
		accounts.push_back(account_);
	}

	delete query;

	if(accounts.size() != 1)
	{
		throw std::runtime_error("account table should have only one row");
	}

	_account = accounts[0];
}

void account_manager::add_received(int received_, Db &db)
{
	if(!db.add_received(_account, received_, false))
	{
		throw std::runtime_error("write account table fail 1");
	}

	_account.set_received(_account.get_received() + received_);
	_account.set_account_amount(_account.get_account_amount() + received_);
}

void account_manager::add_sent(int sent_, Db &db)
{
	if(!db.add_sent(_account, sent_, false))
	{
		throw std::runtime_error("write account table fail 2");
	}

	_account.set_sent(_account.get_sent() + sent_);
	_account.set_account_amount(_account.get_account_amount() - sent_);
}

