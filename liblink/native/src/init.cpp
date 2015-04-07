#include "SQLiteCpp/SQLiteCpp.h"

#include "ec_key.hpp"
#include "primitives.hpp"
#include "model.hpp"
#include "tx.hpp"
#include "coin.hpp"
#include "key.hpp"
#include "account.hpp"
#include "db.hpp"
#include "init.hpp"


input_manager *pinput_manager = NULL;
output_manager *poutput_manager = NULL;
tx_manager *ptx_manager = NULL;
coin_manager *pcoin_manager = NULL;
key_manger *pkey_manger = NULL;
account_manager *paccount_manager = NULL;

static bool check_table()
{
	SQLite::Database db_(db_path(), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
	if(!db_.tableExists("ec_key"))
	{
		db_.exec("create table ec_key ( "
					"ec_key_id integer not null primary key autoincrement, "
					"ec_key_address text not null, "
					"ec_key_public text not null, "
					"ec_key_private text not null, "
					"received unsigned integer not null, "
					"sent unsigned integer not null, "
					"amount unsigned integer not null, "
					"state unsigned integer not null )");
	}

	if(!db_.tableExists("tx"))
	{
		db_.exec("create table tx ( "
					"tx_id integer not null primary key autoincrement, "
					"tx_hash text not null, "
					"tx_hex not null )");
	}

	if(!db_.tableExists("input"))
	{
		db_.exec("create table input ( "
					"input_id integer not null primary key autoincrement, "
					"tx_id unsigned integer not null, "
					"from_tx text not null, "
					"from_index unsigned integer not null, "
					"from_amount unsigned not null, "
					"from_address text not null, "
					"to_tx not null, "
					"to_index unsigned integer not null )");
	}

	if(!db_.tableExists("output"))
	{
		db_.exec("create table output ( "
					"output_id integer not null primary key autoincrement, "
					"tx_id unsigned integer not null, to_tx text not null, "
					"to_index unsigned integer not null, "
					"to_address text not null, "
					"to_amount unsigned integer not null, "
					"spent unsigned integer not null )");
	}

	if(!db_.tableExists("coin"))
	{
		db_.exec("create table coin ( "
					"coin_id integer not null primary key autoincrement, "
					"tx_id unsigned integer not null, "
					"tx_hash text not null, "
					"to_index unsigned integer not null, "
					"owner_address text not null, "
					"amount unsigned integer not null )");
	}

	if(!db_.tableExists("account"))
	{
		db_.exec("create table account ( "
					"account_id integer not null primary key autoincrement, "
					"account_name text not null, "
					"account_amount unsigned integer not null, "
					"received unsigned integer not null, "
					"sent unsigned integer not null )");
	}

	if(!db_.tableExists("address"))
	{
		db_.exec("create table address ( "
					"address_id integer not null primary key autoincrement, "
					"address_str text not null, "
					"label text not null )");
	}

	if(!db_.tableExists("config"))
	{
		db_.exec("create table config ( "
					"config_id integer not null primary key autoincrement, "
					"program_version unsigned integer not null, "
					"db_version unsigned integer not null )");
	}

	return true;
}

bool init()
{
	if(!check_table())
	{
		return false;
	}

	pinput_manager = new input_manager();
	poutput_manager = new output_manager();
	ptx_manager = new tx_manager();
	pcoin_manager = new coin_manager();
	pkey_manger = new key_manger();
	paccount_manager = new account_manager();

	return true;
}
