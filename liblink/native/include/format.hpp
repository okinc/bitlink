#ifndef BITLINK_FORMAT_HPP
#define BITLINK_FORMAT_HPP

#include <iomanip>
#include <algorithm>

#include <boost/foreach.hpp>
#include <boost/range.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/detail/endian.hpp>

#include "constants.hpp"
#include "types.hpp"
#include "utility/assert.hpp"


// Forwarding definitions because primitives.hpp depends on this header.
struct output_point;
// We don't need a stream operator for input_point:
typedef output_point input_point;

template<typename D, typename T>
void extend_data(D& chunk, const T& other)
{
    chunk.insert(boost::end(chunk), boost::begin(other), boost::end(other));
}

template<typename T>
T cast_chunk(data_chunk chunk, bool reverse=false)
{
    #ifdef BOOST_LITTLE_ENDIAN
        // do nothing
    #elif BOOST_BIG_ENDIAN
        reverse = !reverse;
    #else
        #error "Endian isn't defined!"
    #endif

    if (reverse)
    	std::reverse(boost::begin(chunk), boost::end(chunk));

    T value = 0;
    for (size_t i = 0; i < sizeof(T) && i < chunk.size(); ++i)
        value += static_cast<T>(chunk[i]) << (i * 8);
    return value;
}

template<typename T>
data_chunk uncast_type(T value, bool reverse=false)
{
    // TODO Future versions of boost will have boost::native_to_little(value);
    #ifdef BOOST_LITTLE_ENDIAN
        // do nothing
    #elif BOOST_BIG_ENDIAN
        reverse = !reverse;
    #else
        #error "Endian isn't defined!"
    #endif

    data_chunk chunk(sizeof(T));
    uint8_t* value_begin = reinterpret_cast<uint8_t*>(&value);
    std::copy(value_begin, value_begin + sizeof(T), chunk.begin());

    if (reverse)
        std::reverse(boost::begin(chunk), boost::end(chunk));
    return chunk;
}

template<typename T>
std::string encode_hex(T data)
{
    std::stringstream ss;
    ss << std::hex;
    BOOST_FOREACH (int val, data)
        ss << std::setw(2) << std::setfill('0') << val;
    return ss.str();
}

// ADL cannot work on templates
std::ostream& operator<<(std::ostream& stream, const data_chunk& data);
std::ostream& operator<<(std::ostream& stream, const hash_digest& hash);
std::ostream& operator<<(std::ostream& stream, const short_hash& hash);
std::ostream& operator<<(std::ostream& stream, const output_point& point);

data_chunk decode_hex(std::string hex_str);

/**
 * Turns a hash hex string into HashType.
 * byte_stream.size() == 2 * HashType.size()
 *
 * On error, returns null_hash for hash_digest, or
 * null_short_hash for short_hash.
 */
template <typename HashType>
HashType decode_hex_digest(std::string hex_str)
{
    data_chunk raw_bytes = decode_hex(hex_str);
    HashType result;
    if (raw_bytes.size() != result.size())
    {
        // null_hash for hash_digest
        // null_short_hash for short_hash
        result.fill(0);
        return result;
    }
    BITCOIN_ASSERT(raw_bytes.size() == result.size());
    std::copy(raw_bytes.begin(), raw_bytes.end(), result.begin());
    return result;
}

std::string satoshi_to_btc(uint64_t value);

// Python like range
//   for (char c: range(str, 0, 2))
template <typename RangeType>
boost::sub_range<RangeType> range(RangeType iterable,
    size_t start_offset=0, size_t end_offset=0)
{
    return boost::sub_range<RangeType>(
        iterable.begin() + start_offset, iterable.end() - end_offset);
}



#endif /* BITLINK_FORMAT_HPP */
