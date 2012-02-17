# INTEGRATION: the path to the integration project directory
INTEGRATION=integration

# PARTITIONS: the paths to the partition project directories
PARTITIONS=partition1 partition2 partition3 partition4

MAKEFLAGS   += --no-print-directory --quiet
all:
	@for dir in $(PARTITIONS) ; do \
		$(MAKE) -C $$dir $(MAKECMDGOALS); \
		cp $$dir/*.xef ${INTEGRATION}; \
	done
	@$(MAKE) -C ${INTEGRATION} ${MAKECMDGOALS}
	cp ${INTEGRATION}/resident_sw .
	cp resident_sw /boot/xm/l1x3-funciona.xm
clean:
	@for dir in $(PARTITIONS) ; do \
		$(MAKE) -C $$dir $(MAKECMDGOALS); \
		$(RM) $$dir/*.xef; \
	done
	@$(MAKE) -C ${INTEGRATION} ${MAKECMDGOALS}
	@$(RM) ${INTEGRATION}/partition1.xef resident_sw
	rm -f /boot/xm/l1x3-funciona.xm
