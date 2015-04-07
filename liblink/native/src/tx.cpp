/*
 * tx.cpp
 *
 *  Created on: 2015年3月4日
 *      Author: Administrator
 */

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

#include "SQLiteCpp/SQLiteCpp.h"

#include "ec_key.hpp"
#include "primitives.hpp"
#include "tx.hpp"
#include "key.hpp"
#include "coin.hpp"
#include "account.hpp"
#include "satoshi_serialize.hpp"
#include "address.hpp"
#include "wallet.hpp"
#include "init.hpp"


void input_manager::load(Db& db)
{
	SQLite::Statement *query = db.load_inputs();

	while (query->executeStep())
	{
		input _input;
		_input = *query;
		_input.init();

		std::vector<input> inputs = _input_map[_input.get_to_tx()];
		if(inputs.size() <= _input.get_to_index())
		{
			inputs.resize(_input.get_to_index() + 1);
		}

		inputs[_input.get_to_index()] = _input;
	}

	delete query;
}

std::vector<input> &input_manager::get(const std::string &tx_hash)
{
	return _input_map[tx_hash];
}

void output_manager::load(Db& db)
{
	SQLite::Statement *query = db.load_outputs();

	while (query->executeStep())
	{
		output _output;
		_output = *query;
		_output.init();

		std::vector<output> outputs = _output_map[_output.get_to_tx()];
		if(outputs.size() <= _output.get_to_index())
		{
			outputs.resize(_output.get_to_index() + 1);
		}

		outputs[_output.get_to_index()] = _output;
	}

	delete query;
}

std::vector<output> &output_manager::get(const std::string &tx_hash)
{
	return _output_map[tx_hash];
}

bool output_manager::mark_used(output& output_, Db& db)
{
	bool ret = db.mark_used(output_, false);
	if(!ret)
	{
		return ret;
	}

	output_.set_spent(1);
	return ret;
}

void tx_manager::load(Db& db)
{
	SQLite::Statement *query = db.load_txes();

	while (query->executeStep())
	{
		tx _tx;
		_tx = *query;
		_tx.init();

		_tx_map[_tx.get_tx_hash()] = _tx;
	}

	delete query;
}

tx &tx_manager::get(const std::string &tx_hash)
{
	return _tx_map[tx_hash];
}

bool tx_manager::import(const std::string tx_hex)
{
	transaction_type transaction;
	satoshi_load(tx_hex.begin(), tx_hex.end(), transaction);

	hash_digest hash = hash_transaction(transaction);

	std::string hash_str(hash.begin(), hash.end());

	tx tx_;
	tx_.set_tx_hex(tx_hex);
	tx_.set_tx_hash(hash_str);

	std::vector<input> inputs(transaction.inputs.size());
	std::vector<output> outputs(transaction.outputs.size());
	std::vector<coin> coins(transaction.outputs.size());

	for(int i = 0; i < transaction.inputs.size(); i++)
	{
		const transaction_input_type &transaction_input_ = transaction.inputs[i];
		const output_point &output_point_ = transaction_input_.previous_output;
		input input_;
		input_.set_from_tx(hash_str);
		input_.set_from_index(output_point_.index);
		input_.set_from_amount(0);
		input_.set_from_address("");
		input_.set_to_tx(hash_str);
		input_.set_to_index(i);
		inputs.push_back(input_);
	}

	for(int i = 0; i < transaction.outputs.size(); i++)
	{
		const transaction_output_type &transaction_output_ = transaction.outputs[i];
		output output_;
		output_.set_to_tx(hash_str);
		output_.set_to_index(i);
		output_.set_to_amount(transaction_output_.value);
		output_.set_spent(0);

		payment_address payment_address_;
		if(extract(payment_address_, transaction_output_.script))
		{
			output_.set_to_address(payment_address_.encoded());
		}

		coin coin_;
		coin_.set_tx_hash(hash_str);
		coin_.set_to_index(i);
		coin_.set_owner_address(output_.get_to_address());
		coin_.set_amount(output_.get_to_amount());
	}

	Db db;
	db.startTransaction();

	db.write_tx(tx_, false);
	const int tx_id = tx_.get_tx_id();

	for(int i = 0; i , inputs.size(); i++)
	{
		input& input_ = inputs[i];
		input_.set_tx_id(tx_id);
		db.write_input(input_, false);

		coin coin_;
		if(pcoin_manager->get(input_.get_from_tx(), input_.get_from_index(), coin_))
		{
			key key_;
			if(pkey_manger->get(coin_.get_owner_address(), key_))
			{
				pkey_manger->add_sent(key_, coin_.get_amount(), db);

				paccount_manager->add_sent(coin_.get_amount(), db);
			}

			output output_ = poutput_manager->get(hash_str)[input_.get_from_index()];
			poutput_manager->mark_used(output_, db);

			pcoin_manager->remove(coin_, db);
		}
	}
	for(int i = 0; i < outputs.size(); i++)
	{
		output& output_ = outputs[i];
		output_.set_tx_id(tx_id);
		db.write_output(output_, false);

		coin& coin_ = coins[i];
		coin_.set_tx_id(tx_id);

		key key_;
		if(pkey_manger->get(output_.get_to_address(), key_))
		{
			pkey_manger->add_received(key_, coin_.get_amount(), db);

			paccount_manager->add_received(output_.get_to_amount(), db);
		}
	}

	db.commitTransaction();

	_tx_map[tx_.get_tx_hash()] = tx_;
	pinput_manager->_input_map[tx_.get_tx_hash()] = inputs;
	poutput_manager->_output_map[tx_.get_tx_hash()] = outputs;
	pcoin_manager->add(coins);

	return true;
}

const transaction_type tx_manager::create(const std::string &to_address, const int amount)
{
	std::vector<coin> coins = pcoin_manager->find(amount);
	transaction_type target;

	int total_amount = 0;

	BOOST_FOREACH(const coin &coin_, coins)
	{
		hash_digest hash_digest_;
		if(coin_.get_tx_hash().size() != hash_digest_.size())
		{
			throw std::runtime_error("tx hash size wrong");
		}
		for(int i = 0; i < hash_digest_.size(); i++)
		{
			hash_digest_[i] = coin_.get_tx_hash()[i];
		}

		transaction_input_type transaction_input;
		transaction_input.previous_output.hash = hash_digest_;
		transaction_input.previous_output.index = coin_.get_to_index();
		transaction_input.sequence = 4294967295;

		target.inputs.push_back(transaction_input);

		total_amount += coin_.get_amount();
	}

	transaction_output_type output_target;
	output_target.value = amount;
	payment_address payment_address_target(to_address);
	bool target_script_success = build_output_script(output_target.script, payment_address_target);
	if(!target_script_success)
	{
		throw std::runtime_error("build output script fail");
	}

	key &change_key = pkey_manger->create();

	transaction_output_type output_change;
	output_change.value = total_amount - amount;
	payment_address payment_address_change(change_key.get_ec_key_address());
	bool change_script_success = build_output_script(output_change.script, payment_address_change);
	if(!change_script_success)
	{
		throw std::runtime_error("build change script fail");
	}

	target.outputs.push_back(output_target);
	target.outputs.push_back(output_change);

    // now sign inputs
    for (size_t i = 0; i < target.inputs.size(); ++i)
    {
        transaction_input_type& input_ = target.inputs[i];
        const coin &coin_ = coins[i];
        const std::string addr = coin_.get_owner_address();
        key key_ = pkey_manger->get(addr);
        const elliptic_curve_key &ec_key_ = key_._ec_key;
        payment_address pub_address;
        set_public_key(pub_address, ec_key_.public_key());
        script_type prevout_script_code;
        bool prevout_script_code_success =
            build_output_script(prevout_script_code, pub_address);
        BITCOIN_ASSERT(prevout_script_code_success);
        bool sign_success = make_signature(target, i, ec_key_, prevout_script_code);

        if(!sign_success)
        {
        	throw std::runtime_error("sign fail");
        }
    }

    return target;
//  data_chunk serialized_tx(satoshi_raw_size(target));
//  satoshi_save(target, serialized_tx.begin());
//	return std::string(serialized_tx.begin(), serialized_tx.end());
}

void tx_manager::post_tx(const transaction_type& transaction)
{
	data_chunk serialized_tx(satoshi_raw_size(transaction));
	satoshi_save(transaction, serialized_tx.begin());
	std::string tx_hex = std::string(serialized_tx.begin(), serialized_tx.end());
	if(!import(tx_hex))
	{
		throw std::runtime_error("import post tx fail");
	}
}
