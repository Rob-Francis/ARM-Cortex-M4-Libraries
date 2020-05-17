###################################################
# LIBRARY SOURCES
###################################################

LTA1000G_PATH=/home/csse3010/csse3010/mylib/lta1000g

CFLAGS += -I$(LTA1000G_PATH)

LIBSRCS += $(LTA1000G_PATH)/s4532390_hal_lta1000g.c
LIBSRCS += $(LTA1000G_PATH)/s4532390_os_lta1000g.c

