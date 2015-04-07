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
#ifndef BITLINK_SHA256_HPP
#define BITLINK_SHA256_HPP

#include <openssl/sha.h>
#include <cstdint>

#include "types.hpp"

const size_t sha256_digest_size = SHA256_DIGEST_LENGTH;
// static_assert(sha256_digest_size == 32, "SHA256_DIGEST_LENGTH not defined correctly!");

hash_digest generate_sha256_hash(const data_chunk& data);
uint32_t generate_sha256_checksum(const data_chunk& data);


#endif /* BITLINK_SHA256_HPP */

