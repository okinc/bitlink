# Path of the sources
JNI_DIR := $(call my-dir)

#optional arguments
#WITH_POLAR=1
#WITH_OPENVPN3=1
# Build openvpn with polar (OpenVPN3 core is always build with polar)
#WITH_BREAKPAD=0


#include lzo/Android.mk
#include snappy/Android.mk

include native/Android.mk
#include openssl/Android.mk


LOCAL_PATH := $(JNI_DIR)
BOOST_PATH := $(JNI_DIR)/../Boost-for-Android/build/
SQLITE_PATH := $(JNI_DIR)/../sqlite3/
SQLITE_CPP_PATH := $(JNI_DIR)/../sqlitecpp/include/

# The only real JNI library
include $(CLEAR_VARS)
LOCAL_LDLIBS := -llog  -lz
#LOCAL_CFLAGS := --std=c99
#LOCAL_CPPFLAGS += -std=c++11

LOCAL_CFLAGS := -O0 -g
LOCAL_STATIC_LIBRARIES := -L$(BOOST_PATH)/lib/ -lboost_system
LOCAL_STATIC_LIBRARIES += libcrypto_static
LOCAL_STATIC_LIBRARIES += libssl_static

LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti

LOCAL_SHARED_LIBRARIES := bitlink


LOCAL_C_INCLUDES := openssl/include openssl/crypto openssl 
LOCAL_C_INCLUDES += native/include
LOCAL_C_INCLUDES += $(BOOST_PATH)/include/boost-1_53/
LOCAL_C_INCLUDES += $(SQLITE_PATH)
LOCAL_C_INCLUDES += $(SQLITE_CPP_PATH)

LOCAL_SRC_FILES:= main.cpp empty.c
LOCAL_MODULE = link

include $(BUILD_SHARED_LIBRARY)


