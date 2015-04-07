/*
 * Copyright (c) 2011-2013 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) 
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "utility/base58.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include "utility/big_number.hpp"


// Thanks for all the wonderful bitcoin hackers

const char* base58_chars = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

std::string encode_base58(const data_chunk& unencoded_data)
{                                                                                
    std::string encoded_data;
    // Expected size increase from base58 conversion is approximately 137%
    // use 138% to be safe
    encoded_data.reserve((unencoded_data.size() - 1) * 138 / 100 + 1);

    // Convert big endian data to little endian
    // Extra zero at the end make sure bignum will interpret
    // as a positive number
    data_chunk tmp_data(unencoded_data.size() + 1, 0);
    std::reverse_copy(unencoded_data.begin(), unencoded_data.end(),
        tmp_data.begin());

    big_number long_value;
    long_value.set_data(tmp_data);
    while (long_value > 0)
    {                                                                            
    	divmod_result result = divmod(long_value, 58);
        long_value = result.first;
        size_t remainder = result.second.uint32();
        encoded_data += base58_chars[remainder];
    }                                                                            
                                                                                 
    // Leading zeroes encoded as base58 zeros                                    
    BOOST_FOREACH (const uint8_t unencoded_byte, unencoded_data)
    {
        if (unencoded_byte != 0)
            break;
        encoded_data += base58_chars[0];
    }

    // Convert little endian std::string to big endian
    std::reverse(encoded_data.begin(), encoded_data.end());
    return encoded_data;
}                                                                                

data_chunk decode_base58(std::string encoded_data)
{                                                                                
    // Trim spaces and newlines around the string.
    boost::algorithm::trim(encoded_data);
    // We're building a big number.
    big_number bn = 0;       
                                                                                 
    // Convert big endian string to bignum                                       
    BOOST_FOREACH (const uint8_t current_char, encoded_data)
    {                                                                            
        bn *= 58;
        bn += std::string(base58_chars).find(current_char);
    }                                                                            
                                                                                 
    // Get bignum as little endian data                                          
    data_chunk temp_data = bn.data();       
                                                                                 
    // Trim off sign byte if present                                             
    if (temp_data.size() >= 2 &&
            temp_data.end()[-1] == 0 && temp_data.end()[-2] >= 0x80)
        temp_data.erase(temp_data.end() - 1);
                                                                                 
    // Restore leading zeros                                                     
    int leading_zeros = 0;
    BOOST_FOREACH (const uint8_t current_char, encoded_data)
    {
        if (current_char != base58_chars[0])
            break;
        leading_zeros++;
    }

    data_chunk unencoded_data;
    unencoded_data.assign(leading_zeros + temp_data.size(), 0);
    // Convert little endian data to big endian                                  
    std::reverse_copy(temp_data.begin(), temp_data.end(),
        unencoded_data.end() - temp_data.size());    
    return unencoded_data;
}                                                             


