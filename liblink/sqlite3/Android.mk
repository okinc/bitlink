# Path of the sources
LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_CFLAGS := --std=c99
LOCAL_CFLAGS += -O0 -g
LOCAL_CFLAGS := -DSQLITE_THREADSAFE=1

LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_SRC_FILES := sqlite3.c empty.c
LOCAL_MODULE := sqlite3
#LOCAL_MODULE_FILENAME := libsqlite3
include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_SHARED_LIBRARY)
