###################################################
# LIBRARY SOURCES
###################################################

PROJECT_PATH=$(HOME)/csse3010/mylib/project

CFLAGS += -I$(PROJECT_PATH)

LIBSRCS += $(PROJECT_PATH)/s4532390_cag_display.c

LIBSRCS += $(PROJECT_PATH)/s4532390_cag_keypad_grid.c

LIBSRCS += $(PROJECT_PATH)/s4532390_cag_keypad_mnemonic.c

LIBSRCS += $(PROJECT_PATH)/s4532390_cag_simulator.c

LIBSRCS += $(PROJECT_PATH)/s4532390_cag_pushbutton.c

LIBSRCS += $(PROJECT_PATH)/s4532390_cag_joystick.c




