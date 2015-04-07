#ifndef BITLINK_TYPES_HPP
#define BITLINK_TYPES_HPP

#include <sstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/functional/hash.hpp>
#include <boost/range.hpp>


#define DECLARE_PTR_TYPE(nametype) \
    class nametype; \
    typedef boost::shared_ptr<nametype> nametype##_ptr

DECLARE_PTR_TYPE(acceptor);
DECLARE_PTR_TYPE(channel);

#undef DECLARE_PTR_TYPE


const size_t hash_digest_size = 32;
const size_t short_hash_size = 20;
typedef boost::array<uint8_t, hash_digest_size> hash_digest;
typedef boost::array<uint8_t, short_hash_size> short_hash;

typedef std::vector<uint8_t> data_chunk;
typedef std::vector<data_chunk> data_stack;

// A list of indices. Used for creating block_locator objects or
// Storing list of unconfirmed input indexes in tx pool.
typedef std::vector<size_t> index_list;

// List of hashes. Useful primitive.
typedef std::vector<hash_digest> hash_digest_list;
typedef std::vector<short_hash> short_hash_list;

// Make hash_digest and short_hash hashable for std::*map variants
template <typename HashType>
struct std_hash_wrapper
{
    size_t operator()(const HashType& h) const
    {
        boost::hash<std::string> functor;
        return functor(std::string(boost::begin(h), boost::end(h)));
    }
};

struct node_address
{
    std::string hostname;
    uint16_t port;
};


// Extend std namespace with our hash wrappers
namespace boost
{
    template <>
    struct hash<hash_digest>
      : public std_hash_wrapper<hash_digest>
    {
    };

    template <>
    struct hash<short_hash>
      : public std_hash_wrapper<short_hash>
    {
    };
} // namespace boost

#endif /* BITLINK_TYPES_HPP */

