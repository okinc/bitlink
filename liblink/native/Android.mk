# Path of the sources
NATIVE_DIR := $(call my-dir)

#optional arguments
#WITH_POLAR=1
#WITH_OPENVPN3=1
# Build openvpn with polar (OpenVPN3 core is always build with polar)
#WITH_BREAKPAD=0

#NDK_TOOLCHAIN_VERSION := 4.8


#include lzo/Android.mk
#include snappy/Android.mk

include openssl/Android.mk
include sqlite3/Android.mk
include sqlitecpp/Android.mk


LOCAL_PATH := $(NATIVE_DIR)
BOOST_PATH := $(NATIVE_DIR)/../Boost-for-Android/build/
SQLITE_PATH := $(NATIVE_DIR)/../sqlite3/
SQLITE_CPP_PATH := $(NATIVE_DIR)/../sqlitecpp/include/

# The only real JNI library
include $(CLEAR_VARS)
LOCAL_LDLIBS := -llog  -lz
#LOCAL_CFLAGS = --std=c99
#LOCAL_CPPFLAGS := -std=c++11
#LOCAL_CPPFLAGS := -std=c++0x

LOCAL_CFLAGS := -O0 -g
LOCAL_CFLAGS += -I$(BOOST_PATH)/include/boost-1_53/
LOCAL_STATIC_LIBRARIES += -L$(BOOST_PATH)/lib/ -lboost_system
LOCAL_STATIC_LIBRARIES += libsqlite3
LOCAL_STATIC_LIBRARIES += libsqlitecpp
LOCAL_STATIC_LIBRARIES += libcrypto_static
LOCAL_STATIC_LIBRARIES += libssl_static

LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti

LOCAL_C_INCLUDES := openssl/include openssl/crypto openssl
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(SQLITE_PATH)
LOCAL_C_INCLUDES += $(SQLITE_CPP_PATH)


NATIVE_CPP_LIST := $(wildcard $(NATIVE_DIR)/src/*.cpp)
NATIVE_CPP_LIST += $(wildcard $(NATIVE_DIR)/src/utility/*.cpp)

LOCAL_SRC_FILES := $(NATIVE_CPP_LIST:$(NATIVE_DIR)/%=%)


LOCAL_MODULE = bitlink
#LOCAL_SHARED_LIBRARIES :=  libcrypto
include $(BUILD_SHARED_LIBRARY)


