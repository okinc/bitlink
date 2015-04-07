/*
 * account.hpp
 *
 *  Created on: 2015年3月9日
 *      Author: Administrator
 */

#ifndef BITLINK_ACCOUNT_HPP
#define BITLINK_ACCOUNT_HPP

#include "model.hpp"
#include "db.hpp"

class account_manager
{
public:
	void load(Db &db);
	void add_received(int received, Db &db);
	void add_sent(int spent, Db &db);

private:
	account _account;
};


#endif /* BITLINK_ACCOUNT_HPP */
