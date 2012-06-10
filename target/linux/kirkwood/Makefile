#
# Copyright (C) 2009-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=kirkwood
BOARDNAME:=Marvell Kirkwood
FEATURES:=targz usb jffs2 squashfs

MAINTAINER:=Imre Kaloz <kaloz@openwrt.org>

LINUX_VERSION:=3.2.12

include $(INCLUDE_DIR)/target.mk

KERNELNAME:="uImage"

DEFAULT_PACKAGES += kmod-crypto-core kmod-crypto-mv-cesa \
	kmod-leds-gpio kmod-gpio-button-hotplug swconfig \
	kmod-ath9k wpad-mini uboot-envtools

$(eval $(call BuildTarget))
