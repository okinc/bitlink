/*
 * init.hpp
 *
 *  Created on: 2015年3月11日
 *      Author: Administrator
 */

#ifndef BITLINK_INIT_HPP
#define BITLINK_INIT_HPP

#include <cstddef>

class input_manager;
class output_manager;
class tx_manager;
class coin_manager;
class key_manger;
class account_manager;

extern input_manager *pinput_manager;
extern output_manager *poutput_manager;
extern tx_manager *ptx_manager;
extern coin_manager *pcoin_manager;
extern key_manger *pkey_manger;
extern account_manager *paccount_manager;


bool init();


#endif /* BITLINK_INIT_HPP */
