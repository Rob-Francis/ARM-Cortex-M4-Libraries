###################################################
# LIBRARY SOURCES
###################################################

PROJECT_PATH=$(HOME)/csse3010/mylib/project

CFLAGS += -I$(PROJECT_PATH)

LIBSRCS += $(PROJECT_PATH)/s4532390_CAG_Display.c

LIBSRCS += $(PROJECT_PATH)/s4532390_CAG_Keypad_Grid.c

LIBSRCS += $(PROJECT_PATH)/s4532390_CAG_Keypad_Mnemonic.c

LIBSRCS += $(PROJECT_PATH)/s4532390_CAG_Simulator.c

LIBSRCS += $(PROJECT_PATH)/s4532390_CAG_PushButton.c




