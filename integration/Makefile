# XAL_PATH: Paths of the XtratuM distribution
XAL_PATH=/opt/xm-sdk/xal

# XMLCF: path to the XML configuration file
XMLCF=xm_cf.ia32.xml

# PARTITIONS: partition files (xef format) to integrate
PARTITIONS=partition1.xef partition2.xef partition3.xef partition4.xef

# CONTAINTER: container of the xtratum core, the partitions and the xm_cf configuration
CONTAINER=container.bin

# LINUXTOOLS_PATH: path to the LINUX TOOLS
LINUXTOOLS_PATH=$(XTRATUM_PATH)/../linux

all: resident_sw
include $(XAL_PATH)/common/rules.mk

INITRDFILES=\
	$(LINUXTOOLS_PATH)/bin/xmmanager\
	$(call checkfile,$(LINUX_PATH)/arch/x86/xm/usr/virtdisk)\

initrd.img.gz: # $(INITRDFILES)
	@$(LINUXTOOLS_PATH)/bin/mkinitramfs init initrd.img tmproot
	@cp -f $(INITRDFILES) tmproot/bin
	@$(LINUXTOOLS_PATH)/bin/mkinitramfs build initrd.img tmproot

PACK_ARGS=-h $(XMCORE_BIN):xm_cf.bin.xmc \
	-b partition1.xef:cmdline.txt:initrd.img.gz \
	-b partition2.xef:p1.cfg\
	-b partition3.xef:p1.cfg\
	-b partition4.xef:p1.cfg\

$(CONTAINER): $(PARTITIONS) xm_cf.bin.xmc initrd.img.gz
	$(XMPACK) build $(PACK_ARGS) $@

.PHONY: initrd.img.gz
