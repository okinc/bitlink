/*
 * model.hpp
 *
 *  Created on: 2015年3月4日
 *      Author: Administrator
 */

#ifndef BITLINK_MODEL_HPP
#define BITLINK_MODEL_HPP

#include <string>

namespace SQLite { class Statement; }
class elliptic_curve_key;

class key
{
public:
	friend class tx_manager;
private:
	int _ec_key_id;
	std::string _ec_key_address;
	std::string _ec_key_public;
	std::string _ec_key_private;
	int _received;
	int _sent;
	int _amount;
	int _state;
public:
	int get_ec_key_id() const
	{
		return _ec_key_id;
	}
	std::string get_ec_key_address() const
	{
		return _ec_key_address;
	}
	std::string get_ec_key_public() const
	{
		return _ec_key_public;
	}
	std::string get_ec_key_private() const
	{
		return _ec_key_private;
	}
	int get_received() const
	{
		return _received;
	}
	int get_sent() const
	{
		return _sent;
	}
	int get_amount() const
	{
		return _amount;
	}
	int get_state() const
	{
		return _state;
	}

	void set_ec_key_id(int ec_key_id)
	{
		_ec_key_id = ec_key_id;
	}
	void set_ec_key_address(const std::string &ec_key_address)
	{
		_ec_key_address = ec_key_address;
	}
	void set_ec_key_public(const std::string &ec_key_public)
	{
		_ec_key_public = ec_key_public;
	}
	void set_ec_key_private(const std::string &ec_key_private)
	{
		_ec_key_private = ec_key_private;
	}
	void set_received(int received)
	{
		_received = received;
	}
	void set_sent(int sent)
	{
		_sent = sent;
	}
	void set_amount(int amount)
	{
		_amount = amount;
	}
	void set_state(int state)
	{
		_state = state;
	}

public:
	bool create();
	bool import(const std::string &priv_key_);
	bool remove();

private:
	bool _init;
	elliptic_curve_key _ec_key;
public:
	key();

	key& operator=(SQLite::Statement& query);

	void init();
};

class input
{
private:
	int _input_id;
	int _tx_id;
	std::string _from_tx;
	int _from_index;
	int _from_amount;
	std::string _from_address;
	std::string _to_tx;
	int _to_index;
public:
	int get_input_id() const
	{
		return _input_id;
	}
	int get_tx_id() const
	{
		return _tx_id;
	}
	std::string get_from_tx() const
	{
		return _from_tx;
	}
	int get_from_index() const
	{
		return _from_index;
	}
	int get_from_amount() const
	{
		return _from_amount;
	}
	std::string get_from_address() const
	{
		return _from_address;
	}
	std::string get_to_tx() const
	{
		return _to_tx;
	}
	int get_to_index() const
	{
		return _to_index;
	}

	void set_input_id(int input_id)
	{
		_input_id = input_id;
	}
	void set_tx_id(int tx_id)
	{
		_tx_id = tx_id;
	}
	void set_from_tx(const std::string &from_tx)
	{
		_from_tx = from_tx;
	}
	void set_from_index(int from_index)
	{
		_from_index = from_index;
	}
	void set_from_amount(int from_amount)
	{
		_from_amount = from_amount;
	}
	void set_from_address(const std::string &from_address)
	{
		_from_address = from_address;
	}
	void set_to_tx(const std::string& to_tx)
	{
		_to_tx = to_tx;
	}
	void set_to_index(int to_index)
	{
		_to_index = to_index;
	}

private:
	bool _init;

public:
	input() : _input_id(0), _tx_id(0), _from_tx(""), _from_index(0), _from_amount(0),
				_from_address(""), _to_tx(""), _to_index(0),
				_init(false)
	{}

	input& operator=(SQLite::Statement& query);

	void init()
	{}
};

class output
{
private:
	int _output_id;
	int _tx_id;
	std::string _to_tx;
	int _to_index;
	std::string _to_address;
	int _to_amount;
	int _spent;

public:
	int get_output_id() const
	{
		return _output_id;
	}
	int get_tx_id() const
	{
		return _tx_id;
	}
	std::string get_to_tx() const
	{
		return _to_tx;
	}
	int get_to_index() const
	{
		return _to_index;
	}
	std::string get_to_address() const
	{
		return _to_address;
	}
	int get_to_amount() const
	{
		return _to_amount;
	}
	int get_spent() const
	{
		return _spent;
	}

	void set_output_id(int output_id)
	{
		_output_id = output_id;
	}
	void set_tx_id(int tx_id)
	{
		_tx_id = tx_id;
	}
	void set_to_tx(const std::string& to_tx)
	{
		_to_tx = to_tx;
	}
	void set_to_index(int to_index)
	{
		_to_index = to_index;
	}
	void set_to_address(const std::string &to_address)
	{
		_to_address = to_address;
	}
	void set_to_amount(int to_amount)
	{
		_to_amount = to_amount;
	}
	void set_spent(int spent)
	{
		_spent = spent;
	}

private:
	bool _init;

public:
	output() : _output_id(0), _tx_id(0), _to_tx(""), _to_index(0),
				_to_address(""), _to_amount(0), _spent(0),
				_init(false)
	{}

	output& operator=(SQLite::Statement& query);
	void init()
	{}
};

class tx
{
private:
	int _tx_id;
	std::string _tx_hash;
	std::string _tx_hex;

public:
	int get_tx_id() const
	{
		return _tx_id;
	}
	std::string get_tx_hash() const
	{
		return _tx_hash;
	}
	std::string get_tx_hex() const
	{
		return _tx_hex;
	}

	void set_tx_id(int tx_id)
	{
		_tx_id = tx_id;
	}
	void set_tx_hash(const std::string &tx_hash)
	{
		_tx_hash = tx_hash;
	}
	void set_tx_hex(const std::string &tx_hex)
	{
		_tx_hex = tx_hex;
	}

private:
	bool _init;

public:
	tx() : _tx_id(0), _tx_hash(""), _tx_hex(""),
			_init(false)
	{}

	tx &operator=(SQLite::Statement& query);
	void init()
	{}
};

class coin
{
private:
	int _coin_id;
	int _tx_id;
	std::string _tx_hash;
	int _to_index;
	std::string _owner_address;
	int _amount;

public:
	int get_coin_id() const
	{
		return _coin_id;
	}
	int get_tx_id() const
	{
		return _tx_id;
	}
	std::string get_tx_hash() const
	{
		return _tx_hash;
	}
	int get_to_index() const
	{
		return _to_index;
	}
	std::string get_owner_address() const
	{
		return _owner_address;
	}
	int get_amount() const
	{
		return _amount;
	}

	void set_coin_id(int coin_id)
	{
		_coin_id = coin_id;
	}
	void set_tx_id(int tx_id)
	{
		_tx_id = tx_id;
	}
	void set_tx_hash(const std::string &tx_hash)
	{
		_tx_hash = tx_hash;
	}
	void set_to_index(int to_index)
	{
		_to_index = to_index;
	}
	void set_owner_address(const std::string &owner_address)
	{
		_owner_address = owner_address;
	}
	void set_amount(int amount)
	{
		_amount = amount;
	}

private:
	bool _init;

public:
	coin() : _coin_id(0), _tx_id(0), _to_index(0), _owner_address(""), _amount(0),
		_init(false)
	{}

	coin& operator=(SQLite::Statement& query);
	void init();
};

class account
{
private:
	int _account_id;
	std::string _account_name;
	int _account_amount;
	int _received;
	int _sent;

public:
	int get_account_id() const
	{
		return _account_id;
	}
	std::string get_account_name() const
	{
		return _account_name;
	}
	int get_account_amount() const
	{
		return _account_amount;
	}
	int get_received() const
	{
		return _received;
	}
	int get_sent() const
	{
		return _sent;
	}

	void set_account_id(int account_id)
	{
		_account_id = account_id;
	}
	void set_account_name(const std::string &account_name)
	{
		_account_name = account_name;
	}
	void set_account_amount(int account_amount)
	{
		_account_amount = account_amount;
	}
	void set_received(int received)
	{
		_received = received;
	}
	void set_sent(int sent)
	{
		_sent = sent;
	}

private:
	bool _init;

public:
	account() : _account_id(0), _account_name(""), _account_amount(0), _received(0), _sent(0),
				_init(false)
	{}

	account &operator=(SQLite::Statement& query);
	void init();
};

class address
{
private:
	int _address_id;
	std::string _address_str;
	std::string _label;

public:
	int get_address_id() const
	{
		return _address_id;
	}
	std::string get_address_str() const
	{
		return _address_str;
	}
	std::string get_label() const
	{
		return _label;
	}

	void set_address_id(int address_id)
	{
		_address_id = address_id;
	}
	void set_address_str(const std::string &address_str)
	{
		_address_str = address_str;
	}
	void set_label(const std::string &label)
	{
		_label = label;
	}

private:
	bool _init;

public:
	address() : _address_id(0), _address_str(""), _label(""),
				_init(false)
	{}

	address& operator=(SQLite::Statement& query);
	void init();
};

class config
{
private:
	int _config_id;
	int _program_version;
	int _db_version;

public:
	int get_config_id() const
	{
		return _config_id;
	}
	int get_program_version() const
	{
		return _program_version;
	}
	int get_db_version() const
	{
		return _db_version;
	}

	void set_config_id(int config_id)
	{
		_config_id = config_id;
	}
	void set_program_version(int program_version)
	{
		_program_version = program_version;
	}
	void set_db_version(int db_version)
	{
		_db_version = db_version;
	}

private:
	bool _init;

public:
	config() : _config_id(0), _program_version(0), _db_version(0),
				_init(false)
	{}

	config& operator=(SQLite::Statement& query);
	void init();
};


#endif /* BITLINK_MODEL_HPP */
