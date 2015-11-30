LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
  milk.c

LOCAL_MODULE := almondmilk
LOCAL_MODULE_TAGS := optional

# Allow execution on android-16
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE

APP_ABI := armeabi armeabi-v7a x86

include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
