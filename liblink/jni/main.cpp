#include <okcoin_oklink_liblink_MyClass.h>

#include <stdexcept>

#include <openssl/rand.h>

#include <limits>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "SQLiteCpp/SQLiteCpp.h"

#include "format.hpp"
#include "ec_key.hpp"
#include "model.hpp"
#include "db.hpp"
#include "key.hpp"
#include "multisig.hpp"
#include "init.hpp"

#define LL <<

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL JNI_OnLoad( JavaVM *vm, void *pvt )
{
	if(!init())
	{
		throw std::runtime_error("init fail!");
	}
	return JNI_VERSION_1_2;
}

/*
 * Class:     okcoin_oklink_liblink_MyClass
 * Method:    getStringFromNative
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_okcoin_oklink_liblink_MyClass_getStringFromNative
  (JNIEnv *env, jobject obj)
{

    long long nRand = 0;
    RAND_bytes((unsigned char*)&nRand, sizeof(nRand));

    std::stringstream ss;
    std::string str;

    ss << nRand;
    str = "F*ck: " + ss.str();

    uint64_t major = 1;
    std::string result = boost::lexical_cast<std::string>(major);

    boost::shared_ptr<std::string> shared_str(new std::string("test: "+str+result));

//    for(int i = 0; i < 1000; i++)
//    {
//    	key_ = pkey_manger->create();
//    }

    key &key1 = pkey_manger->create();
    key &key2 = pkey_manger->create();

    std::vector<std::string> public_keys;
    public_keys.push_back(encode_hex(key1.get_ec_key_public()));
    public_keys.push_back(encode_hex(key2.get_ec_key_public()));

    script_type script = create_multisig(2, public_keys);
    payment_address address = multisig_to_address(script);

    //return env->NewStringUTF(shared_str->c_str());

    return env->NewStringUTF(address.encoded().c_str());
}

#ifdef __cplusplus
}
#endif
