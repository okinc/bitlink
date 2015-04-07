/*
 * wallet.cpp
 *
 *  Created on: 2015年3月13日
 *      Author: Administrator
 */


#include "constants.hpp"
#include "primitives.hpp"
#include "types.hpp"
#include "ec_key.hpp"
#include "transaction.hpp"
#include "script.hpp"
#include "address.hpp"
#include "wallet.hpp"


bool make_signature(transaction_type& tx, size_t input_index,
    const elliptic_curve_key& key, const script_type& script_code)
{
    transaction_input_type& input = tx.inputs[input_index];

    const data_chunk public_key = key.public_key();
    if (public_key.empty())
        return false;
    hash_digest tx_hash =
        script_type::generate_signature_hash(tx, input_index, script_code, 1);
    if (tx_hash == null_hash)
        return false;
    data_chunk signature = key.sign(tx_hash);
    signature.push_back(0x01);
    //std::cout << signature << std::endl;
    script_type& script = tx.inputs[input_index].script;
    // signature
    script.push_operation({opcode::special, signature});
    // public key
    script.push_operation({opcode::special, public_key});
    return true;
}

script_type build_pubkey_hash_script(const short_hash& pubkey_hash)
{
    script_type result;
    result.push_operation({opcode::dup, data_chunk()});
    result.push_operation({opcode::hash160, data_chunk()});
    result.push_operation({opcode::special,
        data_chunk(pubkey_hash.begin(), pubkey_hash.end())});
    result.push_operation({opcode::equalverify, data_chunk()});
    result.push_operation({opcode::checksig, data_chunk()});
    return result;
}

script_type build_script_hash_script(const short_hash& script_hash)
{
    script_type result;
    result.push_operation({opcode::hash160, data_chunk()});
    result.push_operation({opcode::special,
        data_chunk(script_hash.begin(), script_hash.end())});
    result.push_operation({opcode::equal, data_chunk()});
    return result;
}

bool build_output_script(
    script_type& out_script, const payment_address& payaddr)
{
    switch (payaddr.version())
    {
        case payment_address::pubkey_version:
            out_script = build_pubkey_hash_script(payaddr.hash());
            return true;

        case payment_address::script_version:
            out_script = build_script_hash_script(payaddr.hash());
            return true;
    }
    return false;
}

