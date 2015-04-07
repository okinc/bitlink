/*
 * wallet.hpp
 *
 *  Created on: 2015年3月13日
 *      Author: Administrator
 */

#ifndef BITLINK_WALLET_HPP
#define BITLINK_WALLET_HPP


struct transaction_type;
class elliptic_curve_key;
class script_type;
class payment_address;


script_type build_pubkey_hash_script(const short_hash& pubkey_hash);
script_type build_script_hash_script(const short_hash& script_hash);

bool build_output_script(
    script_type& out_script, const payment_address& payaddr);

bool make_signature(transaction_type& tx, size_t input_index,
    const elliptic_curve_key& key, const script_type& script_code);


#endif /* BITLINK_WALLET_HPP */
