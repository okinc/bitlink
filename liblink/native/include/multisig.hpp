/*
 * multisig.hpp
 *
 *  Created on: 2015年4月7日
 *      Author: Administrator
 */

#ifndef BITLINK_MULTISIG_HPP
#define BITLINK_MULTISIG_HPP

#include <vector>
#include <string>

#include "utility/assert.hpp"
#include "script.hpp"
#include "address.hpp"


inline operation create_data_operation(data_chunk& data)
{
    BITCOIN_ASSERT(data.size() < std::numeric_limits<uint32_t>::max());
    operation op;
    op.data = data;
    if (data.size() <= 75)
        op.code = opcode::special;
    else if (data.size() < std::numeric_limits<uint8_t>::max())
        op.code = opcode::pushdata1;
    else if (data.size() < std::numeric_limits<uint16_t>::max())
        op.code = opcode::pushdata2;
    else if (data.size() < std::numeric_limits<uint32_t>::max())
        op.code = opcode::pushdata4;
    return op;
}


script_type create_multisig(const int required, const std::vector<std::string> &public_keys);

payment_address multisig_to_address(const script_type &script);



#endif /* BITLINK_MULTISIG_HPP */
