/*
 * constants.hpp
 *
 *  Created on: 2015年1月5日
 *      Author: Administrator
 */

#ifndef BITLINK_CONSTANTS_HPP
#define BITLINK_CONSTANTS_HPP

#include <cstdint>

#include <boost/assign/list_of.hpp>

#include "utility/big_number.hpp"


const uint32_t protocol_version = 60000;

const uint64_t block_reward = 50;
// 210000 ~ 4 years / 10 minutes
const uint64_t reward_interval = 210000;

const size_t coinbase_maturity = 100;

#ifdef ENABLE_TESTNET
const uint32_t protocol_port = 18333;
#else
const uint32_t protocol_port = 8333;
#endif

// Threshold for nLockTime: below this value it is
// interpreted as block number, otherwise as UNIX timestamp.
// Tue Nov 5 00:53:20 1985 UTC
const uint32_t locktime_threshold = 500000000;

inline const uint64_t max_money_recursive(uint64_t current)
{
    return (current > 0) ?
        current + max_money_recursive(current >> 1) : 0;
}

inline const uint64_t coin_price(uint64_t value=1)
{
    return value * 100000000;
}

inline const uint64_t max_money()
{
    return reward_interval * max_money_recursive(coin_price(block_reward));
}

const hash_digest null_hash = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
const short_hash null_short_hash = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

const uint32_t max_bits = 0x1d00ffff;
big_number max_target();
uint32_t magic_value();
const uint32_t max_index = std::numeric_limits<uint32_t>::max();

// Every two weeks we readjust target
const uint64_t target_timespan = 14 * 24 * 60 * 60;
// Aim for blocks every 10 mins
const uint64_t target_spacing = 10 * 60;
// Two weeks worth of blocks = readjust interval = 2016
const uint64_t readjustment_interval = target_timespan / target_spacing;

#ifdef ENABLE_TESTNET
// Block 514 is the first block after Feb 15 2014.
// Testnet started bip16 before mainnet.
const uint32_t bip16_switchover_timestamp = 1333238400;
const uint32_t bip16_switchover_height = 514;
#else
// April 1 2012
const uint32_t bip16_switchover_timestamp = 1333238400;
const uint32_t bip16_switchover_height = 173805;
#endif


#endif /* BITLINK_CONSTANTS_HPP */
