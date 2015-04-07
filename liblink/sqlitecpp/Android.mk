# Path of the sources
LOCAL_PATH := $(call my-dir)

SQLITE_PATH := $(LOCAL_PATH)/../sqlite3/

include $(CLEAR_VARS)

LOCAL_STATIC_LIBRARIES += libsqlite3

LOCAL_CFLAGS := -O0 -g

LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(SQLITE_PATH)


SQLITE_CPP_LIST := $(wildcard $(LOCAL_PATH)/src/*.cpp)

LOCAL_SRC_FILES := $(SQLITE_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_MODULE := sqlitecpp
#LOCAL_MODULE_FILENAME := libsqlite3
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
