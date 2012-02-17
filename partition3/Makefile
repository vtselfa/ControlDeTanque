# XAL_PATH: Paths of the XtratuM distribution
XAL_PATH=/opt/xm-sdk/xal

# XMLCF: path to the XML configuration file
XMLCF=xm_cf.ia32.xml

# PARTITION: partition identifier name
PARTITION=partition3

all: $(PARTITION).xef
include $(XAL_PATH)/common/rules.mk

SRCS = $(sort $(wildcard src/*.c))
OBJS = $(patsubst %.c, %.o, $(SRCS))

$(PARTITION).o: $(OBJS)
	@$(LD) -o $@ $^ $(LDFLAGS) -Ttext=$(call xpathstart,3,$(XMLCF))
$(PARTITION).xef: $(PARTITION).o
	@$(XEF) $^ -o $@ -i 2
%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $^
