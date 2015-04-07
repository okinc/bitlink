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
#ifndef BITLINK_SATOSHI_SERIALIZE_HPP
#define BITLINK_SATOSHI_SERIALIZE_HPP

#include "constants.hpp"
#include "format.hpp"
#include "primitives.hpp"
#include "transaction.hpp"
#include "utility/assert.hpp"
#include "utility/serializer.hpp"
#include "utility/sha256.hpp"


const size_t command_size = 12;

// message headers
size_t satoshi_raw_size(const header_type& head);
template <typename Iterator>
Iterator satoshi_save(const header_type& head, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    header_type& head);

// version messages
const std::string satoshi_command(const version_type&);
size_t satoshi_raw_size(const version_type& packet);
template <typename Iterator>
Iterator satoshi_save(const version_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    version_type& packet);

// verack messages
const std::string satoshi_command(const verack_type&);
size_t satoshi_raw_size(const verack_type& packet);
template <typename Iterator>
Iterator satoshi_save(const verack_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    verack_type& packet);

// addr messages
const std::string satoshi_command(const address_type&);
size_t satoshi_raw_size(const address_type& packet);
template <typename Iterator>
Iterator satoshi_save(const address_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    address_type& packet);

// getaddr messages
const std::string satoshi_command(const get_address_type&);
size_t satoshi_raw_size(const get_address_type& packet);
template <typename Iterator>
Iterator satoshi_save(const get_address_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    get_address_type& packet);

// inv messages
const std::string satoshi_command(const inventory_type&);
size_t satoshi_raw_size(const inventory_type& packet);
template <typename Iterator>
Iterator satoshi_save(const inventory_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    inventory_type& packet);

// getdata messages
const std::string satoshi_command(const get_data_type&);
size_t satoshi_raw_size(const get_data_type& packet);
template <typename Iterator>
Iterator satoshi_save(const get_data_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    get_data_type& packet);

// getblocks messages
const std::string satoshi_command(const get_blocks_type&);
size_t satoshi_raw_size(const get_blocks_type& packet);
template <typename Iterator>
Iterator satoshi_save(const get_blocks_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    get_blocks_type& packet);

// tx messages
const std::string satoshi_command(const transaction_type&);
size_t satoshi_raw_size(const transaction_type& packet);
template <typename Iterator>
Iterator satoshi_save(const transaction_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    transaction_type& packet);

// block header
size_t satoshi_raw_size(const block_header_type& packet);
template <typename Iterator>
Iterator satoshi_save(const block_header_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    block_header_type& packet);

// block messages
const std::string satoshi_command(const block_type&);
size_t satoshi_raw_size(const block_type& packet);
template <typename Iterator>
Iterator satoshi_save(const block_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    block_type& packet);

// ping messages
const std::string satoshi_command(const ping_type&);
size_t satoshi_raw_size(const ping_type& packet);
template <typename Iterator>
Iterator satoshi_save(const ping_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    ping_type& packet);

// pong messages
const std::string satoshi_command(const pong_type&);
size_t satoshi_raw_size(const pong_type& packet);
template <typename Iterator>
Iterator satoshi_save(const pong_type& packet, Iterator result);
template <typename Iterator>
void satoshi_load(const Iterator first, const Iterator last,
    pong_type& packet);


#include "impl/serialize/misc.ipp"
#include "impl/serialize/block.ipp"

#endif /* BITLINK_SATOSHI_SERIALIZE_HPP */

