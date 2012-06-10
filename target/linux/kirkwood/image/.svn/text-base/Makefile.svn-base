#
# Copyright (C) 2009-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

JFFS2OPTS += --little-endian --pagesize=0x800 --no-cleanmarkers --pad
JFFS2_BLOCKSIZE = 128k

define Image/Prepare
	cp $(LINUX_DIR)/arch/arm/boot/uImage $(KDIR)/uImage
	cp $(LINUX_DIR)/arch/arm/boot/zImage $(KDIR)/zImage
endef

define Image/BuildKernel
# do mach-id fixup here, if needed
	cp $(KDIR)/uImage $(BIN_DIR)/$(IMG_PREFIX)-uImage
# QNAP TS-41x: mach id 2502 (0x09C6)
	echo -en "\x09\x1c\xa0\xe3\xC6\x10\x81\xe3" > $(KDIR)/ts41x-zImage
	cat $(LINUX_DIR)/arch/arm/boot/zImage >> $(KDIR)/ts41x-zImage
	$(STAGING_DIR_HOST)/bin/mkimage -A arm -O linux -T kernel \
	-C none -a 0x0008000 -e 0x0008000 -n 'Linux-$(LINUX_VERSION)' \
	-d $(KDIR)/ts41x-zImage $(KDIR)/ts41x-uImage
	dd if=$(KDIR)/ts41x-uImage of=$(BIN_DIR)/openwrt-TS41X-uImage \
		ibs=2097152 conv=sync
endef

define Image/Build
	$(if $(Image/Build/$(1)), \
		$(call Image/Build/$(1),$(1)), \
		$(CP) $(KDIR)/root.$(1) $(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).img \
	)
endef

define Image/Build/jffs2-128k
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).img \
		bs=2048 conv=sync
endef

define Image/Build/squashfs
	$(STAGING_DIR_HOST)/bin/mkimage -A arm -O linux -T multi \
	-C none -a 0x00008000 -e 0x00008000 -n 'Linux-$(LINUX_VERSION)-initrd' \
	-d $(KDIR)/zImage:$(KDIR)/root.squashfs $(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).uimg

	$(call prepare_generic_squashfs,$(KDIR)/root.squashfs)
	( \
		dd if=$(KDIR)/uImage bs=2048k conv=sync; \
		dd if=$(KDIR)/root.$(1) bs=128k conv=sync; \
	) > $(BIN_DIR)/$(IMG_PREFIX)-$(PROFILE)-$(1).img
endef

$(eval $(call BuildImage))
