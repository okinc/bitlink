#ifndef BITLINK_KEY_HPP
#define BITLINK_KEY_HPP

#include <string>
//#include <tr1/unordered_map>
#include <map>

//using std::tr1::unordered_map;

#include "db.hpp"


class key;

class key_manger
{
public:
	void load(Db& db);
	bool import(const std::string &priv_key_);
	bool remove(key &key_);
	bool add_received(key &key_, int received, Db &db);
	bool add_sent(key &key_, int sent, Db &db);
	key &get(const std::string& address);
	bool get(const std::string& address, key &key_);
	key &create();

private:
	std::map<std::string, key> _key_map;

public:
	key_manger() : _key_map()
	{}
};

#endif /* BITLINK_KEY_HPP */
