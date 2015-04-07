/*
 * model.cpp
 *
 *  Created on: 2015年3月4日
 *      Author: Administrator
 */

#include <stdexcept>

#include "ec_key.hpp"
#include "model.hpp"
#include "utility/base58.hpp"
#include "address.hpp"

#include "SQLiteCpp/SQLiteCpp.h"

#include "db.hpp"


key::key() : _ec_key_id(0), _ec_key_address(""), _ec_key_public(""), _ec_key_private(""),
			_received(0), _sent(0), _amount(0), _state(0),
			_init(false),
			_ec_key()
{}


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


key& key::operator=(SQLite::Statement& query)
{
	this->set_ec_key_id(query.getColumn(EC_KEY_ID));
	this->set_ec_key_address(query.getColumn(EC_KEY_ADDRESS));
	this->set_ec_key_public(query.getColumn(EC_KEY_PUBLIC));
	this->set_ec_key_private(query.getColumn(EC_KEY_PRIVATE));
	this->set_received(query.getColumn(KEY_RECEIVED));
	this->set_sent(query.getColumn(KEY_SENT));
	this->set_amount(query.getColumn(KEY_AMOUNT));
	this->set_state(query.getColumn(KEY_STATE));

	return *this;
}

input& input::operator=(SQLite::Statement& query)
{
	this->set_input_id(query.getColumn(INPUT_ID));
	this->set_tx_id(query.getColumn(INPUT_TX_ID));
	this->set_from_tx(query.getColumn(FROM_TX));
	this->set_from_index(query.getColumn(FROM_INDEX));
	this->set_from_amount(query.getColumn(FROM_AMOUNT));
	this->set_from_address(query.getColumn(FROM_ADDRESS));
	this->set_to_tx(query.getColumn(TO_TX));
	this->set_to_index(query.getColumn(TO_INDEX));

	return *this;
}

output& output::operator=(SQLite::Statement& query)
{
	this->set_output_id(query.getColumn(OUTPUT_ID));
	this->set_tx_id(query.getColumn(OUTPUT_TX_ID));
	this->set_to_tx(query.getColumn(OUTPUT_TO_TX));
	this->set_to_index(query.getColumn(OUTPUT_TO_INDEX));
	this->set_to_address(query.getColumn(TO_ADDRESS));
	this->set_to_amount(query.getColumn(TO_AMOUNT));
	this->set_spent(query.getColumn(SPENT));

	return *this;
}

tx & tx::operator=(SQLite::Statement& query)
{
	this->set_tx_id(query.getColumn(TX_ID));
	this->set_tx_hash(query.getColumn(TX_HASH));
	this->set_tx_hex(query.getColumn(TX_HEX));

	return *this;
}

coin& coin::operator=(SQLite::Statement& query)
{
	this->set_coin_id(query.getColumn(COIN_ID));
	this->set_tx_id(query.getColumn(COIN_TX_ID));
	this->set_tx_hash(query.getColumn(COIN_TX_HASH));
	this->set_to_index(query.getColumn(COIN_TO_INDEX));
	this->set_owner_address(query.getColumn(OWNER_ADDRESS));
	this->set_amount(query.getColumn(COIN_AMOUNT));

	return *this;
}

account& account::operator=(SQLite::Statement& query)
{
	this->set_account_id(query.getColumn(ACCOUNT_ID));
	this->set_account_name(query.getColumn(ACCOUNT_NAME));
	this->set_account_amount(query.getColumn(ACCOUNT_AMOUNT));
	this->set_received(query.getColumn(ACCOUNT_RECEIVED));
	this->set_sent(query.getColumn(ACCOUNT_SENT));

	return *this;
}

address& address::operator=(SQLite::Statement& query)
{
	this->set_address_id(query.getColumn(ADDRESS_ID));
	this->set_address_str(query.getColumn(ADDRESS_STR));
	this->set_label(query.getColumn(LABEL));

	return *this;
}

config& config::operator=(SQLite::Statement& query)
{
	this->set_config_id(query.getColumn(CONFIG_ID));
	this->set_program_version(query.getColumn(PROGRAM_VERSION));
	this->set_db_version(query.getColumn(DB_VERSION));

	return *this;
}

void key::init()
{
	_ec_key.set_private_key(private_data(get_ec_key_private().begin(), get_ec_key_private().end()));
	if(_ec_key.public_key() != data_chunk(get_ec_key_public().begin(), get_ec_key_public().end()))
	{
		throw std::runtime_error("private key and public key do not match");
	}

	payment_address address;
	set_public_key(address, _ec_key.public_key());
	if(address.encoded() != get_ec_key_address())
	{
		throw std::runtime_error("address and public key do not match");
	}

	_init = true;
}

bool key::create()
{
	_ec_key.new_key_pair();

	private_data private_data_ = _ec_key.private_key();
	set_ec_key_private(std::string(private_data_.begin(), private_data_.end()));

	data_chunk public_key_ = _ec_key.public_key();
	set_ec_key_public(std::string(public_key_.begin(), public_key_.end()));

	payment_address address;
	set_public_key(address, public_key_);

	set_ec_key_address(address.encoded());

	return true;
}

bool key::import(const std::string &priv_key_)
{
	data_chunk data_chunk_ = decode_base58(priv_key_);
	secret_parameter secret_parameter_;
	if(data_chunk_.size() != secret_parameter_.size())
	{
		throw std::runtime_error("private key size error");
	}

	for(int i = 0; i < secret_parameter_.size(); i++)
	{
		secret_parameter_[i] = data_chunk_[i];
	}

	return _ec_key.set_secret(secret_parameter_);
}
