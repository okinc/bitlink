/*
 * assert.hpp
 *
 *  Created on: 2015年1月5日
 *      Author: Administrator
 */

#ifndef BITLINK_ASSERT_HPP
#define BITLINK_ASSERT_HPP

#include <boost/assert.hpp>

#ifdef BITCOIN_DISABLE_ASSERTS
    #define BOOST_DISABLE_ASSERTS
#endif

#define BITCOIN_ASSERT BOOST_ASSERT
#define BITCOIN_ASSERT_MSG BOOST_ASSERT_MSG



#endif /* BITLINK_ASSERT_HPP */
