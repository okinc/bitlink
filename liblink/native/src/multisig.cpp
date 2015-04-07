/*
 * multisig.cpp
 *
 *  Created on: 2015年4月7日
 *      Author: Administrator
 */


#include <boost/foreach.hpp>

#include "multisig.hpp"
#include "format.hpp"
#include "address.hpp"


script_type create_multisig(const int required, const std::vector<std::string> &public_keys)
{
	script_type script;

	data_chunk data_required;
	data_required.push_back(required);
	operation op_required = create_data_operation(data_required);
	script.push_operation(op_required);

	BOOST_FOREACH(const std::string &public_key, public_keys)
	{
		data_chunk data_public_key = decode_hex(public_key);
		operation op_public_key = create_data_operation(data_public_key);
		script.push_operation(op_public_key);
	}

	data_chunk data_size;
	data_size.push_back(public_keys.size());
	operation op_size = create_data_operation(data_size);
	script.push_operation(op_size);

	operation op_checkmultisig;
	op_checkmultisig.code = opcode::checkmultisig;
	script.push_operation(op_checkmultisig);

	return script;
}

payment_address multisig_to_address(const script_type &script)
{
	payment_address address;
	set_script(address, script);
	return address;
}
