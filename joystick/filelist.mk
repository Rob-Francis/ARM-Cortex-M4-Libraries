###################################################
# LIBRARY SOURCES
###################################################

JOYSTICK_PATH=/home/csse3010/csse3010/mylib/joystick

CFLAGS += -I$(JOYSTICK_PATH)

LIBSRCS += $(JOYSTICK_PATH)/s4532390_hal_joystick.c
LIBSRCS += $(JOYSTICK_PATH)/s4532390_os_joystick.c
LIBSRCS += $(JOYSTICK_PATH)/s4532390_CAG_joystick.c

