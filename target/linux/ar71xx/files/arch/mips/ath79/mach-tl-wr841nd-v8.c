/*
 *  Atheros TL_WR841NDV8 board (WASP SoC) support
 *
 *  Copyright (C) 2010-2011 Jaiganesh Narayanan <jnarayanan@atheros.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/ath9k_platform.h>

#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"


#define TL_WR841NDV8_GPIO_LED_USB	11
#define TL_WR841NDV8_GPIO_LED_WLAN	13
#define TL_WR841NDV8_GPIO_LED_STATUS	14
#define TL_WR841NDV8_GPIO_LED_WPS	15

#define TL_WR841NDV8_GPIO_LED_wan	18 
#define TL_WR841NDV8_GPIO_LED_lan1	19
#define TL_WR841NDV8_GPIO_LED_lan2	20
#define TL_WR841NDV8_GPIO_LED_lan3	21
#define TL_WR841NDV8_GPIO_LED_lan4	12

#define TL_WR841NDV8_GPIO_BTN_RESET	17

#define TL_WR841NDV8_MAC0_OFFSET	0x1000
#define TL_WR841NDV8_MAC1_OFFSET	0x1006
#define TL_WR841NDV8_WMAC_CALDATA_OFFSET 0x1000
#define TL_WR841NDV8_PCIE_CALDATA_OFFSET 0x5000

#define TL_WR841NDV8_KEYS_POLL_INTERVAL	20	/* msecs */
#define TL_WR841NDV8_KEYS_DEBOUNCE_INTERVAL	(3 * TL_WR841NDV8_KEYS_POLL_INTERVAL)

static const char *tl_wr841ndv8_part_probes[] = {
	"tp-link",
	NULL,
};

static struct flash_platform_data tl_wr841ndv8_flash_data = {
	.part_probes	= tl_wr841ndv8_part_probes,
};

static struct gpio_led tl_wr841ndv8_leds_gpio[] __initdata = {
	{
		.name		= "tp-link:green:system",
		.gpio		= TL_WR841NDV8_GPIO_LED_STATUS,
		.active_low	= 1,
		.default_trigger= "default-on",
	}, {
		.name		= "tp-link:green:qss",
		.gpio		= TL_WR841NDV8_GPIO_LED_WPS,
		.active_low	= 1,
	}, {
		.name		= "tp-link:green:wlan",
		.gpio		= TL_WR841NDV8_GPIO_LED_WLAN,
		.active_low	= 1,
	}, {
		.name		= "tp-link:green:wan",
		.gpio		= TL_WR841NDV8_GPIO_LED_wan,
		.active_low	= 1,
	}, {
		.name		= "tp-link:green:lan1",
		.gpio		= TL_WR841NDV8_GPIO_LED_lan1,
		.active_low	= 1,
	}, {
		.name		= "tp-link:green:lan2",
		.gpio		= TL_WR841NDV8_GPIO_LED_lan2,
		.active_low	= 1,
	}, {
		.name		= "tp-link:green:lan3",
		.gpio		= TL_WR841NDV8_GPIO_LED_lan3,
		.active_low	= 1,
	}, {
		.name		= "tp-link:green:lan4",
		.gpio		= TL_WR841NDV8_GPIO_LED_lan4,
		.active_low	= 1,
	}
};

static struct gpio_keys_button tl_wr841ndv8_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = TL_WR841NDV8_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= TL_WR841NDV8_GPIO_BTN_RESET,
		.active_low	= 1,
	}
};

static void __init tl_wr841ndv8_gmac_setup(void)
{
	void __iomem *base;
	u32 t;

	base = ioremap(AR934X_GMAC_BASE, AR934X_GMAC_SIZE);

	t = __raw_readl(base + AR934X_GMAC_REG_ETH_CFG);
	t &= ~(AR934X_ETH_CFG_RGMII_GMAC0 | AR934X_ETH_CFG_MII_GMAC0 |
	       AR934X_ETH_CFG_MII_GMAC0 | AR934X_ETH_CFG_SW_ONLY_MODE);
	__raw_writel(t, base + AR934X_GMAC_REG_ETH_CFG);

	iounmap(base);
}

static void __init tl_wr841ndv8_setup(void)
{
	u8 *ee = (u8 *) KSEG1ADDR(0x1f3f1000);
	u8 *mac = (u8 *) KSEG1ADDR(0x1f01fc00);
	tl_wr841ndv8_gmac_setup();

	ath79_gpio_function_disable(AR933X_GPIO_FUNC_ETH_SWITCH_LED0_EN |
				    AR933X_GPIO_FUNC_ETH_SWITCH_LED1_EN |
				    AR933X_GPIO_FUNC_ETH_SWITCH_LED2_EN |
				    AR933X_GPIO_FUNC_ETH_SWITCH_LED3_EN |
				    AR933X_GPIO_FUNC_ETH_SWITCH_LED4_EN);

	ath79_gpio_output_select(TL_WR841NDV8_GPIO_LED_USB, AR934X_GPIO_OUT_GPIO);

	ath79_register_usb();

	ath79_register_leds_gpio(-1, ARRAY_SIZE(tl_wr841ndv8_leds_gpio),
					tl_wr841ndv8_leds_gpio);

	ath79_register_gpio_keys_polled(-1, TL_WR841NDV8_KEYS_POLL_INTERVAL,
					 ARRAY_SIZE(tl_wr841ndv8_gpio_keys),
					 tl_wr841ndv8_gpio_keys);

	ath79_register_m25p80(&tl_wr841ndv8_flash_data);

	ath79_register_mdio(0, 0x0);
	ath79_register_mdio(1, 0x0);

	// eth0 wan
	ath79_init_mac(ath79_eth0_data.mac_addr, mac, 0);

	ath79_switch_data.phy4_mii_en = 1;

	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_MII;
	ath79_eth0_data.phy_mask = BIT(4);
	ath79_eth0_data.mii_bus_dev = &ath79_mdio1_device.dev;

	ath79_register_eth(0);


	// eth1  is connected to the internal switch  lan
	ath79_init_mac(ath79_eth1_data.mac_addr, mac, 1);
	ath79_eth1_data.phy_if_mode = PHY_INTERFACE_MODE_GMII;
	ath79_eth1_data.speed = SPEED_1000;
	ath79_eth1_data.duplex = DUPLEX_FULL;

	ath79_register_eth(1);


	ath79_register_wmac(ee, mac);
	ap91_pci_init(ee + TL_WR841NDV8_PCIE_CALDATA_OFFSET, NULL);

}

MIPS_MACHINE(ATH79_MACH_TL_WR841ND_V8, "TL-WR841ND-v8", 
	     "TP-LINK TL-WR841ND v8", tl_wr841ndv8_setup);

