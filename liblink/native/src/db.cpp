#include <stdexcept>

#include <sys/stat.h>
#include <unistd.h>

#include "SQLiteCpp/SQLiteCpp.h"

#include "ec_key.hpp"
#include "model.hpp"
#include "db.hpp"


#define EC_KEY_ID 0
#define EC_KEY_ADDRESS 1
#define EC_KEY_PUBLIC 2
#define EC_KEY_PRIVATE 3
#define KEY_RECEIVED 4
#define KEY_SENT 5
#define KEY_AMOUNT 6
#define KEY_STATE 7

#define INPUT_ID 0
#define INPUT_TX_ID 1
#define FROM_TX 2
#define FROM_INDEX 3
#define FROM_AMOUNT 4
#define FROM_ADDRESS 5
#define TO_TX 6
#define TO_INDEX 7

#define OUTPUT_ID 0
#define OUTPUT_TX_ID 1
#define OUTPUT_TO_TX 2
#define OUTPUT_TO_INDEX 3
#define TO_ADDRESS 4
#define TO_AMOUNT 5
#define SPENT 6

#define TX_ID 0
#define TX_HASH 1
#define TX_HEX 2

#define COIN_ID 0
#define COIN_TX_ID 1
#define COIN_TX_HASH 2
#define COIN_TO_INDEX 3
#define OWNER_ADDRESS 4
#define COIN_AMOUNT 5

#define ACCOUNT_ID 0
#define ACCOUNT_NAME 1
#define ACCOUNT_AMOUNT 2
#define ACCOUNT_RECEIVED 3
#define ACCOUNT_SENT 4

#define ADDRESS_ID 0
#define ADDRESS_STR 1
#define LABEL 2

#define CONFIG_ID 0
#define PROGRAM_VERSION 1
#define DB_VERSION 2


#define DATABASE_FILENAME	"bitlink.db3"

std::string db_path()
{
	struct stat sb;
	std::string path = "//data//data//okcoin.oklink.bitlink//databases";

	int32_t res = stat(path.c_str(), &sb);
	if (0 == res && (sb.st_mode & S_IFDIR))
	{
	}
	else
	{
	    int status = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IWOTH | S_IXOTH);
	    if(status != 0){
	        throw std::runtime_error("Error occurred while creating database path : "+path);
	    }
	}

	return path + "//" + DATABASE_FILENAME;
}


Db::Db() : _db(db_path(), SQLITE_OPEN_READWRITE), _transaction(NULL)
{

}

void Db::startTransaction()
{
	if(_transaction != NULL)
	{
		delete _transaction;
		_transaction = NULL;
	}
	_transaction = new SQLite::Transaction(_db);
}

void Db::commitTransaction()
{
	_transaction->commit();
	delete _transaction;
	_transaction = NULL;
}

void Db::rollbackTransaction()
{
	delete _transaction;
	_transaction = NULL;
}

bool Db::write_account(account& account_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "insert into `account_name` "
									"(`account_name`, `account_amount`, `received`, `sent`) "
									"values (?, ?, ?, ?)");

	query.bind(ACCOUNT_NAME, account_.get_account_name());
	query.bind(ACCOUNT_AMOUNT, account_.get_account_amount());
	query.bind(ACCOUNT_RECEIVED, account_.get_received());
	query.bind(ACCOUNT_SENT, account_.get_sent());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		account_.set_account_id((int)_db.getLastInsertRowid());
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

bool Db::add_received(const account& account_, int received_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "update account set received=received+?, account_amount=account_amount+? where account_id=?");

	query.bind(1, received_);
	query.bind(2, received_);
	query.bind(3, account_.get_account_id());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

bool Db::add_sent(const account& account_, int spent_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "update account set sent=sent+?, account_amount=account_amount-? where account_id=?");

	query.bind(1, spent_);
	query.bind(2, spent_);
	query.bind(3, account_.get_account_id());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

SQLite::Statement *Db::load_account()
{
	return new SQLite::Statement(_db, "SELECT * FROM `account`");
}

bool Db::write_key(key& key_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "insert into `ec_key` "
								"(`ec_key_address`, `ec_key_public`, `ec_key_private`, "
									"`received`, `sent`, `amount`, `state`) "
								"values (?, ?, ?, ?, ?, ?, ?)");
	query.bind(EC_KEY_ADDRESS, key_.get_ec_key_address());
	query.bind(EC_KEY_PUBLIC, key_.get_ec_key_public());
	query.bind(EC_KEY_PRIVATE, key_.get_ec_key_private());
	query.bind(KEY_RECEIVED, key_.get_received());
	query.bind(KEY_SENT, key_.get_sent());
	query.bind(KEY_AMOUNT, key_.get_amount());
	query.bind(KEY_STATE, key_.get_state());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		key_.set_ec_key_id((int)_db.getLastInsertRowid());
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

bool Db::update_key_state(const key& key_, int state_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "update `key` set state=? where ec_key_id=?");
	query.bind(1, state_);
	query.bind(2, key_.get_ec_key_id());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

bool Db::add_key_received(const key& key_, int received, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "update `key` set received=received+?, amount=amount+? where ec_key_id=?");
	query.bind(1, received);
	query.bind(2, received);
	query.bind(3, key_.get_ec_key_id());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

bool Db::add_key_sent(const key& key_, int sent, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "update `key` set sent=sent+?, amount=amount-? where ec_key_id=?");
	query.bind(1, sent);
	query.bind(2, sent);
	query.bind(3, key_.get_ec_key_id());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

SQLite::Statement *Db::load_keys()
{
	return new SQLite::Statement(_db, "SELECT * FROM `ec_key` where state=0");
}

bool Db::write_coins(coin& coin_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "insert into `coin` "
									"(`tx_id`, `tx_hash`, `to_index`, `owner_address`, `amount`) "
									"values (?, ?, ?, ?, ?)");

	query.bind(COIN_TX_ID, coin_.get_tx_id());
	query.bind(COIN_TX_HASH, coin_.get_tx_hash());
	query.bind(COIN_TO_INDEX, coin_.get_to_index());
	query.bind(OWNER_ADDRESS, coin_.get_owner_address());
	query.bind(COIN_AMOUNT, coin_.get_amount());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		coin_.set_coin_id((int)_db.getLastInsertRowid());
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

SQLite::Statement *Db::load_coins()
{
	return new SQLite::Statement(_db, "SELECT * FROM `coin`");
}

bool Db::write_address(address& address_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "insert into `address` "
									"(`address_str`, `label`) "
									"values (?, ?)");

	query.bind(ADDRESS_STR, address_.get_address_str());
	query.bind(LABEL, address_.get_label());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		address_.set_address_id((int)_db.getLastInsertRowid());
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

bool Db::update_label(const address& address_, const std::string &label_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "update `address` "
									"set `label`=? "
									"where `address_id`=?");

	query.bind(1, label_);
	query.bind(2, address_.get_address_id());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

bool Db::remove_label(const address& address_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "delete from `address` "
									"where `address_id`=?");

	query.bind(1, address_.get_address_id());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

SQLite::Statement *Db::load_addresses()
{
	return new SQLite::Statement(_db, "SELECT * FROM `address`");
}

bool Db::write_tx(tx& tx_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "insert into `tx` "
									"(`tx_hash`, `tx_hex`) "
									"values (?, ?)");

	query.bind(TX_HASH, tx_.get_tx_hash());
	query.bind(TX_HEX, tx_.get_tx_hex());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		tx_.set_tx_id((int)_db.getLastInsertRowid());
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

SQLite::Statement *Db::load_txes()
{
	return new SQLite::Statement(_db, "SELECT * FROM `tx`");
}

bool Db::write_input(input& input_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "insert into `input` "
									"(`tx_id`, `from_tx`, `from_index`, `from_amount`, `from_address`, `to_tx`, `to_index`) "
									"values (?, ?, ?, ?, ?, ?, ?)");

	query.bind(INPUT_TX_ID, input_.get_tx_id());
	query.bind(FROM_TX, input_.get_from_tx());
	query.bind(FROM_INDEX, input_.get_from_index());
	query.bind(FROM_AMOUNT, input_.get_from_amount());
	query.bind(FROM_ADDRESS, input_.get_from_address());
	query.bind(TO_TX, input_.get_to_tx());
	query.bind(TO_INDEX, input_.get_to_index());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		input_.set_input_id((int)_db.getLastInsertRowid());
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

SQLite::Statement *Db::load_inputs()
{
	return new SQLite::Statement(_db, "SELECT * FROM `input`");
}

bool Db::write_output(output& output_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "insert into `output` "
									"(`tx_id`, `to_tx`, `to_index`, `to_address`, `to_amount`, `spent`) "
									"values (?, ?, ?, ?, ?, ?)");

	query.bind(OUTPUT_TX_ID, output_.get_tx_id());
	query.bind(OUTPUT_TO_TX, output_.get_to_tx());
	query.bind(OUTPUT_TO_INDEX, output_.get_to_index());
	query.bind(TO_ADDRESS, output_.get_to_address());
	query.bind(TO_AMOUNT, output_.get_to_amount());
	query.bind(SPENT, output_.get_spent());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		output_.set_output_id((int)_db.getLastInsertRowid());
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

bool Db::mark_used(const output& output_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "update output set spent=1 where output_id=?");

	query.bind(1, output_.get_output_id());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}

SQLite::Statement *Db::load_outputs()
{
	return new SQLite::Statement(_db, "SELECT * FROM `output`");
}

bool Db::remove_coin(const coin& coin_, const bool new_transaction)
{
	if(new_transaction)
		startTransaction();

	SQLite::Statement query(_db, "delete from coin where coin_id=?");

	query.bind(1, coin_.get_coin_id());

	if(query.exec() == 1)
	{
		if(new_transaction)
			commitTransaction();
		return true;
	}
	else
	{
		if(new_transaction)
			rollbackTransaction();
		return false;
	}
}
