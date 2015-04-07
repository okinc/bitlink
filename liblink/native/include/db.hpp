/*
 * db.hpp
 *
 *  Created on: 2015年3月2日
 *      Author: Administrator
 */

#ifndef BITLINK_DB_HPP
#define BITLINK_DB_HPP

#include "model.hpp"


std::string db_path();


namespace SQLite { class Database; class Statement; class Transaction;}

class Db
{
public:
	bool write_account(account& account_, const bool new_transaction=true);
	SQLite::Statement *load_account();
	bool add_received(const account& account_, int received, const bool new_transaction=true);
	bool add_sent(const account& account_, int spent, const bool new_transaction=true);

	bool write_key(key& key_, const bool new_transaction=true);
	bool update_key_state(const key& key_, int state, const bool new_transaction=true);
	bool add_key_received(const key& key_, int received, const bool new_transaction=true);
	bool add_key_sent(const key& key_, int sent, const bool new_transaction=true);
	SQLite::Statement *load_keys();

	bool write_coins(coin& coin_, const bool new_transaction=true);
	SQLite::Statement *load_coins();
	bool remove_coin(const coin& coin_, const bool new_transaction=true);

	bool write_address(address& address_, const bool new_transaction=true);
	bool update_label(const address& address_, const std::string &label_, const bool new_transaction=true);
	bool remove_label(const address& address_, const bool new_transaction=true);
	SQLite::Statement *load_addresses();

	bool write_tx(tx& tx_, const bool new_transaction=true);
	SQLite::Statement *load_txes();

	bool write_input(input& input_, const bool new_transaction=true);
	SQLite::Statement *load_inputs();

	bool write_output(output& output_, const bool new_transaction=true);
	SQLite::Statement *load_outputs();
	bool mark_used(const output& output_, const bool new_transaction=true);

	void startTransaction();
	void commitTransaction();
	void rollbackTransaction();

	Db();

private:
	SQLite::Database _db;
	SQLite::Transaction *_transaction;
};



#endif /* BITLINK_DB_HPP */
