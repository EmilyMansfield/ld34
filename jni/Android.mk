LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := squares

LOCAL_CPP_FLAGS := -std=c++11

LOCAL_SRC_FILES := ../constants.cpp
LOCAL_SRC_FILES += ../game_state_game.cpp
LOCAL_SRC_FILES += ../game_state_scores.cpp
LOCAL_SRC_FILES += ../game_state_settings.cpp
LOCAL_SRC_FILES += ../game_state_title.cpp
LOCAL_SRC_FILES += ../main.cpp
LOCAL_SRC_FILES += ../player.cpp
LOCAL_SRC_FILES += ../util.cpp

LOCAL_SHARED_LIBRARIES := sfml-system
LOCAL_SHARED_LIBRARIES += sfml-window
LOCAL_SHARED_LIBRARIES += sfml-graphics
LOCAL_SHARED_LIBRARIES += sfml-audio
LOCAL_SHARED_LIBRARIES += sfml-network
LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main

include $(BUILD_SHARED_LIBRARY)

$(call import-module,sfml)
