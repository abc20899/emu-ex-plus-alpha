ifndef inc_resource_font_android
inc_resource_font_android := 1

include $(IMAGINE_PATH)/src/font/build.mk

configDefs += CONFIG_RESOURCE_FONT_ANDROID

LDLIBS += -ljnigraphics

SRC += font/AndroidFont.cc

endif
