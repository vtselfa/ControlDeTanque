# XAL_PATH: path to the XTRATUM directory
XAL_PATH=/opt/xm-sdk/xal

# XMLCF: path to the XML configuration file
XMLCF=xm_cf.ia32.xml

# PARTITIONS: partition files (xef format) composing the example
PARTITIONS=partition1.xef

# LINUXTOOLS_PATH: path to the LINUX TOOLS
LINUXTOOLS_PATH=$(XTRATUM_PATH)/../linux

all: $(PARTITIONS)
include $(XAL_PATH)/common/rules.mk

linuxloader:
	@make -C $(LINUXTOOLS_PATH)/loader clean 
	@make -C $(LINUXTOOLS_PATH)/loader linuxloader
	@cp $(LINUXTOOLS_PATH)/loader/linuxloader linuxloader

partition1.xef: linuxloader
	@$(XEF) linuxloader -o $@ -i 0

