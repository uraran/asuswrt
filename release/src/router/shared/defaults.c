/* nvram table for asuswrt, version 0.1 */

#include <epivers.h>
#include <typedefs.h>
#include <string.h>
#include <ctype.h>
#include <bcmnvram.h>
#include <wlioctl.h>
#include <stdio.h>
#include <shared.h>
#include <version.h>
#include <shutils.h>

#define XSTR(s) STR(s)
#define STR(s) #s

// stub for wlconf, etc.
struct nvram_tuple router_defaults[] = {
	// NVRAM for restore_defaults: system wide parameters
	{ "nvramver", RTCONFIG_NVRAM_VER, CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "restore_defaults", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Set to 0 to not restore defaults on boot */
#if defined(RPAC66) || defined(RPAC87) || defined(RTCONFIG_DEFAULT_REPEATER_MODE)
	{ "sw_mode", "2", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* big switch for different mode */
#elif defined(RTCONFIG_DEFAULT_AP_MODE)
	{ "sw_mode", "3", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* big switch for different mode */
#else
	{ "sw_mode", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* big switch for different mode */
#endif

	{ "preferred_lang", "EN", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// NVRAM from init_nvram: system wide parameters accodring to model and mode
	//{ "wan_ifnames", "" },
	//{ "lan_ifnames", "" },
	//{ "lan1_ifnames", "" },
	//{ "vlan_enable", "" },
	//{ "vlan0ports", "" },
	//{ "vlan1ports", "" },
	//{ "vlan2ports", "" },
	/* Guest H/W parameters */

	//Use to do force submit form request
	{ "force_change", "0", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// NVRAM for switch
	{ "switch_stb_x", "0", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* oleg patch */
	{ "switch_wantag", "none", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* for IPTV/VoIP case */
	{ "switch_wan0tagid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Wan Port */
	{ "switch_wan0prio", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "switch_wan1tagid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IPTV Port */
	{ "switch_wan1prio", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "switch_wan2tagid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* VoIP Port */
	{ "switch_wan2prio", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "iptv_port_settings", "12", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_unit", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_subunit", "-1", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_vifnames", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Virtual Interface Names */
	/* Wireless parameters */
#ifdef CONFIG_BCMWL5
	{ "wl_version", EPI_VERSION_STR, CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Os revision */
#endif
#ifdef RTCONFIG_WIRELESS_SWITCH
	{ "wl_HW_switch", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* siwtch WiFi slash */
#endif
	{ "wl_ifname", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Interface name */
	{ "wl_hwaddr", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* MAC address */
#ifdef RTCONFIG_RALINK
#elif defined(RTCONFIG_QCA)
#elif defined(RTCONFIG_REALTEK)
	{ "wl_phytype", "v", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Current wireless band (a (5 GHz) v(5GHz ac) b (2.4 GHz) or g (2.4 GHz)) */
	{ "wl0_phytype", "v", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_phytype", "v", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl_phytype", "n", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Current wireless band (a (5 GHz) b (2.4 GHz) or g (2.4 GHz)) */
	{ "wl_corerev", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Current core revision */
	{ "wl_phytypes", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* List of supported wireless bands (e.g. ga) */
	{ "wl_radioids", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* List of radio IDs */
#endif
	{ "wl_ssid", SSID_PREFIX, CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Service set ID (network name) */
#ifdef RTCONFIG_CONCURRENTREPEATER
	{ "wl0_ssid", DEF_SSID_2G, CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_ssid", DEF_SSID_5G, CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_RALINK) || defined(RTCONFIG_REALTEK)
	{ "wl1_bw", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1.1_bw", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1.1_channel", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl0_bw", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl0.1_bw", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl0.1_channel", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	
#endif
#else
#if !defined (RTAC3200) && !defined (RTAC5300) && !defined (GTAC5300) && !defined(MAPAC2200)
	{ "wl1_ssid", SSID_PREFIX "_5G", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl1_ssid", SSID_PREFIX "_5G-1", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_ssid", SSID_PREFIX "_5G-2", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif	// RTCONFIG_CONCURRENTREPEATER

	{ "wl_bss_enabled", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Service set Enable (1) or disable (0) radio */
	{ "wl_bw_enabled", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Bandwidth Limiter of Guest Network set Enable (1) or disable (0) radio */
	{ "wl_bw_dl", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Bandwidth Limiter DownStream */
	{ "wl_bw_ul", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Bandwidth Limiter UpStream */
#ifdef __CONFIG_HSPOT__	/* See "default_get" below. */
	{ "wl_hsflag", "3159", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Passpoint Flags */
	{ "wl_hs2en", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Passpoint Enable (1) */
	{ "wl_hs2cap", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Passpoint Realese 2 (1) */
	{ "wl_opercls", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Operating Class */
	{ "wl_anonai", "anonymous.com", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Anonymous NAI */

	{ "wl_oplist", "T-Mobile!eng", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Operator Friendly Name List */

	{ "wl_homeqlist", "t-mobile.com:rfc4282", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* NAIHomeRealmQueryList */
	{ "wl_wanmetrics", "1:0:0=2500>384=0>0=0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WAN Metrics */
	{ "wl_osu_ssid", "CellSpot_AutoConnect", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* OSU SSID */

	{ "wl_osu_frndname", "T-Mobile!eng", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* OSU Friendly Name */

	{ "wl_osu_uri", "https://osu-server.t-mobile.com/", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* OSU Server URI */

	{ "wl_osu_nai", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* OSU NAI */
	{ "wl_osu_method", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* OSU Method */

	{ "wl_osu_icons", "icon_red_zxx.png+icon_red_eng.png", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* OSU Icons */

	{ "wl_osu_servdesc", "T-MobilePasspointservice!eng", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* OSU Serv Desc */

	/* ---- Passpoint Flag ----------------------------------- */
	{ "wl_gascbdel", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* GAS CB Delay */
	{ "wl_4framegas", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 4 Frame GAS */

	/* ---- temporary ----------------------------------- */
	{ "wl_osuicon_id", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* OSU Provider's Icon ID */
	{ "wl_conn_id", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Connection Capability ID */

//#ifdef __CONFIG_802_11U__
	{ "wl_u11en", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 802.11u IW Enable (1) disable (0) radio */
	{ "wl_iwint", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Internet Enable (1) disable (0) radio */
	{ "wl_iwnettype", "2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Select Access Network Type */
	{ "wl_hessid", "50:6F:9A:00:11:22", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Interworking HESSID */
	{ "wl_ipv4addr", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Select IPV4 Address Type Availability */
	{ "wl_ipv6addr", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Select IPV6 Address Type Availability */

	{ "wl_netauthlist", "accepttc=+" "httpred=https://t-mobile.com", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Network Authentication Type List */

	{ "wl_venuegrp", "2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Venue Name List */
	{ "wl_venuetype", "8", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Venue Type */

	{ "wl_venuelist", "542D4D6F62696C6521656E67", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Venue Name List */

	{ "wl_ouilist", "506F9A;001BC504BD", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Roaming Consortium List */
	{ "wl_3gpplist", "310:260;310:310", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 3GPP Cellular Network Information List */
	{ "wl_domainlist", "t-mobile.com", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Domain Name List */

	{"wl_realmlist", "wlan.mnc260.mcc310.3gppnetwork.org+0+21=2,4#5,7?"	"wlan.mnc260.mcc310.3gppnetwork.org+0+13=5,6?"	"wlan.mnc310.mcc310.3gppnetwork.org+0+21=2,4#5,7?"	"wlan.mnc310.mcc310.3gppnetwork.org+0+13=5,6", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//#endif /* __CONFIG_802_11U__ */

#endif /* __CONFIG_HSPOT__ */
#ifdef RTCONFIG_REALTEK
	{ "wl_country_code", DEF_COUNTRY_CODE, CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
//	{ "wl_country_code", "", 0 },		/* Country Code (default obtained from driver) */
#ifdef CONFIG_BCMWL5
	{ "wl_country_rev", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Regrev Code (default obtained from driver) */
#endif
	{ "wl_radio", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable (1) or disable (0) radio */
	{ "wl_closed", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Closed (hidden) network */
	{ "wl_ap_isolate", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* AP isolate mode */

	{ "wl_igs", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* BCM: wl_wmf_bss_enable Ralink: IGMPSnEnable */

#ifdef CONFIG_BCMWL5
	{ "wl_wmf_bss_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WMF Enable/Disable */
	{ "wl_mcast_regen_bss_enable", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* MCAST REGEN Enable/Disable */

	/* operational capabilities required for stations to associate to the BSS */
#ifdef RTCONFIG_BCMWL6
	{ "wl_bss_opmode_cap_reqd", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wl_rxchain_pwrsave_enable", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Rxchain powersave enable */
	{ "wl_rxchain_pwrsave_quiet_time", "1800", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Quiet time for power save */
	{ "wl_rxchain_pwrsave_pps", "10", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Packets per second threshold for power save */
	{ "wl_rxchain_pwrsave_stas_assoc_check", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* STAs associated before powersave */
	{ "wl_radio_pwrsave_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Radio powersave enable */
	{ "wl_radio_pwrsave_quiet_time", "1800", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Quiet time for power save */
	{ "wl_radio_pwrsave_pps", "10", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Packets per second threshold for power save */
	{ "wl_radio_pwrsave_level", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Radio power save level */
	{ "wl_radio_pwrsave_stas_assoc_check", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* STAs associated before powersave */
#endif
	{ "wl_mode", "ap", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* AP mode (ap|sta|wds) */
#ifdef RTCONFIG_RALINK
#elif defined(RTCONFIG_QCA)
#else
#if defined(RTCONFIG_BCM9)
	{ "wl_dwds", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wl_lazywds", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable (lazy) WDS mode (0|1) */
	{ "wl_wds", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* xx:xx:xx:xx:xx:xx ... */
	{ "wl_wds_timeout", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WDS link detection interval defualt 1 sec */
	{ "wl_wep", "disabled", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WEP data encryption (enabled|disabled) */
	{ "wl_auth", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Shared key authentication optional (0) or required (1) */
#endif
	{ "wl_key", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Current WEP key */
	{ "wl_key1", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 5/13 char ASCII or 10/26 char hex */
	{ "wl_key2", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 5/13 char ASCII or 10/26 char hex */
	{ "wl_key3", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 5/13 char ASCII or 10/26 char hex */
	{ "wl_key4", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 5/13 char ASCII or 10/26 char hex */
#ifdef RTCONFIG_RALINK
#elif defined(RTCONFIG_QCA)
#else
	{ "wl_maclist", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* xx:xx:xx:xx:xx:xx ... */
#endif
	{ "wl_macmode", "disabled", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* allow only */
	{ "wl_assoc_retry_max", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Non-zero limit for association retries */
#ifndef RTCONFIG_BCMWL6
	{ "wl_channel", "0", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Channel number */
#else
#if !(defined(HND_ROUTER) && defined(RTCONFIG_HNDMFG))
	{ "wl_chanspec", "0", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Channel specification */
#else
	{ "wl_chanspec", "11", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Channel specification */
#endif
#endif
#ifdef RTCONFIG_RALINK
#elif defined(RTCONFIG_QCA)
#else
	{ "wl_noisemitigation", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_reg_mode", "off", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Regulatory: 802.11H(h)/802.11D(d)/off(off) */
	{ "wl_rate", "0", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Rate (bps */
#if 0
	{ "wl_mrate", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Mcast Rate (bps */
#endif
#endif
	{ "wl_mrate_x", "0", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Mcast Auto rate */
#ifndef RTCONFIG_WIFILOGO
	{ "wl_frameburst", "on", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* BRCM Frambursting mode (off|on) */
#else
	{ "wl_frameburst", "off", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#if defined (RTCONFIG_BCMARM) && !defined (RTCONFIG_BCM7) && !defined(RTCONFIG_BCM9)
	{ "frameburst_dyn", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Frameburst controlled dynamically if on */
#endif
	{ "wl_rateset", "default", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* default or all or 12 */
	{ "wl_frag", "2346", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Fragmentation threshold */
	{ "wl_rts", "2347", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* RTS threshold */
#ifdef RTCONFIG_RALINK
	{ "wl_dtim", "1", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#elif defined(RTCONFIG_QCA)
	{ "wl_dtim", "1", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl_dtim", "3", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* DTIM period */
#endif
	{ "wl_bcn", "100", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Beacon interval */
#ifdef CONFIG_BCMWL5
	{ "wl_bcn_rotate", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Beacon rotation */
#endif
	{ "wl_plcphdr", "long", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 802.11b PLCP preamble type */
#ifdef RTCONFIG_RALINK
	{ "wl_nmode_protection", "auto", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 802.11n protection */
#elif defined(RTCONFIG_QCA) || defined(RTCONFIG_BCM_7114) || defined(HND_ROUTER)
	{ "wl_nmode_protection", "auto", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 802.11n protection */
#else
	{ "wl_gmode", "XSTR(GMODE_AUTO)", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 54g mode */
#endif
	{ "wl_gmode_protection", "auto", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 802.11g RTS/CTS protection (off|auto) */

#ifdef RTCONFIG_OPTIMIZE_XBOX
	{ "wl_optimizexbox", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Optimize WiFi packet for Xbox */
#endif
	{ "wl_wme", "auto", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME mode (off|on|auto) */
#ifdef CONFIG_BCMWL5
	{ "wl_wme_bss_disable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME BSS disable advertising (off|on) */
	{ "wl_antdiv", "-1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Antenna Diversity (-1|0|1|3) */
	{ "wl_infra", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Network Type (BSS/IBSS) */
//	{ "wl_interfmode", "2", 0 },		/* Interference mitigation mode (2/3/4) */
#ifndef RTCONFIG_BCMWL6
	{ "wl_nbw_cap", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* BW Cap; def 20inB and 40inA */
#else
	{ "wl_bw_cap", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* BW Cap; 20 MHz */
#endif
#endif
#ifndef RTCONFIG_BCMWL6
	{ "wl_nctrlsb", "lower", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* N-CTRL SB */
#endif
#ifdef RTCONFIG_WL_AUTO_CHANNEL
	{ "AUTO_CHANNEL", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: real channel; 1: Auto; for RT-N12HP & RT-N14UHP */
#endif
#ifdef RTCONFIG_TCODE
	{ "location_code", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: real channel; 1: Auto; for RT-N12HP & RT-N14UHP */
#endif
#ifdef RTCONFIG_RALINK
	{ "wl_nband", "2", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* N-BAND */
	{ "wl0_nband", "2", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 2.4 GHz */
	{ "wl1_nband", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 5 GHz */
#elif defined(RTCONFIG_QCA)
	{ "wl_nband", "2", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* N-BAND */
	{ "wl0_nband", "2", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 2.4 GHz */
	{ "wl1_nband", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 5 GHz */
#if defined(MAPAC2200)
	{ "wl2_nband", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 5G-2 */
#endif	/* MAPAC2200 */
#else
	{ "wl_nband", "2", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* N-BAND */
#if defined (RTAC3200) || defined (RTAC5300) || defined (GTAC5300)
	{ "wl1_nband", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_nband", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl1_nband", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 5 GHz */
#endif

	{ "wl_nmcsidx", "-1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* MCS Index for N - rate */
	{ "wl_nmode", "-1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* N-mode */
	{ "wl_rifs_advert", "auto", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* RIFS mode advertisement */
	{ "wl_vlan_prio_mode", "off", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* VLAN Priority support */
#ifdef CONFIG_BCMWL5
	{ "wl_btc_mode", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* bluetooth coexistance(BTC) mode Disable(0) */
#endif
	{ "wl_leddc", "0x640000", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 100% duty cycle for LED on router */
	{ "wl_rxstreams", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 802.11n Rx Streams */
	{ "wl_txstreams", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 802.11n Tx Streams 0 */
	{ "wl_stbc_tx", "auto", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Default STBC TX setting */
	{ "wl_stbc_rx", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Default STBC RX setting */
	{ "wl_ampdu", "auto", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Default AMPDU setting */
#ifndef RTCONFIG_BCMWL6
	{ "wl_nreqd", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Require 802.11n support */
#endif
#endif	/* RTCONFIG_RALINK */
#ifdef RTCONFIG_RALINK
	{ "wl_mimo_preamble", "mm", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#elif defined(RTCONFIG_QCA)
	{ "wl_mimo_preamble", "mm", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#elif defined(RTCONFIG_BCMWL6)
//	{ "wl_mimo_preamble", "gfbcm", 0 },	/* Mimo PrEamble: mm/gf/auto/gfbcm */
#endif
#ifdef CONFIG_BCMWL5
#if defined(RTCONFIG_BCM_7114) || defined(RTCONFIG_BCM9) || defined(HND_ROUTER)
	{ "wl_ampdu_rtylimit_tid", "5 5 5 5 5 5 5 5", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_ampdu_rr_rtylimit_tid", "2 2 2 2 2 2 2 2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	/* Default AMPDU retry limit per-tid setting */
	{ "wl_ampdu_rtylimit_tid", "7 7 7 7 7 7 7 7", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	/* Default AMPDU regular rate retry limit per-tid setting */
	{ "wl_ampdu_rr_rtylimit_tid", "3 3 3 3 3 3 3 3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#if defined(RTCONFIG_BCMARM) && !defined(RTCONFIG_BCM7) && !defined(RTCONFIG_BCM_7114) && !defined(RTCONFIG_BCM9) && !defined(HND_ROUTER)
	{ "wl_rx_amsdu_in_ampdu", "auto", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Default RX AMSDU In AMPDU setting */
#endif
	{ "wl_obss_coex", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Default OBSS Coexistence setting - OFF */
#ifdef RTCONFIG_BCMWL6
#if !defined(RTCONFIG_BCM7) && !defined(RTCONFIG_BCM_7114) && !defined(RTCONFIG_BCM9) && !defined(HND_ROUTER)
	{ "wl_ack_ratio", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#if !defined(RTCONFIG_BCM9)
	{ "wl_ampdu_mpdu", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_ampdu_rts", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_BCMARM
#if defined(RTCONFIG_BCM_7114) || defined(HND_ROUTER)
	{ "wl_turbo_qam", "2", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl_turbo_qam", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if !defined(RTCONFIG_BCM9) && !defined(RTAC56U) && !defined(RTAC56S) && !defined(RTCONFIG_BCM_7114) && !defined(HND_ROUTER)
	{ "wl0_turbo_qam", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
#endif	// RTCONFIG_BCMARM
#endif	// RTCONFIG_BCMWL6
#endif	// CONFIG_BCMWL5
#if defined(CONFIG_BCMWL5) || defined(RTCONFIG_REALTEK)
	{ "wl_amsdu", "auto", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Default AMSDU setting */
#endif
	/* WPA parameters */
	{ "wl_auth_mode", "none", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Network authentication mode (radius|none) */
	{ "wl_wpa_psk", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WPA pre-shared key */
	{ "wl_wpa_gtk_rekey", "3600", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* GTK rotation interval */
	{ "wl_radius_ipaddr", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* RADIUS server IP address */
	{ "wl_radius_key", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* RADIUS shared secret */
	{ "wl_radius_port", "1812", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* RADIUS server UDP port */
	{ "wl_crypto", "tkip+aes", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WPA data encryption */
#if 0
	{ "wl_net_reauth", "3600", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Network Re-auth/PMK caching duration */
#else
	{ "wl_pmk_cache", "60", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Network Re-auth/PMK caching duration in minutes */
#endif
	{ "wl_akm", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WPA akm list */
#ifdef RTCONFIG_BCMWL6
#ifdef MFP
	{ "wl_mfp", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Protected Management Frame */
#endif
	{ "wl_psr_mrpt", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Default to one level repeating mode */
#endif
#ifdef RTCONFIG_WPS
	/* WSC parameters */
#ifdef RTCONFIG_RALINK
#elif defined(RTCONFIG_QCA)
#else
	{ "wps_version2", "enabled", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Must specified before other wps variables */
	{ "wl_wps_mode", "enabled", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* enabled wps */
	{ "wl_wps_config_state", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* config state unconfiged */
#endif
//	{ "wps_device_pin", "12345670", 0 },	// it is mapped to secret_code
#if 0
	{ "wps_modelname", "RT_BUILD_NAME", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wps_modelname", "Wi-Fi Protected Setup Router", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wps_mfstring", "ASUSTeK Computer Inc.", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//	{ "wps_device_name", RT_BUILD_NAME, 0 },
#ifdef RTCONFIG_RALINK
#elif defined(RTCONFIG_QCA)
#else
	{ "wl_wps_reg", "enabled", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wps_sta_pin", "00000000", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//	{ "wps_modelnum", RT_BUILD_NAME},
	/* Allow or Deny Wireless External Registrar get or configure AP security settings */
	{ "wps_wer_mode", "allow", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "lan_wps_oob", "enabled", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* OOB state */
	{ "lan_wps_reg", "enabled", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* For DTM 1.4 test */

	{ "lan1_wps_oob", "enabled", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lan1_wps_reg", "enabled", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_WPS_LED
	{ "wps_success", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* after 3 seconds under WPS success */
#endif
#endif /* __CONFIG_WPS__ */
#ifdef __CONFIG_WFI__
	{ "wl_wfi_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disable */
	{ "wl_wfi_pinmode", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: auto pin */
#endif /* __CONFIG_WFI__ */
#ifdef __CONFIG_WAPI_IAS__
	/* WAPI parameters */
	{ "wl_wai_cert_name", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* AP certificate name */
	{ "wl_wai_cert_index", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* AP certificate index. 1:X.509 */
	{ "wl_wai_cert_status", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* AP certificate status */
	{ "wl_wai_as_ip", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* ASU server IP address */
	{ "wl_wai_as_port", "3810", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* ASU server UDP port */
#endif /* __CONFIG_WAPI_IAS__ */
#ifdef CONFIG_BCMWL5
	/* WME parameters (cwmin cwmax aifsn txop_b txop_ag adm_control oldest_first) */
	/* EDCA parameters for STA */
	{ "wl_wme_sta_be", "15 1023 3 0 0 off off", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME STA AC_BE parameters */
	{ "wl_wme_sta_bk", "15 1023 7 0 0 off off", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME STA AC_BK parameters */
	{ "wl_wme_sta_vi", "7 15 2 6016 3008 off off", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME STA AC_VI parameters */
	{ "wl_wme_sta_vo", "3 7 2 3264 1504 off off", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME STA AC_VO parameters */

	/* EDCA parameters for AP */
	{ "wl_wme_ap_be", "15 63 3 0 0 off off", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME AP AC_BE parameters */
	{ "wl_wme_ap_bk", "15 1023 7 0 0 off off", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME AP AC_BK parameters */
	{ "wl_wme_ap_vi", "7 15 1 6016 3008 off off", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME AP AC_VI parameters */
	{ "wl_wme_ap_vo", "3 7 1 3264 1504 off off", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME AP AC_VO parameters */
#endif
	{ "wl_wme_no_ack", "off", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME No-Acknowledgment mode */
	{ "wl_wme_apsd", "on", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WME APSD mode */
#ifdef RTCONFIG_RALINK
#elif defined(RTCONFIG_QCA)
#else
	{ "wl_maxassoc", "128", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Max associations driver could support */
	{ "wl_bss_maxassoc", "128", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Max associations driver could support */

	{ "wl_sta_retry_time", "5", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Seconds between association attempts */
#endif
#ifdef BCMDBG
	{ "wl_nas_dbg", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable/Disable NAS Debugging messages */
#endif
	// ASUS used only?
	{ "wl_nmode_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0/1/2 */
#if defined(RTCONFIG_BCMWL6)
#if !(defined(HND_ROUTER) && defined(RTCONFIG_HNDMFG))
	{ "wl_bw", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0/1/2/3 auto/20/40/80MHz */
#else
	{ "wl_bw", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 }, /* 0/1/2/3 auto/20/40/80MHz */
#endif
#ifdef RTCONFIG_QTN
	{ "wl1_bw", "3", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0/1/2/3 auto/20/40/80MHz */
#endif
#else
	{ "wl_bw", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0/1/2 20 */
#endif
	{ "wl_auth_mode_x", "open", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
// open/shared/psk/wpa/radius
	{ "wl_wep_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
// WEP data encryption 0, 1, 2 : disabled/5/13
	{ "wl_timesched", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_radio_date_x", "1111111", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_radio_time_x", "00002359", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_radio_time2_x", "00002359", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_sched", "000000", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_phrase_x", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Passphrase */
	{ "wl_lanaccess", "off", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_expire", "0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_mbss", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_txpower", "100", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	/* Total number of clients of a guest network.
	 * 1 ~ maxsta:	1 ~ maxsta
	 * otherwise:	do nothing
	 */
	{ "wl_guest_num",	"10"		},

#if defined (RTCONFIG_RALINK) || defined (RTCONFIG_BCMWL6) || defined (RTCONFIG_REALTEK)
#if defined(RTAC53U) || defined(RTAC1200G) || defined(RTAC1200GP) || defined(RPAC68U) || defined(RPAC87) || defined(RPAC53)
	/* RT-AC53U disable txbf by default */
	{ "wl_txbf", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl_txbf", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_QTN
	{ "wl1_80211h", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
#endif
#ifdef RTCONFIG_BCMWL6
#ifdef RTCONFIG_BCMARM
	{ "wl_itxbf", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
#if defined(RTCONFIG_BCM_7114) || defined(HND_ROUTER)
	{ "wl_mumimo", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#elif defined(RTCONFIG_QTN) || defined(RTCONFIG_MUMIMO_2G) || defined(RTCONFIG_MUMIMO_5G)
	{ "wl_mumimo", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#if defined(RTCONFIG_REALTEK) && defined(RTCONFIG_MUMIMO_5G)
	{ "wl1_mumimo", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#if defined(RTCONFIG_QCA)
#if defined(RTCONFIG_WIFI_QCA9990_QCA9990) || defined(RTCONFIG_WIFI_QCA9994_QCA9994) || defined(RTCONFIG_SOC_IPQ40XX)
	{ "wl_turbo_qam", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_turbo_qam_brcm_intop", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_txbf", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_mumimo", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_implicitxbf", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "restwifi_qis", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif

#if defined (RTCONFIG_WIRELESSREPEATER) || defined (RTCONFIG_PROXYSTA)
#if defined (RTCONFIG_CONCURRENTREPEATER)
	/* 2.4GHz */
	{ "wlc0_list", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_ssid", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_wep", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_key", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_wep_key", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_auth_mode", "open", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_crypto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_wpa_psk", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_state", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_sbstate", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_scan_state", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_mode", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_ure_ssid", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_hide_pap", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc0_wifipxy", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	/* 5GHz */
	{ "wlc1_list", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_ssid", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_wep", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_key", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_wep_key", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_auth_mode", "open", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_crypto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_wpa_psk", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_state", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_sbstate", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_scan_state", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_mode", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_ure_ssid", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_hide_pap", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_wifipxy", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifndef RTCONFIG_BCMWL6
	{ "wlc0_nbw_cap", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_nbw_cap", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wlc0_bw_cap", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc1_bw_cap", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#else
	{ "wlc_list", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc_band", "", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc_ssid", "8f3610e3c9feabed953a6", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc_wep", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc_key", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc_wep_key", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc_auth_mode", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc_crypto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc_wpa_psk", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifndef RTCONFIG_BCMWL6
	{ "wlc_nbw_cap", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wlc_bw_cap", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wlc_ure_ssid", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wlc_express", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disabled */
#endif
#ifdef RTCONFIG_PROXYSTA
	{ "wlc_psta", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disabled */
#ifdef RTCONFIG_DPSTA
	{ "wlc_dpsta", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif

	// WPA parameters
#ifdef RTCONFIG_RALINK
	{ "wl_PktAggregate", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* UI configurable */
	{ "wl_HT_OpMode", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* UI configurable */
#if defined(RTAC85U) || defined(RTAC65U)
	{ "wl_DLSCapable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* UI configurable */
#else
	{ "wl_DLSCapable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* UI configurable */
#endif
	{ "wl_GreenAP", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* UI configurable */
	{ "wl_HT_AutoBA", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_HT_HTC", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_HT_RDG", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_HT_LinkAdapt", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_HT_MpduDensity", "5", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTAC85U) || defined(RPAC87)
	{ "wl_HT_AMSDU", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl_HT_AMSDU", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wl_HT_GI", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_HT_BAWinSize", "64", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_HT_MCS", "33", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_HT_BADecline", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//	{ "wl_HT_TxStream", "2" },
//	{ "wl_HT_RxStream", "3" },
//	{ "wl0_HT_TxStream", "2" }, // move to init_nvram for model dep.
//	{ "wl0_HT_RxStream", "2" }, // move to init_nvram for model dep.
//	{ "wl1_HT_TxStream", "2" }, // move to init_nvram for model dep.
//	{ "wl1_HT_RxStream", "3" }, // move to init_nvram for model dep.
	{ "wl_HT_STBC", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	// the following for ralink 5g only
	{ "wl_IEEE80211H", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_CSPeriod", "10", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_RDRegion", "FCC", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_CarrierDetect", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_ChannelGeography", "2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_MUMIMO_2G)
	{ "wl0_mumimo", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#if defined(RTCONFIG_MUMIMO_5G)
	{ "wl1_mumimo", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_CONCURRENTREPEATER)
	{ "wl1.1_mumimo", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
#if defined(RTCONFIG_QAM256_2G)
	{ "wl0_turbo_qam", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_turbo_qam", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_CONCURRENTREPEATER)
	{ "wl0.1_turbo_qam", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
#if defined(RTAC85U)
	{ "wl_txbf_en", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl_txbf_en", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
#if defined(RTCONFIG_QCA)
	{ "wl_HT_GI", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Short-GI */
	{ "wl_HT_STBC", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* STBC */
#endif

	{ "emf_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* it is common entry for all platform now broadcom: enable mrate */
#if defined(RTCONFIG_EMF)
	{ "emf_entry", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Static MFDB entry (mgrp:if) */
	{ "emf_uffp_entry", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Unreg frames forwarding ports */
	{ "emf_rtport_entry", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IGMP frames forwarding ports */
#ifdef RTCONFIG_BCMWL6
	{ "wl_wmf_ucigmp_query", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Disable Converting IGMP Query to ucast (default) */
	{ "wl_wmf_mdata_sendup", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Disable Sending Multicast Data to host (default) */
#ifdef RTCONFIG_BCMARM
	{ "wl_wmf_ucast_upnp", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Disable Converting upnp to ucast (default) */
	{ "wl_wmf_igmpq_filter", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Disable igmp query filter */
#endif
#endif
#endif

// WPS
	{ "wps_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wps_enable_x", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#ifdef RTCONFIG_RALINK
	{ "wl_wsc_config_state", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* config state unconfiged */
#endif
	{ "wps_band_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: 2.4G */
#if defined(RTCONFIG_WPSMULTIBAND)
	{ "wps_multiband", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable WPS on 2.4G and 5G both */
#else
	{ "wps_multiband", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

// Wireless WDS Mode
	{ "wl_mode_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0/1/2(ap/wds/hybrid) */
	{ "wl_wdsapply_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_wdslist", "", CKN_STR256, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* xxxxxxxxxxxx... */

// Wireless Mac Filter
	{ "wl_maclist_x", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* xxxxxxxxxxxx...xxxxxxxxxxx */

#if defined(RTCONFIG_WIFI_QCA9994_QCA9994)
	{ "wl_hwol", "0", 0 }, 			/* Hardware WiFi Offloading */
#endif

#if defined(RTCONFIG_BCMARM)
#ifndef RTCONFIG_WIFILOGO
	{ "wl_atf", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Airtime Fairness */
#else
	{ "wl_atf", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#if defined(RTCONFIG_BCM_7114) || defined(HND_ROUTER)
	{ "wl_atf_delay_disable", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* delay scheme to reserve air time for the idle flowring */
#endif
#elif defined(RTCONFIG_AIR_TIME_FAIRNESS)
	{ "wl_atf", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },			/* disable/enable Airtime Fairness */
	{ "wl_atf_mode", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 }, 		/* ATF mode: 0/1/2(by client/SSID/client+SSID) */
	{ "wl_atf_sta", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 }, 		/* ATF by client: <MAC>%<MAC>% */
	{ "wl_atf_ssid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 }, 		/* ATF by SSID: <%<%<%<% */
#endif

#ifdef RTCONFIG_BCMWL6
	{ "acs_ifnames", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "acs_dfs", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* enable DFS channels for acsd by default */
	{ "acs_band1", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "acs_band3", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "acs_ch13", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "wl_wet_tunnel", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Disable wet tunnel */

#ifdef RTCONFIG_DPSTA
	{ "dpsta_ifnames", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dpsta_policy", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dpsta_lan_uif", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dpsta_band", "-1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef TRAFFIC_MGMT_RSSI_POLICY
	{ "wl_trf_mgmt_rssi_policy", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Disable RSSI (default) */
#endif /* TRAFFIC_MGMT */

	/* Tx Beamforming */
#ifdef RTCONFIG_BCMARM
#ifdef HND_ROUTER
	{ "wl_txbf_bfr_cap", "2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 2 for mu-mimo case */
	{ "wl_txbf_bfe_cap", "2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 2 for mu-mimo case */
#else
	{ "wl_txbf_bfr_cap", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 2 for mu-mimo case */
	{ "wl_txbf_bfe_cap", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 2 for mu-mimo case */
#endif
#ifndef RTCONFIG_WIFILOGO
	{ "wl_txbf_imp", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl_txbf_imp", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#endif
#ifdef RTCONFIG_MUMIMO
#ifdef HND_ROUTER
	{ "wl_mu_features", "0x8000", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl_mu_features", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* mu-features=0x8000 when mu-mimo enabled */
#endif
#endif
#else
	{ "wl_txbf_timer", "25", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_BCM_7114
	{ "stop_gmac3", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

	/* acsd setting */
	{ "wl_acs_fcs_mode", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd disable FCS mode */
	{ "wl_dcs_csa_unicast", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* disable unicast csa */
	{ "wl_acs_excl_chans", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd exclude chanspec list */
#if defined(RTCONFIG_BCM7) || defined(RTCONFIG_BCM_7114) || defined(RTCONFIG_BCM9) || defined(HND_ROUTER)
	{ "wl_acs_dfs", "2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd fcs disable init DFS chan */
#else
	{ "wl_acs_dfs", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd fcs disable init DFS chan */
#endif
	{ "wl_acs_dfsr_immediate", "300 3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* immediate if > 3 switches last 5 minutes */
	{ "wl_acs_dfsr_deferred", "604800 5", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* deferred if > 5 switches in last 7 days */
	{ "wl_acs_dfsr_activity", "30 10240", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* active: >10k I/O in the last 30 seconds */
	{ "wl_acs_cs_scan_timer", "900", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd fcs cs scan timeout */
	{ "wl_acs_ci_scan_timer", "4", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd fcs CI scan period */
	{ "wl_acs_ci_scan_timeout", "300", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd fcs CI scan timeout */
	{ "wl_acs_scan_entry_expire", "3600", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd fcs scan expier time */
	{ "wl_acs_tx_idle_cnt", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd fcs tx idle thld */
	{ "wl_acs_chan_dwell_time", "70", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd fcs chan dwell time */
	{ "wl_acs_chan_flop_period", "70", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd fcs chan flip-flop time */
	{ "wl_intfer_period", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd fcs sample period */
	{ "wl_intfer_cnt", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd fcs sample cnt */
	{ "wl_intfer_txfail", "5", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* fcs non-TCP txfail threshold setting */
	{ "wl_intfer_tcptxfail", "5", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* fcs TCP txfail threshold setting */
#if defined(RTCONFIG_BCM_7114) || defined(HND_ROUTER)
	{ "wl_acs_bgdfs_enab", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd BGDFS toggle */
	{ "wl_acs_bgdfs_ahead", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd BGDFS ahead toggle */
	{ "wl_acs_bgdfs_idle_interval", "3600", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd traffic idle time before BGDFS */
	{ "wl_acs_bgdfs_idle_frames_thld", "36000", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd max frames for idle */
	{ "wl_acs_bgdfs_avoid_on_far_sta", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* bgdfs avoided by default on far sta */
	{ "wl_acs_far_sta_rssi", "-75", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* rssi threshold for far sta */
	{ "wl_acs_bgdfs_fallback_blocking_cac", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* full MIMO blocking CAC */
	{ "wl_acs_bgdfs_txblank_threshold", "50", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* tx blanking threshold for 3+1 */
	{ "wl_dyn160", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* dyn160 toggle */
	{ "wl_phy_dyn_switch_th", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* phy_dyn_switch_th (threshold values) */
	{ "wl_acsd_scs_dfs_scan", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acsd_scs_dfs_scan in scs mode */
#endif
	{ "acs_2g_ch_no_restrict", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: only pick from channel 1 */
	{ "acs_no_restrict_align", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: only aligned chanspec(few) can be picked (non-20Hz) */
#ifdef RTCONFIG_BCM_7114
	/* bgn itf BSS BUSY INTF I-ADJ FCS TXP NOISE TOT CNS ADJ*/
	{ "wl0_acs_pol", "0 100 -20 -15 -18 -1 -10 30 -1 1 1 0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acs default policy */
	{ "wl1_acs_pol", "0 100 -20 -15 -18 -1 -10 35 -1 1 1 0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acs default policy */
#if defined(RTAC5300)
	{ "wl2_acs_pol", "0 100 -20 -15 -18 -1 -10 35 -1 1 1 0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* acs default policy */
#endif
#endif

#ifdef RTCONFIG_BCMARM
#if defined(RTCONFIG_BCM7) || defined(RTCONFIG_BCM_7114) || defined(RTCONFIG_BCM9) && defined(HND_ROUTER)
	{ "wl_pspretend_retry_limit", "5", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable PsPretend */
#else
	{ "wl_pspretend_retry_limit", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable PsPretend */
#endif
#ifndef RTCONFIG_BCM7
	{ "wl_pspretend_threshold", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Disable PsPretend Threshold */
#else
#endif
	{ "wl_taf_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Disable TAF */
#endif
#ifdef BCM_BSD
	{ "bsd_role", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Disable Band Steer Daemon */
						/* 0:Disable, 1:Primary, 2:Helper, 3:Standalone */
	{ "bsd_hport", "9877", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* BSD helper port */
	{ "bsd_pport", "9878", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* BSD Primary port */
	{ "bsd_helper", "192.168.1.2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* BSD primary ipaddr */
	{ "bsd_primary", "192.168.1.1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* BSD Helper ipaddr */
	{ "smart_connect_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0:Disable */
#if 0
	{ "bsd_msglevel", "0x000010", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* BSD_DEBUG_STEER */
	{ "bsd_dbg", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTAC3200
	{ "bsd_ifnames", "eth2 eth1 eth3", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl0_bsd_steering_policy", "0 5 3 -52 0 110 0x22", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_steering_policy", "80 5 3 -82 0 0 0x20", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_steering_policy", "0 5 3 -82 0 0 0x28", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl0_bsd_sta_select_policy", "10 -52 0 110 0 1 1 0 0 0 0x122", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_sta_select_policy", "10 -82 0 0 0 1 1 0 0 0 0x24", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_sta_select_policy", "10 -82 0 0 0 1 1 0 0 0 0x28", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl0_bsd_if_select_policy", "eth3 eth1", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_if_select_policy", "eth2 eth3", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_if_select_policy", "eth2 eth1", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl0_bsd_if_qualify_policy", "0 0x0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_if_qualify_policy", "60 0x0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_if_qualify_policy", "0 0x4", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "bsd_bounce_detect", "180 2 3600", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "bsd_aclist_timeout", "3", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#elif defined(RTAC5300) || defined(GTAC5300)
	// Tri-Band
#ifndef GTAC5300
	{ "bsd_ifnames", "eth1 eth2 eth3", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "bsd_ifnames", "eth6 eth7 eth8", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wl0_bsd_steering_policy", "0 5 3 -52 0 110 0x22", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_steering_policy", "80 5 3 -82 0 0 0x20", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_steering_policy", "0 5 3 -82 0 0 0x28", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl0_bsd_sta_select_policy", "10 -52 0 110 0 1 1 0 0 0 0x122", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_sta_select_policy", "10 -82 0 0 0 1 1 0 0 0 0x24", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_sta_select_policy", "10 -82 0 0 0 1 1 0 0 0 0x28", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifndef GTAC5300
	{ "wl0_bsd_if_select_policy", "eth3 eth2", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_if_select_policy", "eth1 eth3", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_if_select_policy", "eth1 eth2", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl0_bsd_if_select_policy", "eth8 eth7", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_if_select_policy", "eth6 eth8", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_if_select_policy", "eth6 eth7", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wl0_bsd_if_qualify_policy", "0 0x0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_if_qualify_policy", "60 0x0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_if_qualify_policy", "0 0x4", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "bsd_bounce_detect", "180 2 3600", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "bsd_aclist_timeout", "3", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	// 5GHz Only
#ifndef GTAC5300
	{ "bsd_ifnames_x", "eth2 eth3", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "bsd_ifnames_x", "eth7 eth8", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wl1_bsd_steering_policy_x", "80 5 3 0 0 600 0x20", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_steering_policy_x", "0 5 3 0 700 0 0x28", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_sta_select_policy_x", "10 0 0 500 0 1 1 0 0 0 0x60", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_sta_select_policy_x", "10 0 900 0 0 1 1 0 0 0 0x68", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifndef GTAC5300
	{ "wl1_bsd_if_select_policy_x", "eth3", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_if_select_policy_x", "eth2", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl1_bsd_if_select_policy_x", "eth8", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_if_select_policy_x", "eth7", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wl1_bsd_if_qualify_policy_x", "60 0x0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl2_bsd_if_qualify_policy_x", "0 0x4", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "bsd_bounce_detect_x", "180 2 3600", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "bsd_aclist_timeout", "3", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#elif defined(RTAC88U) || defined(RTAC86U) || defined(AC2900) || defined(RTAC3100)
	// Dual-Band
#if defined(RTAC86U) || defined(AC2900)
	{ "bsd_ifnames", "eth5 eth6", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "bsd_ifnames", "eth1 eth2", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wl0_bsd_steering_policy", "0 5 3 -52 0 110 0x22", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_steering_policy", "80 5 3 -82 0 0 0x20", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl0_bsd_sta_select_policy", "10 -52 0 110 0 1 1 0 0 0 0x122", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_sta_select_policy", "10 -82 0 0 0 1 1 0 0 0 0x20", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTAC86U) || defined(AC2900)
	{ "wl0_bsd_if_select_policy", "eth6", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_if_select_policy", "eth5", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wl0_bsd_if_select_policy", "eth2", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_if_select_policy", "eth1", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wl0_bsd_if_qualify_policy", "0 0x0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl1_bsd_if_qualify_policy", "60 0x0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "bsd_bounce_detect", "180 2 3600", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "bsd_aclist_timeout", "3", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "bsd_scheme", "2", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#if defined(BCM_APPEVENTD)
	{ "appeventd_enable", "0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_BCMARM
#if !defined(RTCONFIG_BCM7) && !defined(RTCONFIG_BCM_7114) && !defined(HND_ROUTER)
	{ "acsd_foreground", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* daemon foreground mode control */
	{ "nas_foreground", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* daemon foreground mode control */
	{ "eapd_foreground", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* daemon foreground mode control */
#endif
#ifdef HND_ROUTER
	{ "wl_acs_boot_only", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* enable acsd full operation */
#endif
#endif
#ifdef __CONFIG_LBR_AGGR__
	{ "wl_lbr_aggr_en_mask", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* per tid/ac mask disable by default */
	{ "wl_lbr_aggr_len", "16", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* default aggregate len */
	{ "wl_lbr_aggr_release_timeout", "10", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* default release timeout in msec */
#endif /* __CONFIG_LBR_AGGR__ */
#if defined (RTCONFIG_BCM_7114) || defined(HND_ROUTER)
	{ "wl_flr_lfrag_max", "400", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_flr_lfrag_txpkts_min", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_flr_lfrag_txpkts_adjust", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wl_acs_use_escan", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable acsd enhanced scan mode */
#endif
#ifdef BCM_SSD
	{ "ssd_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Disable SSID Steer Daemon */
	{ "wl_ssd_type", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* default ssd_type "disabled" */
#endif
#if defined(RTCONFIG_BCM7) || defined(RTCONFIG_BCM_7114) || defined(RTCONFIG_BCM9) || defined(HND_ROUTER)
	{ "wl_dfs_pref", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* DFS Preferred channel value */
	{ "wl_probresp_mf", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* MAC filter based probe response */
	{ "wl_probresp_sw", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* SW probe response */
#if defined(RTCONFIG_BCM_7114) || defined(HND_ROUTER)
	{ "wl_vhtmode", "-1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* VHT mode */
#endif
	{ "wl_vht_features", "-1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* VHT features */
#endif
#if defined(RTCONFIG_BCM_7114) || defined(RTCONFIG_BCM9) || defined(HND_ROUTER)
	{ "wl_obss_dyn_bw", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Dynamic BWSW disable defaults */
#endif
#ifdef RTCONFIG_BCM_7114
	{ "wl_tpc_db", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* TPC Mitigation (db) */
	{ "dhd_msg_level", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef HND_ROUTER
	{ "dhd_hw_acc_dis_unit", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 }, /* Disable runner on 2G band to make tcpack suppression effective*/
	{ "wl0_custom_oui", "ff:ff:ff", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 }, /* scheme for forcing AC mode */
#endif
#ifdef __CONFIG_VISUALIZATION__
	{ "vis_dcon_ipaddr", "127.0.0.1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },/* Visualization Dcon IP address */
	{ "vis_do_remote_dcon", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Visualization remote debugging enabled flag */
	{ "vis_debug_level", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0},		/* Visualization debug messages to be printed */
	{ "vis_disable_animate", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0},	/* Visualization site survey animation on or off */
	{ "wl_rrm", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0},			/* Set rrm caps per interface */
#endif /* __CONFIG_VISUALIZATION__ */
#endif

	// make sure its purpose
	// advanced-ctnf
	{ "ct_tcp_timeout", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ct_udp_timeout", "30 180", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ct_timeout", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#ifdef CONFIG_BCMWL5
#ifdef HND_ROUTER
	{ "fc_disable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fc_pt_war", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fc_pt_udp_war", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "runner_disable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "watchdog", "6000", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "ctf_disable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ctf_disable_force", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_BCMARM
#if 0
#ifdef RTCONFIG_USB_MODEM
	// TODO: for the bad CTF. After updating CTF, need to mark these codes.
	{ "ctf_disable_modem", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
	{ "ctf_pt_udp", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_BCMFA
	{ "ctf_fa_mode", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif	// RTCONFIG_BCMARM
#endif	// HND_ROUTER
#ifndef RTCONFIG_BCMARM
#if defined(RTAC53U)
	// RT-AC53U disable gro can get better usb performance
	{ "gro_disable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "gro_disable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#else
	{ "gro_disable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif	// RTCONFIG_BCMARM
#endif	// CONFIG_BCMWL5
#if defined(RTCONFIG_LED_BTN) || defined(RTCONFIG_WPS_ALLLED_BTN)
	{ "AllLED", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

	// NVRAM for start_lan:
// LAN H/W parameters
	{ "lan_hwnames", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN driver names (e.g. et0) */
	{ "lan_hwaddr", "", CKN_STR17, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN interface MAC address */

	// LAN TCP/IP parameters
	{ "lan_unit", "-1", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_DEFAULT_AP_MODE) || defined(RTCONFIG_CONCURRENTREPEATER)
	{ "lan_proto", "dhcp", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* DHCP server [static|dhcp] */
#else
	{ "lan_proto", "static", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* DHCP server [static|dhcp] */
#endif
#if defined(RTN300)
	{ "lan_ipaddr", "192.168.50.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN IP address */
	{ "lan_ipaddr_rt", "192.168.50.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#elif defined(MAPAC1300) || defined(MAPAC2200)
	{ "lan_ipaddr", "192.168.72.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN IP address */
	{ "lan_ipaddr_rt", "192.168.72.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "lan_ipaddr", "192.168.1.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN IP address */
	{ "lan_ipaddr_rt", "192.168.1.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "lan_netmask", "255.255.255.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN netmask */
	{ "lan_netmask_rt", "255.255.255.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lan_gateway", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN Gateway */

	{ "lan_wins", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* x.x.x.x x.x.x.x ... */
	{ "lan_domain", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN domain name */
	{ "lan_lease", "86400", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN lease time in seconds */
	{ "lan_stp", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN spanning tree protocol */
#ifdef RTCONFIG_LACP
#ifdef GTAC5300
	{ "lacp_enabled", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },  /* LACP enable or not */
#else
	{ "lacp_enabled", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LACP enable or not */
#endif
	{ "bonding_policy", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* bonding policy */
#endif
	{ "lan_route", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Static routes (ipaddr:netmask:gateway:metric:ifname ...) */

#if defined(RTCONFIG_DEFAULT_AP_MODE) || defined(RTCONFIG_CONCURRENTREPEATER)
	{ "lan_dnsenable_x", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "lan_dnsenable_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "lan_dns1_x", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* x.x.x.x x.x.x.x ... */
	{ "lan_dns2_x", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "jumbo_frame_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	/* Guest TCP/IP parameters */

	/* Guest H/W parameters */
	{ "lan1_hwnames", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN driver names (e.g. et0) */
	{ "lan1_hwaddr", "", CKN_STR17, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN interface MAC address */

	{ "lan1_proto", "0", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* DHCP client [static|dhcp] */
	{ "lan1_ipaddr", "192.168.2.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN IP address */
	{ "lan1_netmask", "255.255.255.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN netmask */
	{ "lan1_gateway", "192.168.2.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN gateway */
	{ "lan1_wins", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* x.x.x.x x.x.x.x ... */
	{ "lan1_domain", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN domain name */
	{ "lan1_lease", "86400", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN lease time in seconds */
	{ "lan1_stp", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* LAN spanning tree protocol */
	{ "lan1_route", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Static routes (ipaddr:netmask:gateway:metric:ifname ...) */

	// NVRAM for start_dhcpd
	// DHCP server parameters
	{ "dhcp_enable_x", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTN300)
	{ "dhcp_start", "192.168.50.2", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp_end", "192.168.50.254", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#elif defined(MAPAC1300) || defined(MAPAC2200)
	{ "dhcp_start", "192.168.72.2", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp_end", "192.168.72.254", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "dhcp_start", "192.168.1.2", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp_end", "192.168.1.254", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "dhcp_lease", "86400", CKN_STR6, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp_gateway_x", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp_dns1_x", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp_wins_x", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp_static_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp_staticlist", "", CKN_STR2500, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcpd_lmax", "253", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcpc_mode", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DHCP_OVERRIDE
	{ "dnsqmode", "2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

	// NVRAM for start_dhcpd
	// Guest DHCP server parameters
	{ "dhcp1_enable_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp1_start", "192.168.2.2", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp1_end", "192.168.2.254", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp1_lease", "86400", CKN_STR6, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp1_gateway_x", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp1_dns1_x", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp1_wins_x", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp1_static_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dhcp1_staticlist", "", CKN_STR2500, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "time_zone", "GMT0", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "time_zone_dst", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "time_zone_dstoff", "M3.2.0/2, M10.2.0/2", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT },	/* 0 */
	{ "ntp_server1", "time.nist.gov", CKN_STR256, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ntp_server0", "pool.ntp.org", CKN_STR256, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// NVRAM for do_startic_routes
	{ "sr_enable_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sr_rulelist", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dr_enable_x", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* oleg patch */
	{ "mr_enable_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* oleg patch */
	{ "mr_qleave_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "mr_altnet_x", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// Domain Name
	{ "local_domain", "router.asus.com", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// NVRAM for switch control
//#ifdef RTCONFIG_SWITCH_CONTROL_8367
#if defined(RTCONFIG_RALINK) || \
    defined(RTCONFIG_SWITCH_RTL8370M_PHY_QCA8033_X2) || \
    defined(RTCONFIG_SWITCH_RTL8370MB_PHY_QCA8033_X2)
	{ "switch_ctrlrate_unknown_unicast", "20", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "switch_ctrlrate_unknown_multicast", "20", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "switch_ctrlrate_multicast", "20", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "switch_ctrlrate_broadcast", "20", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTN56U
	{ "switch_wanport_force_1g", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif

#ifdef RTCONFIG_RALINK
#if defined (W7_LOGO) || defined (WIFI_LOGO) || defined(RTCONFIG_DSL) || defined(RTCONFIG_RALINK_MT7628)
	{ "hwnat", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "hwnat", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
#ifdef RTCONFIG_QCA
	{ "qca_sfe", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_SOC_IPQ8064)
	/* Define QCA specific nvram variable to enable/disable GRO on
	 * LAN and WAN interface temporarilly.
	 * GRO is not turn on if WAN is PPPoE, PPTP, L2TP, USB
	 */
	{ "qca_gro", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0},
#endif

#if defined(RTCONFIG_SOC_QCA9557)
	{ "traffic_5g", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wl_wds_vht", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

	// NVRAM for start_wan
	{ "wan_unit", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Last configured connection */
	{ "wan_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	// NVRAM for start_wan_if
	/* WAN H/W parameters */
	{ "wan_hwname", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WAN driver name (e.g. et1) */
	{ "wan_hwaddr", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WAN interface MAC address */
	{ "wan_phytype", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	/* WAN TCP/IP parameters */
	{ "wan_proto", "dhcp", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* [static|dhcp|pppoe|pptp/l2tp|disabled] */
	{ "wan_nat_x", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_dhcpenable_x", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* replace x_DHCPClient */
	{ "wan_ipaddr_x", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WAN IP address */
	{ "wan_ipaddr", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_netmask_x", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WAN netmask */
	{ "wan_gateway_x", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WAN gateway */
	{ "wan_gateway", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_dnsenable_x", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_dns1_x", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* x.x.x.x x.x.x.x ... */
	{ "wan_dns2_x", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_dns", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_wins", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* x.x.x.x x.x.x.x ... */
	{ "wan_heartbeat_x", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* VPN Server */
	{ "wan_hostname", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WAN hostname */
	{ "wan_hwaddr_x", "", CKN_STR17, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Cloned mac */
#ifdef TODO
	{ "wan_domain", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WAN domain name */
	{ "wan_lease", "86400", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WAN lease time in seconds */
#endif

	/* Auth parameters */
	{ "wan_auth_x", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WAN authentication type */

	/* PPPoE parameters */
	{ "wan_pppoe_username", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* PPP username */
	{ "wan_pppoe_passwd", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* PPP password */
	{ "wan_pppoe_idletime", "0", CKN_STR10, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* oleg patch */
	{ "wan_pppoe_mru", "1492", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Negotiate MRU to this value */
	{ "wan_pppoe_mtu", "1492", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Negotiate MTU to the smaller of this value or the peer MRU */
	{ "wan_pppoe_service", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* PPPoE service name */
	{ "wan_pppoe_ac", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* PPPoE access concentrator name */
	{ "wan_pppoe_hostuniq", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* PPPoE host-uniq */
	{ "wan_pppoe_options_x", "", CKN_STR255, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* oleg patch */
	{ "wan_pptp_options_x", "", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* oleg patch */
#ifdef RTCONFIG_DSL
	{ "wan_pppoe_auth", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wan_ppp_echo", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_ppp_echo_interval", "6", CKN_STR6, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_ppp_echo_failure", "10", CKN_STR6, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "dns_probe_host", "dns.msftncsi.com", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* host to resolve */
#ifdef RTCONFIG_IPV6
	{ "dns_probe_content", "131.107.255.255 fd3e:4f5a:5b81::1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* resolve target addr or wildcard */
#else
	{ "dns_probe_content", "131.107.255.255", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* resolve target addr or wildcard */
#endif
	{ "dns_delay_round", "2", CKN_STR6, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	/* Misc WAN parameters */
	{ "wan_desc", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* WAN connection description */
	{ "wan_upnp_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* upnp igd */
	{ "wan_pppoe_relay", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// VPN+DHCP, a sperated nvram to control this function
	{ "wan_vpndhcp", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// DHCP option
	{ "wan_clientid_type", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: MAC 1: Node-specific RFC4361 */
	{ "wan_clientid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_vendorid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// For miniupnpd, so far default value only
#if !(defined(HND_ROUTER) && defined(RTCONFIG_HNDMFG))
	{ "upnp_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "upnp_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "upnp_secure", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "upnp_port", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "upnp_ssdp_interval", "60", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "upnp_mnp", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "upnp_clean", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "upnp_clean_interval", "600", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "upnp_clean_threshold", "20", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#ifdef RTCONFIG_DUALWAN // RTCONFIG_DUALWAN
#if defined(RTCONFIG_INTERNAL_GOBI) || defined(RTCONFIG_WANPORT2)
	{ "wans_mode", "lb", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wans_mode", "fo", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* off/failover/failback/loadbance(off/fo/fb/lb) */
#endif
#ifdef RTCONFIG_DSL
	{ "wans_dualwan", "dsl " DEF_SECOND_WANIF, CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wans_dualwan", "wan " DEF_SECOND_WANIF, CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wans_standby", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wans_lanport", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wans_lb_ratio", "3:1", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* only support two wan simultaneously */
	{ "wans_routing_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wans_routing_rulelist", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "wandog_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wandog_target", "", CKN_STR100, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_INTERNAL_GOBI
	{ "wandog_interval", "3", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Be the same with lteled's interval. */
#else
	{ "wandog_interval", "5", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wandog_maxfail", "12", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wandog_delay", "0", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wandog_fb_count", "4", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif // RTCONFIG_DUALWAN

#ifdef RTCONFIG_DSL
	{ "dslx_modulation", "5", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* multiple mode */
	{ "dslx_snrm_offset", "0", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Paul add 2012/9/24 */
	{ "dslx_sra", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Paul add 2012/10/15 */
	{ "dslx_bitswap", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Paul add 2013/10/23 */
	{ "dslx_adsl_rx_agc", "Default", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Renjie add 2014/12/23 */
	{ "dslx_adsl_esnp", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enhanced Sudden Noise Protection */
#ifdef RTCONFIG_DSL_ANNEX_B //Paul add 2012/8/21
	{ "dslx_annex", "6", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Annex BJM (EnumAdslTypeB_J_M) */
#else
	{ "dslx_testlab", "disable", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Country-Specific Setting for AU or GB */
	{ "dslx_annex", "4", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Annex AIJLM(EnumAdslTypeA_I_J_L_M) */
#endif
	{ "dslx_ginp", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dslx_ginp_try_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_ginp_try_enable_disp", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "dslx_dla_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_diag_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_diag_duration", "0", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_diag_state", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_diag_end_uptime", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_VDSL
	{ "dslx_vdsl_bitswap", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_vdsl_vectoring", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_vdsl_nonstd_vectoring", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_vdsl_target_snrm", "32767", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_vdsl_tx_gain_off", "32767", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_vdtxpwrtestmode", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_vdsl_rx_agc", "65535", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_vdsl_upbo", "auto", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_vdsl_esnp", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enhanced Sudden Noise Protection */
	{ "dslx_vdsl_profile", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_rmvlan", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

// Viz add dsl transmode
	{ "dslx_transmode", "atm", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* atm */
	{ "dsltmp_transmode", "atm", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* atm */

// the following variables suppose can be removed
	{ "dslx_nat", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_upnp_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_link_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_DHCPClient", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_dhcp_clientid_type", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: MAC 1: Node-specific RFC4361 */
	{ "dslx_dhcp_clientid", "", CKN_STR65, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Required by some ISP using RFC 1483 MER. */
	{ "dslx_dhcp_vendorid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* DHCP option 60 */
	{ "dslx_ipaddr", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IP address */
	{ "dslx_netmask", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* netmask */
	{ "dslx_gateway", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* gateway */
	{ "dslx_dnsenable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_dns1", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_dns2", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
// now use switch_stb_x
	{ "dslx_pppoe_auth", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_pppoe_username", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_pppoe_passwd", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	// this one is no longer to use
	//{ "dslx_pppoe_dial_on_demand", "" },
	{ "dslx_pppoe_idletime", "0", CKN_STR10, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_pppoe_mtu", "1492", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	// this one is no longer to use
//	{ "dslx_pppoe_mru", "" },
	{ "dslx_pppoe_service", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_pppoe_ac", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_pppoe_hostuniq", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_pppoe_options", "", CKN_STR255, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dslx_hwaddr", "", CKN_STR17, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//
	{ "dsl_unit", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl_subunit", "-1", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//
	{ "dsl_enable", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl_vpi", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl_vci", "", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl_encap", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	/* Paul modify 2012/8/6, set default Service Category to UBR without PCR, with PCR, SCR and MBS set to 0. */
	{ "dsl_svc_cat", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl_pcr", "0", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl_scr", "0", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl_mbs", "0", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dsl_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

// those PVC need to init first so that QIS internet/IPTV PVC setting could write to NVRAM
	{ "dsl0_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl0_vpi", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl0_vci", "", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl0_encap", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl0_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl0_svc_cat", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl0_pcr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl0_scr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl0_mbs", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dsl0_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl0_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl0_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
//
	{ "dsl1_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl1_vpi", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl1_vci", "", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl1_encap", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl1_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl1_svc_cat", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl1_pcr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl1_scr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl1_mbs", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dsl1_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl1_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl1_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
//
	{ "dsl2_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl2_vpi", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl2_vci", "", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl2_encap", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl2_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl2_svc_cat", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl2_pcr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl2_scr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl2_mbs", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dsl2_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl2_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl2_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
//
	{ "dsl3_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl3_vpi", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl3_vci", "", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl3_encap", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl3_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl3_svc_cat", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl3_pcr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl3_scr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl3_mbs", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dsl3_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl3_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl3_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
//
	{ "dsl4_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl4_vpi", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl4_vci", "", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl4_encap", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl4_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl4_svc_cat", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl4_pcr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl4_scr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl4_mbs", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dsl4_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl4_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl4_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
//
	{ "dsl5_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl5_vpi", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl5_vci", "", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl5_encap", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl5_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl5_svc_cat", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl5_pcr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl5_scr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl5_mbs", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dsl6_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
//
	{ "dsl6_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_vpi", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_vci", "", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_encap", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_svc_cat", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_pcr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_scr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_mbs", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dsl6_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl6_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
//
	{ "dsl7_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl7_vpi", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl7_vci", "", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl7_encap", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl7_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl7_svc_cat", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl7_pcr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl7_scr", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl7_mbs", "", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dsl7_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl7_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl7_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
// number of PVC , program generated
	{ "dslx_config_num", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
// for debug , program generated
	{ "dslx_debug", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#ifdef RTCONFIG_VDSL
	{ "dsl8_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.1_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.1_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.1_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.1_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.1_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.2_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.2_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.2_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.2_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.2_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.3_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.3_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.3_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.3_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.3_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.4_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.4_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.4_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.4_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.4_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.5_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.5_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.5_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.5_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.5_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.6_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.6_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.6_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.6_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.6_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.7_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.7_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.7_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.7_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl8.7_dot1p", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//
	{ "dsl9_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl9_proto", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl9_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsl9_vid", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#endif

	// NVRAM for start_firewall/start_qos
	// QOS
	// Maybe removed later
	/* Usage:
	 * 	qos0_YYY ==> WAN0, qos1_YYY ==> WAN1, qos2_YYY ==> WAN2, except
	 * 	qos_type and qos_enable.
	 */
#if defined(RTCONFIG_MULTIWAN_CFG)
	{ "qos_unit", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },		/* Last configured QoS */
#endif
	{ "qos_rulelist", "<Web Surf>>80>tcp>0~512>0<HTTPS>>443>tcp>0~512>0<File Transfer>>80>tcp>512~>3<File Transfer>>443>tcp>512~>3", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "qos_orates", "80-100,10-100,5-100,3-100,2-95,0-0,0-0,0-0,0-0,0-0", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"qos_irates", "100,100,100,100,100,0,0,0,0,0", CKN_STR512, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: off */
#ifdef RTCONFIG_BWDPI
	{ "qos_type", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: Traditional */
#else
	{ "qos_type", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: Traditional */
#endif
	{ "qos_method", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_sticky", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_ack", "on", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_syn", "on", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_fin", "off", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_rst", "off", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_icmp", "on", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_reset", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_obw", "", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_ibw", "", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_orules", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_burst0", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_burst1", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_default", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "qos_bw_rulelist", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// TriggerList
	{ "autofw_enable_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "autofw_rulelist", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// VSList
	{ "vts_enable_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vts_rulelist", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vts_upnplist", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vts_ftpport", "2021", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	//Game VSList
	{ "game_vts_rulelist", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// DMZ
	{ "dmz_ip", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sp_battle_ips", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// NVRAM for start_ddns
	{ "ddns_enable_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_server_x", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_username_x", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_passwd_x", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_hostname_x", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_wildcard_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_regular_check", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_regular_period", "60", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_transfer", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// NVRAM for start_firewall
	// Firewall
#ifdef RTCONFIG_PARENTALCTRL
	{ "MULTIFILTER_ALL", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "MULTIFILTER_ENABLE", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "MULTIFILTER_MAC", "", CKN_STR512, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "MULTIFILTER_DEVICENAME", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "MULTIFILTER_MACFILTER_DAYTIME", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "MULTIFILTER_URL_ENABLE", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "MULTIFILTER_URL", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "MULTIFILTER_TMP", "", CKN_STR_MAX, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#ifdef RTCONFIG_BWDPI
	// TrendMicro module
	{ "wrs_protect_enable",	"0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// AiProtection v2.0 total switch
	{ "wrs_mals_enable",	"1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// mals switch
	{ "wrs_cc_enable",	"1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// vp   switch
	{ "wrs_vp_enable",	"1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// cc   switch
	{ "wrs_enable",		"0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// web content filter
	{ "wrs_rulelist",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// web content filter : <switch1>MAC1>Adult1>...<switch2>MAC2>Adult2>...
	{ "wrs_app_enable",	"0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// app patrol
	{ "wrs_app_rulelist",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// app patrol : <switch1>MAC1>Commercial1>...<switch2>MAC2>Commercial2>...
	{ "wrs_mail_bit",	"7", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// alert mail option, default : mals + vp + cc
	{ "wrs_mals_t",		"0", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// mals timestamp for query
	{ "wrs_cc_t",		"0", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// cc   timestamp for query
	{ "wrs_vp_t",		"0", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// vp   timestamp for query
	{ "bwdpi_db_enable",	"0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// traffic analyzer switch
	{ "bwdpi_rsa_check",	"0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },// signature update check
	{ "bwdpi_alive",	"10", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },// check dpi engine alive timeout
	{ "bwdpi_app_rulelist", "9,20<8<4<0,5,6,15,17<13,24<1,3,14<7,10,11,21,23<<", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "bwdpi_sig_ver",	"", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },// dpi engine signature version
	{ "TM_EULA",		"0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// EULA of Trend Micro
	{ "apps_analysis",	"0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// Apps Analysis in bandwidth monitor (Adaptive QoS first page)
	{ "bwdpi_wh_enable",	"0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// web history
	{ "sig_update_t",	"0", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	// signature upgrade timestamp
#endif
#endif	/* RTCONFIG_PARENTALCTRL */
#ifdef RTCONFIG_YANDEXDNS
	{ "yadns_enable_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: Disable */
	{ "yadns_mode", "0", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: Basic */
	{ "yadns_rulelist", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* List client modes <devname>hh:ww:aa:dd:dd:rr>mode>enable... */
#endif /* RTCONFIG_YANDEXDNS */
	{ "fw_enable_x", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fw_dos_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fw_log_x", "none", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fw_pt_pptp", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fw_pt_l2tp", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fw_pt_ipsec", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fw_pt_rtsp", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fw_pt_h323", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fw_pt_sip", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fw_pt_pppoerelay", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_DUALWAN)
	{ "pppoerelay_unit", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Select WAN unit if dualwan + load-balanced. */
#endif
	{ "misc_http_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "misc_httpport_x", "8080", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_HTTPS
	{ "misc_httpsport_x", "8443", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "misc_ping_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "misc_lpr_x", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// UrlList
	{ "url_enable_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "url_mode_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "url_date_x", "1111111", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "url_time_x", "00002359", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "url_enable_x_1", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "url_time_x_1", "00002359", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "url_rulelist", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// KeywordList
	{ "keyword_enable_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "keyword_date_x", "1111111", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "keyword_time_x", "00002359", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "keyword_rulelist", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// LWFilterListi
	{ "fw_lw_enable_x", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "filter_lw_date_x", "1111111", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "filter_lw_time_x", "00002359", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "filter_lw_time2_x", "00002359", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "filter_lw_default_x", "ACCEPT", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "filter_lw_icmp_x", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "filter_lwlist", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// NVRAM for start_usb
	{ "usb_enable", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_USB_XHCI
#if defined(MAPAC1300) || defined(MAPAC2200)
/* MODEL DON'T USB USB3 but NEED XHCI */
#else
#ifdef RTAC87U
	{ "usb_usb3", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "usb_usb3", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif	/* MODEL DON'T USB USB3 but NEED XHCI */
#ifdef RTCONFIG_XHCIMODE
	{ "xhcimode_waitsec", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
	{ "usb_usb2", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftpenable_x", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftpanonymous_x", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftpsuper_x", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftpport_x", "21", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftpmax_x", "12", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftptimeout_x", "120", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftpstaytimeout_x", "240", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftpscript_x", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftpnum_x", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_bannum_x", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	#ifndef MR
	{ "qos_rulenum_x", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	#endif
	{ "usb_ftpusername_x", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftppasswd_x", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftpmaxuser_x", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftprights_x", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ftpbanip_x", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	// remain default setting control as tomato
	{ "usb_enable", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_uhci", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_INTERNAL_GOBI
	{ "usb_gobi", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#if defined(RTCONFIG_INTERNAL_GOBI) && !defined(RTCONFIG_USB_MULTIMODEM)
	{ "usb_ohci", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "usb_ohci", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "usb_usb2", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_irq_thresh", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_storage", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_printer", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ext_opt", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_fat_opt", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_ntfs_opt", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_hfs_opt", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_fs_ext3", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_fs_fat", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_NTFS
	{ "usb_fs_ntfs", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_fs_ntfs_sparse", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_HFS
	{ "usb_fs_hfs", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "usb_automount", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "script_usbhotplug", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "script_usbmount", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "script_usbumount", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "smbd_enable", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//	{ "smbd_autoshare", "1" },
	{ "smbd_cpage", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "smbd_cset", "utf8", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "smbd_custom", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//	{ "smbd_master", "1" },
//	{ "smbd_passwd", "" },
//	{ "smbd_shares", "share</mnt<Default Share<1<0>root$</Hidden Root<0<1" },
	{ "smbd_user", "nas", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//	{ "smbd_wgroup", "WORKGROUP" },
	{ "smbd_wins", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "enable_samba_tuxera", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "log_ipaddr", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "log_port", "514", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "log_size", "256", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "log_level", "6", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* < LOG_INFO */
	{ "console_loglevel", "5", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* < KERN_NOTICE */

#if defined(RTCONFIG_JFFS2) || defined(RTCONFIG_BRCM_NAND_JFFS2)
	{ "jffs2_on", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "jffs2_exec", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_UBIFS
	{ "ubifs_on", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ubifs_exec", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_REBOOT_SCHEDULE
	{ "reboot_schedule_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "reboot_schedule", "00000000000", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_USB
	{ "acc_num", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "acc_list", "admin>admin", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "st_samba_mode", "4", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_FTP
	{ "st_ftp_mode", "2", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "enable_ftp", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ftp_lang", "EN", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "enable_samba", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "st_max_user", "5", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "computer_name", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "st_samba_workgroup", "WORKGROUP", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

//#ifdef RTCONFIG_WEBDAV
	{ "enable_webdav", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: Disable */
	{ "st_webdav_mode", "2", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: http */
	{ "webdav_proxy", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "webdav_aidisk", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "webdav_http_port", "8082", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "webdav_https_port", "443", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "acc_webdavproxy", "admin>1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: Only show USBDisk */
	{ "enable_webdav_captcha", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disable */
	{ "enable_webdav_lock", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disable */
	{ "webdav_acc_lock", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: unlock account */
	{ "webdav_lock_interval", "2", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "webdav_lock_times", "3", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "webdav_last_login_info", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//#endif

//#ifdef RTCONFIG_CLOUDSYNC
	{ "enable_cloudsync", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "cloud_sync", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "share_link_param", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "share_link_result", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "share_link_host", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//#endif

#ifdef RTCONFIG_CAPTIVE_PORTAL
	{ "captive_portal",			"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_2g",			"", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_5g",			"", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_5g_2",		"", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_2g_if",		"off", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_5g_if",		"off", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_5g_2_if",		"off", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_2g_if_temp",		"off", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_5g_if_temp",		"off", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_5g_2_if_temp",	"off", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_enable",		"off", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_adv_enable",		"off", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_adv_idle_timeout",		"300", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_adv_profile",		"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_adv_local_clientlist",	"", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_adv_wl_en",	"", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_adv_wl_dis",	"", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_adv_2g_ssid",			"", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_adv_5g_ssid",			"", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "captive_portal_adv_5g_2_ssid",		"", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_COOVACHILLI
	{"chilli_net", "192.168.182.0/24", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_CPFREE
	{"chilli_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_HK_CPFREE
	{"chilli_radius", "cpfree.hk", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{"chilli_radius", "cpfree.com", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{"chilli_backup", "aroundwifi.com", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_HK_CPFREE
	{"chilli_url", "http://www.cpfree.hk/loginIndex.action", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{"chilli_url", "http://www.cpfree.com/loginIndex.action", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{"chilli_pass", "cp27nlaz09s8a", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_uamsecret", "ua8k69fo92n", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{"chilli_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_radius", "127.0.0.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_backup", "127.0.0.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_url", "https://192.168.1.1/Uam", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_pass", "radius", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_uamsecret", "chillispot", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{"chilli_nowifibridge", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_dns1", "8.8.8.8", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_interface", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_radiusnasid", "radius", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_uamanydns", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_uamallowed", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_macauth", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_macpasswd", "password", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_802.1Xauth", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_additional", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_protocol", "http", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_authport", "http", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_awaytime", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_authtime", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_authtype", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_sessiontime", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"chilli_bandwidth", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	/* for captive portal */
	{"cp_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_interface", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_sessiontime", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_awaytime", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_authtime", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_authtype", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_radius", "127.0.0.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_backup", "127.0.0.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_radiusport", "1812", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_radiusauthport", "1812", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_radiusnasid", "radius", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_dns1", "8.8.8.8", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_uamsecret", "chillispot", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_radiussecret", "radius", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_uamanydns", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_uamallowed", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_nowifibridge", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_net", "192.168.183.0/24", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_macauth", "0", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_macpasswd", "password", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_802.1Xauth", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_additional", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_protocol", "http", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"cp_authport", "http", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	/* for hotspotsystem nvram*/
	{"hotss_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_uamenable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_loginonsplash", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_customsplash", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_uamallowed", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_operatorid", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_locationid", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_interface", "eth1", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_net", "192.168.182.0/24", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_customuam", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_customuamproto", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_remotekey", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"hotss_nowifibridge", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif // RTCONFIG_COOVACHILLI
#ifdef RTCONFIG_FREERADIUS
	{"radius_serv_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{"radius_serv_list", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	//NVRAM for fbwifi
#ifdef RTCONFIG_FBWIFI
	{ "fbwifi_enable", "off", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_2g", "off", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_5g", "off", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined (RTAC3200)
	{ "fbwifi_5g_2", "off", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_ssid_2_temp", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_auth_mode_x_2_temp", "open", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_crypto_2_temp", "aes", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_wpa_psk_2_temp", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_macmode_2_temp", "disabled", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_lanaccess_2_temp", "off", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_expire_2_temp", "0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_5g_2_temp", "off", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "fbwifi_secret", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_id", "off", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_ssid", "ASUS-FBWiFi", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_auth_mode_x", "open", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_crypto", "aes", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_wpa_psk", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_ssid_0_temp", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_auth_mode_x_0_temp", "open", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_crypto_x_0_temp", "aes", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_wpa_psk_0_temp", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_macmode_0_temp", "disabled", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_lanaccess_0_temp", "off", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_expire_0_temp", "0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_ssid_1_temp", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_auth_mode_x_1_temp", "open", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_crypto_1_temp", "aes", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_wpa_psk_1_temp", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_macmode_1_temp", "disabled", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_lanaccess_1_temp", "off", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_expire_1_temp", "0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_2g_temp", "off", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fbwifi_5g_temp", "off", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_DISK_MONITOR
	{ "diskmon_policy", "disk", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* all */
	{ "diskmon_part", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* sda1 */
	{ "diskmon_force_stop", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disable */

	{ "diskmon_usbport", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 1 */
	{ "diskmon_freq", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disable */
	{ "diskmon_freq_time", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* DAY>WEEK>HOUR */
	{ "usb_path1_diskmon_freq", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_diskmon_freq_time", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_diskmon_freq", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_diskmon_freq_time", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if !defined(RT4GAC55U) && !defined(RT4GAC68U)
	{ "usb_path3_diskmon_freq", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_diskmon_freq_time", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "diskformat_file_system", "tfat", CKN_STR10, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 }, //tfat, tntfs, thfsplus
	{ "diskformat_label", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifndef RTCONFIG_BCMARM
	{ "diskremove_bad_device", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disable */
#else
	{ "diskremove_bad_device", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
#endif

#ifdef RTCONFIG_MEDIA_SERVER
	{ "dms_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_rebuild", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_port", "8200", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_dbdir", "/var/cache/minidlna", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_dir", "/mnt", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_tivo", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_stdlna", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_sas", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_dir_x", "</mnt", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_dir_type_x", "<AVP", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_dir_manual", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_friendly_name", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "daapd_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "daapd_friendly_name", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef DM
	{ "apps_dl", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_dl_share", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_dl_seed", "24", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_dms", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_caps", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_comp", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_pool", "harddisk/part0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_share", "share", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_ver", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_seed", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_upload_max", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "machine_name", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "apps_dms_usb_port", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_running", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_dl_share_port_from", "10001", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_dl_share_port_to", "10050", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "apps_installed", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "sh_num", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#if !defined(RTCONFIG_BCMARM) && !defined(RTCONFIG_QCA)
	{ "apps_ipkg_old", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "apps_ipkg_old", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "apps_ipkg_server", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_wget_timeout", "30", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_local_space", "/rom", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined (RTAC1200) || defined (RTAC1200GA1) || defined (RTAC1200GU) || defined(RTAC85U)
	{ "apps_swap_enable", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_swap_size", "67000", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	
#else
	{ "apps_swap_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_swap_size", "33000", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 32M */
#endif
	{ "apps_swap_threshold", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 72M */
	{ "apps_swap_file", ".swap", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_state_autofix", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "http_username", "admin", CKN_STR20, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "http_passwd", "admin", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "http_autologout", "30", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "http_client", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "http_clientlist", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "http_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: http */
/*	{ "http_lanport", "80" }, */
#ifdef RTCONFIG_HTTPS
	{ "https_lanport", "8443", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "https_crt_file", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "https_crt_save", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "https_crt_gen", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "https_crt_cn", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "webs_last_info", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "firmware_path", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

	{ "temp_lang", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wollist", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "httpd_die_reboot", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "x_Setting", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* is any setting set */
	{ "r_Setting", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* is repeater set */
	{ "w_Setting", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* is wilreess set */
	{ "ui_Setting", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* for Language change in QIS */
#if defined(MAPAC1300) || defined(MAPAC2200)
	{ "qis_Setting", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* QIS finish */
	{ "hive_ui", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* for HIVE UI debug */
#endif
	{ "asus_mfg", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "asus_mfg_flash", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "btn_rst", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "btn_ez", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "btn_ez_radiotoggle", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Turn WPS into radio toggle */
	{ "btn_ez_mode", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Turn WPS into led toggle. 0: wps */
	//TODO: maybe
	//1: led. led on/off
	//2: radio. Wifi radio on/off (btn_ez_radiotoggle=1) ?
	//3: reset to default (RTCONFIG_WPS_RST_BTN: RT-N11P) ?
#ifdef RTCONFIG_WIFI_TOG_BTN
	{ "btn_wifi_toggle", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_LED_BTN
	{ "btn_led", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "btn_led_mode", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTAC87U
	{ "LED_switch_count", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
#if defined(RTCONFIG_SWMODE_SWITCH)
#if defined(PLAC66U)
	{ "switch_mode", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif

	 /* APCLI/STA parameters */
	#if 0
	{ "sta_ssid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_bssid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_auth_mode", "open", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_wep_x", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_crypto", "tkip", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_wpa_psk", "12345678", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_key", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_key_type", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_key1", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_key2", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_key3", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_key4", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_check_ha", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_authorized", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sta_connected", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	#endif

	{ "record_lanaddr", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "telnetd_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_SSH
	{ "sshd_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disabled 1: enabled 2: enabled for LAN only */
	{ "sshd_port", "22", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sshd_pass", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sshd_authkeys", "", CKN_STR2999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_POWER_SAVE
	{ "pwrsave_mode", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 1: auto (ondemand), 2: power save, otherwise: performance */
#endif
	{ "enable_acc_restriction", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "restrict_rulelist", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#ifdef RTCONFIG_USB_MODEM
	{ "usb_qmi", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Dev3G", "AUTO", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_android", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disabled */
	{ "modem_autoapn", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disabled */
	{ "modem_roaming", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disabled */
	{ "modem_roaming_mode", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: automatically */
	{ "modem_roaming_isp", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* operator at the long format. */
	{ "modem_roaming_imsi", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* MCC+MNC */
	{ "modem_roaming_scantime", "120", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* second. */
	{ "modem_roaming_scanlist", "/tmp/cops.txt", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* station lists. */
	{ "modem_pincode", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_country", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_pdp", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: PDP-IP(IPv4) */
	{ "modem_isp", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_spn", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_apn", "internet", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_dialnum", "*99#", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_authmode", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_user", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_pass", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_apn_v6", "internet6", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_authmode_v6", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_user_v6", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_pass_v6", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_ttlsid", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_mode", "0", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: auto */
	{ "modem_lte_band", "auto", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* auto */
	{ "modem_running", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_USB_MODEM_PIN
	{ "modem_pincode_opt", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "modem_sim_num", "10", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_limit_unit", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: GBytes */
	{ "modem_warning_unit", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: GBytes */
	{ "modem_sms_limit", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disable */
	{ "modem_sms_phone", "", CKN_STR12, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_sms_message1", "This is a alert about the data usage is over:", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_sms_message2", "The data usage has reached the limit:", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_reg_time", "10", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_JFFS2) || defined(RTCONFIG_BRCM_NAND_JFFS2) || defined(RTCONFIG_UBIFS)
	{ "modem_bytes_data_cycle", "31", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_bytes_data_limit", "0", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disabled */
	{ "modem_bytes_data_warning", "0", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disabled */
	{ "modem_bytes_data_save", "1800", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* the interval to save the data usage. need to be 30 times. */
#endif
	{ "modem_mtu", "0", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: auto */
	/* Cherry added temporarily or mobile broadband web implementation in 2014/8/20.*/
	{ "data_usage", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "data_usage_cycle", "30", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "data_usage_limit", "8", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "data_usage_warning", "6", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "modem_idletime", "600", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_MODEM_BRIDGE
	{ "modem_bridge", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: disabled */
#endif
#endif
#ifdef RT4GAC68U
	{ "stop_atlock", "0" },
#endif

	{ "udpxy_enable_x", "0", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "udpxy_clients", "10", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	/* traffic monitor - added by jerry5 2009/07 */
	{ "rstats_enable", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "rstats_path", "*nvram", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "rstats_new", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "rstats_stime", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "rstats_offset", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "rstats_data", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "rstats_colors", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "rstats_exclude", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "rstats_sshut", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "rstats_bak", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "http_id", "TIDe855a6487043d70a", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "env_path", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if 0
	{ "debug_logeval", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "debug_cprintf", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "debug_cprintf_file", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "debug_logrc", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "debug_ovrc", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "debug_abrst", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#elif defined(HND_ROUTER)
#if (defined(HND_ROUTER) && defined(RTCONFIG_HNDMFG)) || defined(RTAC86U) || defined(AC2900)
	{ "debug_cprintf", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "debug_cprintf", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif
	{ "upnp_min_port_int", "1024", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "upnp_max_port_int", "65535", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "upnp_min_port_ext", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "upnp_max_port_ext", "65535", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "mfp_ip_monopoly", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_PPTPD) || defined(RTCONFIG_ACCEL_PPTPD)
	{ "pptpd_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_broadcast", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_ms_network", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_chap", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0/1/2(Auto/MS-CHAPv1/MS-CHAPv2) */
	{ "pptpd_mppe", "13", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 1|4|8(MPPE-128|MPPE-40|No Encryption) */
	{ "pptpd_dns1", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_dns2", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_wins1", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_wins2", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_server", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_clients", "192.168.10.2-11", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_clientlist", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_mru", "1450", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_mtu", "1450", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pptpd_sr_rulelist", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#if defined(RTCONFIG_VPNC)
	{ "vpnc_proto", "disable", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_pppoe_username", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_pppoe_passwd", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_heartbeat_x", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_dnsenable_x", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_pppoe_options_x", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_pptp_options_x", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_state_t", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_sbstate_t", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_clientlist", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_connect_row", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_auto_conn", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_pptp_options_x_list", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#if defined(RTCONFIG_VPNC)
	{ "vpnc_clientlist_ex", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_default_wan", "0", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },		
	{ "vpnc_default_wan_tmp", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },		
	{ "vpnc_unit", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_dev_policy_list", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpnc_policy_unit", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#if defined(RTCONFIG_PPTPD) || defined(RTCONFIG_ACCEL_PPTPD) || defined(RTCONFIG_OPENVPN)
	{ "VPNServer_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "VPNServer_mode", "pptpd", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "VPNClient_rule", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#if defined(RTCONFIG_VPNC) || defined(RTCONFIG_OPENVPN)
	{ "VPNClient_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "VPNClient_rule", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_OPENVPN
	{ "vpn_upload_state", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_upload_unit", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_upload_type", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	// openvpn
	{ "vpn_debug", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_loglevel", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_unit", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_serverx_eas", "1, ", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT },	/* 0 */
	{ "vpn_serverx_dns", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_serverx_clientlist", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "vpn_server_poll", "0", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_if", "tun", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_proto", "udp", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_port", "1194", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_firewall", "auto", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_crypt", "tls", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_comp", "adaptive", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_cipher", "AES-128-CBC", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_dhcp", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_r1", "192.168.1.50", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_r2", "192.168.1.55", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_sn", "10.8.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_nm", "255.255.255.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_local", "10.8.0.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_remote", "10.8.0.2", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_reneg", "-1", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_hmac", "-1", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_plan", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_ccd", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_c2c", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_ccd_excl", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_ccd_val", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_pdns", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_rgw", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_custom", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_igncrt", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_verb", "3", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server_tls_keysize", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server_static", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server_ca", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server_ca_key", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server_crt", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server_key", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server_client_crt", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server_client_key", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server_dh", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server_crl", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server1_static", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server1_ca", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server1_ca_key", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server1_crt", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server1_key", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server1_client_crt", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server1_client_key", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server1_dh", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_server1_crl", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "vpn_client_unit", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_clientx_eas", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "vpn_client_poll", "0", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_if", "tun", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_bridge", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_nat", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_proto", "udp", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_addr", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_port", "1194", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_retry", "30", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_rg", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_firewall", "auto", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_crypt", "tls", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_comp", "adaptive", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_cipher", "default", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_local", "10.8.0.2", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_remote", "10.8.0.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_nm", "255.255.255.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_reneg", "-1", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_hmac", "-1", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_adns", "0", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_rgw", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_gw", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_cn", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_tlsremote", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_custom", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_userauth", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_username", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_password", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_useronly", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client_verb", "3", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client_static", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client_ca", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client_crt", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client_key", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client_crl", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client1_static", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client1_ca", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client1_crt", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client1_key", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client1_crl", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client2_static", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client2_ca", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client2_crt", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client2_key", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client2_crl", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client3_static", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client3_ca", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client3_crt", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client3_key", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client3_crl", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client4_static", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client4_ca", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client4_crt", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client4_key", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client4_crl", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client5_static", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client5_ca", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client5_crt", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client5_key", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_crt_client5_crl", "", CKN_STR3999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "vpn_client1_username", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client1_password", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client2_username", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client2_password", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client3_username", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client3_password", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client4_username", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client4_password", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client5_username", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client5_password", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_NOTIFICATION_CENTER
	/* nt_center */
	{ "nc_setting_conf", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "nc_web_app_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "nc_mail_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	/* actMail */
	{ "NOTIFY_MAIL_TO", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#if defined(RTCONFIG_PUSH_EMAIL) || defined(RTCONFIG_NOTIFICATION_CENTER)
	{ "PM_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable Push Mail feature. */
	{ "PM_type", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Send the tracking to the of xDSL team's official gmail. */
	{ "PM_SMTP_SERVER", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_SMTP_PORT", "", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_MY_NAME", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_MY_EMAIL", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_USE_TLS", "true", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_SMTP_AUTH", "LOGIN", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_SMTP_AUTH_USER", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_SMTP_AUTH_PASS", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_MAIL_SUBJECT", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_LETTER_CONTENT", "", CKN_STR512, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_LETTER_PATH", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_MAIL_FILE", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_MAIL_TARGET", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_title", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* The title of mail. */
	{ "PM_target", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* The address of Mail Server. */
	{ "PM_restart", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* reset the Push Mail Service. */
	{ "PM_freq", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0:daily */
	{ "PM_mon", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* months since January (0 to 11). */
	{ "PM_day", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* days since Sunday (0 to 6 Sunday=0). */
	{ "PM_hour", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* hours since midnight (0 to 23). */
	{ "pushnotify_httplogin", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pushnotify_diskmonitor", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_attach_syslog", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_attach_cfgfile", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_attach_modemlog", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fb_country", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fb_email", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fb_comment", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fb_email_dbg", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* send to email address */
	{ "fb_state", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fb_browserInfo", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL
	// for web ui udentify , 0=notificaton, 1=close
	{ "fb_experience", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fb_ISP", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fb_Subscribed_Info", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "PM_attach_iptables", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fb_availability", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ewan_dot1q", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ewan_vid", "0", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ewan_dot1p", "0", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "fb_ptype", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fb_pdesc", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif /* RTCONFIG_DSL */
#endif /* RTCONFIG_PUSH_EMAIL */

#ifdef __CONFIG_NORTON__
	{ "nga_lickey", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "nga_user", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "nga_pass", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "nga_devname", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "nga_ncw", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "nga_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "nga_gact", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "NGA_MID", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ngap_0", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ngap_1", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif /* __CONFIG_NORTON__ */

#ifdef RTCONFIG_IPV6
	// IPv6 parameters
	{ "ipv6_service", "disabled", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* disabled/ipv6pt/dhcp6/6in4/6to4/6rd/other */
	{ "ipv6_ifdev", "ppp", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv6_prefix", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* The global-scope IPv6 prefix to route/advertise */
	{ "ipv6_prefix_length", "64", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* The bit length of the prefix. Used by dhcp6c. For radvd */
	{ "ipv6_dhcp_pd", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable DHCP-PD (DHCP Prefix Delegation) by default */
	{ "ipv6_rtr_addr", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* defaults to $ipv6_prefix::1 */
	{ "ipv6_prefix_len_wan", "64", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* used in ipv6_service other */
	{ "ipv6_ipaddr", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* used in ipv6_service other */
	{ "ipv6_gateway", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* used in ipv6_service other */
	{ "ipv6_radvd", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable Router Advertisement (radvd) */
	{ "ipv6_relay", "192.88.99.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IPv6 Anycast Address */
	{ "ipv6_tun_v4end", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Foreign IPv4 endpoint of SIT tunnel */
	{ "ipv6_tun_addr", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IPv6 address to assign to local tunnel endpoint */
	{ "ipv6_tun_addrlen", "64", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* CIDR prefix length for tunnel's IPv6 address */
	{ "ipv6_tun_peer", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IPv6 address of remote tunnel endpoint */
	{ "ipv6_tun_mtu", "0", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Tunnel MTU */
	{ "ipv6_tun_ttl", "255", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Tunnel TTL */
	{ "ipv6_6rd_dhcp", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv6_6rd_router", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv6_6rd_ip4size", "0", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv6_6rd_prefix", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv6_6rd_prefixlen", "32", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if 0
	{ "ipv6_dns", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* DNS server(s) IPs */
#else
	{ "ipv6_dns1", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv6_dns2", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv6_dns3", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "ipv6_get_dns", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* DNS IP address which got by dhcp6c */
	{ "ipv6_dnsenable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv6_get_domain", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* domain name which got by dhcp6c */
	{ "ipv6_debug", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv6_autoconf_type", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IPv6 LAN auconfiguration type */
	{ "ipv6_dhcp_lifetime", "86400", CKN_STR6, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* valid lifetimes of IPv6 prefix in seconds */
	{ "ipv6_dhcp_start", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* start of address pool */
	{ "ipv6_dhcp_end", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* end of address pool */
	{ "ipv6_fw_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Default FORWARD table to DROP packets */
	{ "ipv6_fw_rulelist", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IPv6 allowed forward rules */
	{ "ipv6_prefix_s", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* for ipv6 6in4 */
	{ "ipv6_prefix_length_s", "64", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* for ipv6 6in4/other */
	{ "ipv6_rtr_addr_s", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* for ipv6 other */
#endif

#if defined(RTCONFIG_IPV6) && defined(RTCONFIG_DUALWAN)
	// IPv6 parameters
	{ "ipv61_service", "disabled", CKN_STR16, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* disabled/dhcp6/ipv6pt/6in4/6to4/6rd/other */
	{ "ipv61_ifdev", "ppp", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv61_prefix", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* The global-scope IPv6 prefix to route/advertise */
	{ "ipv61_prefix_length", "64", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* The bit length of the prefix. Used by dhcp6c. For radvd */
	{ "ipv61_dhcp_pd", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable DHCP-PD (DHCP Prefix Delegation) by default */
	{ "ipv61_rtr_addr", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* defaults to $ipv6_prefix::1 */
	{ "ipv61_prefix_len_wan", "64", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* used in ipv6_service other */
	{ "ipv61_ipaddr", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* used in ipv6_service other */
	{ "ipv61_gateway", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* used in ipv6_service other */
	{ "ipv61_radvd", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Enable Router Advertisement (radvd) */
	{ "ipv61_relay", "192.88.99.1", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IPv6 Anycast Address */
	{ "ipv61_tun_v4end", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Foreign IPv4 endpoint of SIT tunnel */
	{ "ipv61_tun_addr", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IPv6 address to assign to local tunnel endpoint */
	{ "ipv61_tun_addrlen", "64", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* CIDR prefix length for tunnel's IPv6 address */
	{ "ipv61_tun_peer", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IPv6 address of remote tunnel endpoint */
	{ "ipv61_tun_mtu", "0", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Tunnel MTU */
	{ "ipv61_tun_ttl", "255", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Tunnel TTL */
	{ "ipv61_6rd_dhcp", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv61_6rd_router", "0.0.0.0", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv61_6rd_ip4size", "0", CKN_STR2, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv61_6rd_prefix", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv61_6rd_prefixlen", "32", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if 0
	{ "ipv61_dns", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* DNS server(s) IPs */
#else
	{ "ipv61_dns1", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv61_dns2", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv61_dns3", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "ipv61_get_dns", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* DNS IP address which got by dhcp6c */
	{ "ipv61_dnsenable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv61_get_domain", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* domain name which got by dhcp6c */
	{ "ipv61_autoconf_type", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* IPv6 LAN auconfiguration type */
	{ "ipv61_dhcp_lifetime", "86400", CKN_STR6, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* valid lifetimes of IPv6 prefix in seconds */
	{ "ipv61_dhcp_start", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* start of address pool */
	{ "ipv61_dhcp_end", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* end of address pool */
	{ "ipv61_prefix_s", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* for ipv6 6in4 */
	{ "ipv61_prefix_length_s", "64", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* for ipv6 6in4/other */
	{ "ipv61_rtr_addr_s", "", CKN_STR39, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* for ipv6 other */
#endif

	{ "web_redirect", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Only NOLINK is redirected in default */
	{ "disiosdet", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "nat_redirect_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#ifdef RTCONFIG_FANCTRL
	{ "fanctrl_dutycycle", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_SHP
	{ "lfp_disable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_ISP_METER
	{ "isp_meter", "disable", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "isp_limit", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "isp_day_rx", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "isp_day_tx", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "isp_month_rx", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "isp_month_tx", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "isp_limit_time", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "isp_connect_time", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "Ate_version", "1.0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Ate_power_on_off_ver", "2.4", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Ate_power_on_off_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Ate_reboot_count", "100", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Ate_rc_check", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Ate_dev_check", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Ate_boot_check", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Ate_total_fail_check", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Ate_dev_fail", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Ate_boot_fail", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Ate_total_fail", "10", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Ate_continue_fail", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dev_fail_reboot", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef HND_ROUTER
	{ "Ate_check_delay", "3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0},
#else
	{ "Ate_check_delay", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0},
#endif
	// Wireless parameters

#ifdef RTCONFIG_SNMPD
	{ "snmpd_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sysName", "", CKN_STR256, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sysContact", "", CKN_STR256, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "sysLocation", "", CKN_STR256, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "roCommunity", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "rwCommunity", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "v3_auth_type", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "v3_priv_type", "", CKN_STR4, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "v3_auth_passwd", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "v3_priv_passwd", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_TIMEMACHINE
	{ "timemachine_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tm_device_name", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tm_vol_size", "0", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tm_partition_num", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tm_ui_setting", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tm_usb_path_vid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tm_usb_path_pid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tm_usb_path_serial", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tm_debug", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#if defined(RTCONFIG_TR069)
	{ "tr_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_discovery", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_inform_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_inform_interval", "86400", CKN_STR10, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_acs_url", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_username", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_passwd", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_conn_username", "admin", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_conn_passwd", "admin", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_conn_port", "7547", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_ca_cert", "", CKN_STR2999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_client_cert", "", CKN_STR2999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_client_key", "", CKN_STR2999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tr_client_key", "", CKN_STR2999, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "pvgcode", "ASUS", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "Ate_fw_fail", "10", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_SOC_IPQ40XX)
	{ "Ate_reboot_delay", "20", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "Ate_reboot_delay", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#if (defined(RTCONFIG_USER_LOW_RSSI) || defined(RTCONFIG_NEW_USER_LOW_RSSI))
	{ "wl_user_rssi", "0", CKN_STR3, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* disabled by default */
#ifndef RTCONFIG_BCMARM
	{ "wl_lrc", "2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "rast_idlrt", "2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* roaming assistant idle rate (Kbps) */
#endif
	{ "custom_clientlist", "", CKN_STR512, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* for customize device name */
	{ "nmp_client_list", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ttl_inc_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* enable TTL increment */
	{ "ttl_spoof_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* enable TTL spoofing */
#ifdef RTCONFIG_TOR
	{ "Tor_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* enable Tor Transparent Proxy */
	{ "Tor_socksport", "9050", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Tor_transport", "9040", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Tor_dnsport", "9053", CKN_STR5, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "Tor_redir_list", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_JFFS2USERICON
	{ "custom_usericon", "", CKN_STR_MAX, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "custom_usericon_del", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_TRAFFIC_LIMITER
	{ "tl_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tl_cycle", "1", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tl_date_start", "", CKN_STR512, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tl0_alert_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tl0_alert_max", "40", CKN_STR7, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tl0_limit_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tl0_limit_max", "45", CKN_STR7, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tl1_alert_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tl1_alert_max", "40", CKN_STR7, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tl1_limit_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "tl1_limit_max", "45", CKN_STR7, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_CLOUDCHECK
	{ "enable_cloudcheck", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_RESTRICT_GUI
	{ "fw_restrict_gui", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "fw_restrict_gui_mac", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_KEY_GUARD
	{ "kg_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "kg_wan_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "kg_powersaving_enable", "1", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "kg_wl_radio_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "kg_wl_radio", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "kg_device_enable", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "kg_devicename", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "kg_mac", "", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_WTFAST
	{ "wtf_username", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wtf_passwd", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wtf_rulelist", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wtf_enable_games", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "guard_mode", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_IPSEC
	{ "ipsec_profile_item",		"", CKN_STR256, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_profile_1",		"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_profile_2",		"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_profile_3",		"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_profile_4",		"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_profile_5",		"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_client_list_1",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_client_list_2",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_client_list_3",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_client_list_4",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_client_list_5",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_profile_client_1",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_profile_client_2",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_profile_client_3",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_profile_client_4",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_profile_client_5",	"", CKN_STR1024, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_server_enable",	"0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipsec_client_enable",	"0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_QUAGGA
	{ "quagga_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* 0: Disable */
	{ "zebra_passwd", "zebra", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Set password for vty interface. If there is no password */
	{ "zebra_enpasswd", "zebra", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Set enable password for VTY.(default: zebra) */
	{ "rip_hostname", "ripd", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Set hostname of the ripd.(default: ripd) */
	{ "rip_passwd", "zebra", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Set password for vty interface. If there is no password */
#endif
#ifdef BRTAC828
	{ "lan_trunk_0", "0"},
	{ "lan_trunk_1", "0"},
#endif	/* BRTAC828 */

#if defined(RTCONFIG_PORT_BASED_VLAN)
	//{ "vlan_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vlan_rulelist", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "subnet_rulelist", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "gvlan_rulelist", "", CKN_STR4096, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_DISABLE_NETWORKMAP
	{ "networkmap_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_AUTOCOVER_SIP
	{ "atcover_sip", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "atcover_sip_ip", "192.168.1.1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "atcover_sip_type", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_CFGSYNC
	{ "cfg_group", "", CFGSYNC_GROUPID_LEN, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "cfg_alias", "", CFGSYNC_GROUPID_LEN, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_BT_CONN
	{ "bt_wanport", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "bt_user_place", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif /* RTCONFIG_BT_CONN */
#ifdef RTCONFIG_LP5523
	{ "lp55xx_lp5523_user_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lp55xx_lp5523_user_col", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lp55xx_lp5523_user_beh", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lp55xx_lp5523_user_brightness", "100", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lp55xx_lp5523_sch_enable", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lp55xx_lp5523_sch", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lp55xx_lp5523_sch_col", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lp55xx_lp5523_sch_beh", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lp55xx_lp5523_sch_brightness", "100", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif /* RTCONFIG_LP5523 */


	{ NULL, NULL }
}; // router_defaults

// nvram for system control state
struct nvram_tuple router_state_defaults[] = {
	{ "rc_service", "", CKN_STR128, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "asus_mfg", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "btn_rst", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "btn_ez", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_WIFI_TOG_BTN
	{ "btn_wifi_toggle", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_WIRELESS_SWITCH
	{ "btn_wifi_sw", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_LED_BTN
	{ "btn_led", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RT4GAC55U
	{ "btn_lte", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

	{ "link_internet", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "wan_primary", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* Always first run in WAN port. */
	{ "wan0_primary", "1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_DUALWAN) || defined(RTCONFIG_USB_MODEM)
	{ "wan1_primary", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "wan_pppoe_ifname", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_state_t", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_sbstate_t", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* record substate for each wan state */
	{ "wan_auxstate_t", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* which is maintained by wanduck */
	{ "wan_proto_t", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#ifdef RTCONFIG_GETREALIP
	{ "wan_realip_state", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_realip_ip", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

	{ "nat_state", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "link_wan", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if defined(RTCONFIG_DUALWAN) || defined(RTCONFIG_USB_MODEM)
	{ "link_wan1", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#ifdef RTCONFIG_IPV6
	{ "wan_6rd_router", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_6rd_ip4size", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_6rd_prefix", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_6rd_prefixlen", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv6_state_t", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ipv6_sbstate_t", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "lan_state_t", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lan_sbstate_t", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* record substate for each wan state */
	{ "lan_auxstate_t", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* whcih is mtainted by wanduck */

	{ "autodet_state", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "autodet_auxstate", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "invoke_later", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#if defined (RTCONFIG_WIRELESSREPEATER) || defined (RTCONFIG_PROXYSTA)
	// Wireless Client State
	{ "wlc_state", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc_sbstate", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc_scan_state", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wlc_mode", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_MEDIA_SERVER
	{ "dms_state", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dms_dbcwd", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_USBRESET
	{ "usbreset_active", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usbreset_num", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

	// USB state
#if 0
	{ "usb_path1", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_act", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_vid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_pid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_manufacturer", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_product", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_serial", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_speed", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_removed", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DISK_MONITOR
	{ "usb_path1_pool_error", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "usb_path1_fs_path0", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if 0
	{ "usb_path1_fs_path1", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path2", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path3", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path4", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path5", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path6", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path7", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path8", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path9", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path10", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path11", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path12", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path13", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path14", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_fs_path15", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label0", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label1", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label2", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label3", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label4", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label5", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label6", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label7", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label8", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label9", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label10", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label11", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label12", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label13", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label14", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path1_label15", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

	{ "usb_path2", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_act", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_vid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_pid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_manufacturer", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_product", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_serial", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_speed", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_removed", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DISK_MONITOR
	{ "usb_path2_pool_error", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "usb_path2_fs_path0", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if 0
	{ "usb_path2_fs_path1", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path2", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path3", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path4", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path5", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path6", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path7", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path8", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path9", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path10", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path11", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path12", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path13", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path14", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_fs_path15", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label0", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label1", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label2", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label3", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label4", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label5", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label6", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label7", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label8", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label9", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label10", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label11", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label12", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label13", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label14", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path2_label15", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

	{ "usb_path3", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_act", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_vid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_pid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_manufacturer", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_product", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_serial", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_speed", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_removed", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DISK_MONITOR
	{ "usb_path3_pool_error", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "usb_path3_fs_path0", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#if 0
	{ "usb_path3_fs_path1", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path2", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path3", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path4", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path5", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path6", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path7", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path8", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path9", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path10", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path11", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path12", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path13", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path14", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_fs_path15", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label0", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label1", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label2", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label3", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label4", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label5", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label6", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label7", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label8", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label9", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label10", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label11", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label12", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label13", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label14", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "usb_path3_label15", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
#endif

	{ "apps_dev", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_mounted_path", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_state_action", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_state_autorun", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_state_install", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_state_remove", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_state_switch", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_state_stop", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_state_enable", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_state_update", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_state_upgrade", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_state_error", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_download_file", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_download_percent", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_depend_do", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_depend_action", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "apps_depend_action_target", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#ifdef RTCONFIG_DISK_MONITOR
	{ "diskmon_status", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

	{ "webs_state_update", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "webs_state_upgrade", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "webs_state_error", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "webs_state_info", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "webs_notif_flag", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

	{ "ftp_ports", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

#ifdef RTCONFIG_OPENVPN
	{ "vpn_server1_state", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server1_errno", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server2_state", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_server2_errno", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client1_state", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client1_errno", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client2_state", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client2_errno", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client3_state", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client3_errno", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client4_state", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client4_errno", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client5_state", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "vpn_client5_errno", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#ifdef RTCONFIG_DSL
// name starting with 'dsl' are reserved for dsl unit
// for temp variable please use dsltmp_xxx
	{ "dsltmp_autodet_state", "down", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_autodet_vpi", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_autodet_vci", "33", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_autodet_encap", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_autodet_wan_type", "ATM", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
// manually config
	{ "dsltmp_cfg_country", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_cfg_ispname", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_cfg_vpi", "0", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_cfg_vci", "35", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_cfg_prctl", "0", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_cfg_encap", "0", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dsltmp_cfg_dot1q", "0", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_cfg_vid", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ "dsltmp_cfg_iptv_rmvlan", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_cfg_iptv_mr", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_cfg_iptv_idx", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_cfg_iptv_num_pvc", "0", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_cfg_iptv_pvclist", "", CKN_STR2048, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_cfg_iptv_enable", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },

// tmp variable for QIS , it will write to dsl0_xxx after finish page
	{ "dsltmp_qis_reboot", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* tmp for reboot flag before implement proper service without reboot */
	{ "dsltmp_qis_vpi", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_vci", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_proto", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_encap", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_pppoe_username", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_pppoe_passwd", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_pppoe_dial_on_demand", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_pppoe_idletime", "", CKN_STR32, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_pppoe_mtu", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_pppoe_mru", "", CKN_STR8, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_pppoe_service", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_pppoe_options", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_DHCPClient", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_ipaddr", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_netmask", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_gateway", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_dnsenable", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_dns1", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_dns2", "", CKN_STR15, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_svc_cat", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_pcr", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_scr", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_mbs", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_pppoe_relay", "", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_hwaddr", "", CKN_STR17, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_admin_username", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_admin_passwd", "", CKN_STR64, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_admin_passwd_set", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_qis_dsl_pvc_set", "0", CKN_STR1, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
// for DSL driver and tool
	{ "dsltmp_tc_resp_to_d", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_adslatequit", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_tcbootup", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_adslsyncsts", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_adslsyncsts_detail", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
// for web ui identify , 1=old ui, 2=asuswrt
	{ "dsltmp_web_ui_ver", "2", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//	log sync status
	{ "dsltmp_syncloss", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_syncloss_apply", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsltmp_syncup_cnt", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//	dla for notification
	{ "dsltmp_dla_modified", "0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
//	diagnostic
	{ "dsltmp_diag_log_path", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef RTCONFIG_DSL_TCLINUX
	{ "dsllog_fwver", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_drvver", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_linestate", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },	/* dsltmp_adslsyncsts */
	{ "dsllog_opmode", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_adsltype", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_snrmargindown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_snrmarginup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_attendown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_attenup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_wanlistmode", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_dataratedown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_datarateup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_attaindown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_attainup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_powerdown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_powerup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_crcdown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_crcup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_farendvendorid", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_pathmodedown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_pathmodeup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_interleavedepthdown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_interleavedepthup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_xdslmode", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_tcm", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_vdslcurrentprofile", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_inpdown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_inpup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_inpshinedown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_inpshineup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_inpreindown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_inpreinup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_snrmpbds", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_snrmpbus", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_latndown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_latnup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_satndown", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "dsllog_satnup", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif

#endif
	{ "ddns_cache", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_ipaddr", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_status", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_server_x_old", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_hostname_old", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_hostname_x_old", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_return_code", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_return_code_chk", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "ddns_update_by_wdog", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "reboot_time", "70", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ NULL, NULL }
};

#ifdef REMOVE
const defaults_t if_generic[] = {
	{ "lan_ifname", "br0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lan_ifnames", "eth0 eth2 eth3 eth4", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#ifdef DETWAN
	{ "wan_ifname", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_ifnames", "", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#else
	{ "wan_ifname", "eth1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_ifnames", "eth1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
#endif
	{ NULL, NULL }
};

const defaults_t if_vlan[] = {
	{ "lan_ifname", "br0", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "lan_ifnames", "vlan0 eth1 eth2 eth3", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_ifname", "vlan1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ "wan_ifnames", "vlan1", CKN_STR_DEFAULT, CKN_TYPE_DEFAULT, CKN_ACC_LEVEL_DEFAULT, 0 },
	{ NULL, NULL }
};
#endif

#ifdef RTCONFIG_BCMWL6
#ifndef RTCONFIG_BCMARM
struct nvram_tuple bcm4360ac_defaults[] = {
	{ "pci/2/1/aa2g", "0", 0 },
	{ "pci/2/1/aa5g", "7", 0 },
	{ "pci/2/1/aga0", "0", 0 },
	{ "pci/2/1/aga1", "0", 0 },
	{ "pci/2/1/aga2", "0", 0 },
	{ "pci/2/1/agbg0", "0", 0 },
	{ "pci/2/1/agbg1", "0", 0 },
	{ "pci/2/1/agbg2", "0", 0 },
	{ "pci/2/1/antswitch", "0", 0 },
	{ "pci/2/1/cckbw202gpo", "0", 0 },
	{ "pci/2/1/cckbw20ul2gpo", "0", 0 },
	{ "pci/2/1/dot11agofdmhrbw202gpo", "0", 0 },
	{ "pci/2/1/femctrl", "3", 0 },
	{ "pci/2/1/papdcap2g", "0", 0 },
	{ "pci/2/1/tworangetssi2g", "0", 0 },
	{ "pci/2/1/pdgain2g", "4", 0 },
	{ "pci/2/1/epagain2g", "0", 0 },
	{ "pci/2/1/tssiposslope2g", "1", 0 },
	{ "pci/2/1/gainctrlsph", "0", 0 },
	{ "pci/2/1/papdcap5g", "0", 0 },
	{ "pci/2/1/tworangetssi5g", "0", 0 },
	{ "pci/2/1/pdgain5g", "4", 0 },
	{ "pci/2/1/epagain5g", "0", 0 },
	{ "pci/2/1/tssiposslope5g", "1", 0 },
	{ "pci/2/1/maxp2ga0", "76", 0 },
	{ "pci/2/1/maxp2ga1", "76", 0 },
	{ "pci/2/1/maxp2ga2", "76", 0 },
	{ "pci/2/1/mcsbw202gpo", "0", 0 },
	{ "pci/2/1/mcsbw402gpo", "0", 0 },
	{ "pci/2/1/measpower", "0x7f", 0 },
	{ "pci/2/1/measpower1", "0x7f", 0 },
	{ "pci/2/1/measpower2", "0x7f", 0 },
	{ "pci/2/1/noiselvl2ga0", "31", 0 },
	{ "pci/2/1/noiselvl2ga1", "31", 0 },
	{ "pci/2/1/noiselvl2ga2", "31", 0 },
	{ "pci/2/1/noiselvl5gha0", "31", 0 },
	{ "pci/2/1/noiselvl5gha1", "31", 0 },
	{ "pci/2/1/noiselvl5gha2", "31", 0 },
	{ "pci/2/1/noiselvl5gla0", "31", 0 },
	{ "pci/2/1/noiselvl5gla1", "31", 0 },
	{ "pci/2/1/noiselvl5gla2", "31", 0 },
	{ "pci/2/1/noiselvl5gma0", "31", 0 },
	{ "pci/2/1/noiselvl5gma1", "31", 0 },
	{ "pci/2/1/noiselvl5gma2", "31", 0 },
	{ "pci/2/1/noiselvl5gua0", "31", 0 },
	{ "pci/2/1/noiselvl5gua1", "31", 0 },
	{ "pci/2/1/noiselvl5gua2", "31", 0 },
	{ "pci/2/1/ofdmlrbw202gpo", "0", 0 },
	{ "pci/2/1/pa2ga0", "0xfe72,0x14c0,0xfac7", 0 },
	{ "pci/2/1/pa2ga1", "0xfe80,0x1472,0xfabc", 0 },
	{ "pci/2/1/pa2ga2", "0xfe82,0x14bf,0xfad9", 0 },
	{ "pci/2/1/pcieingress_war", "15", 0 },
	{ "pci/2/1/phycal_tempdelta", "255", 0 },
	{ "pci/2/1/rawtempsense", "0x1ff", 0 },
	{ "pci/2/1/rxchain", "7", 0 },
	{ "pci/2/1/rxgainerr2g", "0xffff", 0 },
	{ "pci/2/1/rxgainerr5g", "0xffff,0xffff,0xffff,0xffff", 0 },
	{ "pci/2/1/rxgains2gelnagaina0", "0", 0 },
	{ "pci/2/1/rxgains2gelnagaina1", "0", 0 },
	{ "pci/2/1/rxgains2gelnagaina2", "0", 0 },
	{ "pci/2/1/rxgains2gtrelnabypa0", "0", 0 },
	{ "pci/2/1/rxgains2gtrelnabypa1", "0", 0 },
	{ "pci/2/1/rxgains2gtrelnabypa2", "0", 0 },
	{ "pci/2/1/rxgains2gtrisoa0", "0", 0 },
	{ "pci/2/1/rxgains2gtrisoa1", "0", 0 },
	{ "pci/2/1/rxgains2gtrisoa2", "0", 0 },
	{ "pci/2/1/sar2g", "18", 0 },
	{ "pci/2/1/sar5g", "15", 0 },
	{ "pci/2/1/sromrev", "11", 0 },
	{ "pci/2/1/subband5gver", "0x4", 0 },
	{ "pci/2/1/tempcorrx", "0x3f", 0 },
	{ "pci/2/1/tempoffset", "255", 0 },
	{ "pci/2/1/temps_hysteresis", "15", 0 },
	{ "pci/2/1/temps_period", "15", 0 },
	{ "pci/2/1/tempsense_option", "0x3", 0 },
	{ "pci/2/1/tempsense_slope", "0xff", 0 },
	{ "pci/2/1/tempthresh", "255", 0 },
	{ "pci/2/1/txchain", "7", 0 },
	{ "pci/2/1/ledbh10", "7", 0 },

	{ 0, 0, 0 }
};
#else
#ifdef RTAC3200
struct nvram_tuple bcm4360ac_defaults[] = {
	{ "devpath1", "pcie/1/4", 0 },
	{ "1:devpath1", "sb/1/", 0 },
	{ "1:boardrev", "0x1421", 0 },
	{ "1:boardvendor", "0x14e4", 0 },
	{ "1:devid", "0x43bb", 0 },
	{ "1:sromrev", "11", 0 },
	{ "1:boardflags", "0x20001000", 0 },
	{ "1:boardflags2", "0x00100002", 0 },
	{ "1:venvid", "0x14e4", 0 },
	{ "1:boardflags3", "0x4000005", 0 },
	{ "1:aa2g", "7", 0 },
	{ "1:agbg0", "0x0", 0 },
	{ "1:agbg1", "0x0", 0 },
	{ "1:agbg2", "0x0", 0 },
	{ "1:txchain", "7", 0 },
	{ "1:rxchain", "7", 0 },
	{ "1:antswitch", "0", 0 },
	{ "1:femctrl", "3", 0 },
	{ "1:tssiposslope2g", "1", 0 },
	{ "1:epagain2g", "0", 0 },
	{ "1:pdgain2g", "21", 0 },
	{ "1:tworangetssi2g", "0", 0 },
	{ "1:papdcap2g", "0", 0 },
	{ "1:gainctrlsph", "0", 0 },
	{ "1:tempthresh", "120", 0 },
	{ "1:tempoffset", "255", 0 },
	{ "1:rawtempsense", "0x1ff", 0 },
	{ "1:tempsense_slope", "0xff", 0 },
	{ "1:tempcorrx", "0x3f", 0 },
	{ "1:tempsense_option", "0x3", 0 },
	{ "1:xtalfreq", "40000", 0 },
	{ "1:phycal_tempdelta", "15", 0 },
	{ "1:temps_period", "5", 0 },
	{ "1:temps_hysteresis", "5", 0 },
	{ "1:pdoffset2g40ma0", "15", 0 },
	{ "1:pdoffset2g40ma1", "15", 0 },
	{ "1:pdoffset2g40ma2", "15", 0 },
	{ "1:pdoffset2g40mvalid", "1", 0 },
	{ "1:pdoffset40ma0", "0", 0 },
	{ "1:pdoffset40ma1", "0", 0 },
	{ "1:pdoffset40ma2", "0", 0 },
	{ "1:pdoffset80ma0", "0", 0 },
	{ "1:pdoffset80ma1", "0", 0 },
	{ "1:pdoffset80ma2", "0", 0 },
	{ "1:cckbw202gpo", "0", 0 },
	{ "1:cckbw20ul2gpo", "0", 0 },
	{ "1:dot11agofdmhrbw202gpo", "0x2000", 0 },
	{ "1:ofdmlrbw202gpo", "0", 0 },
	{ "1:dot11agduphrpo", "0", 0 },
	{ "1:dot11agduplrpo", "0", 0 },
	{ "1:maxp2ga0", "102", 0 },
	{ "1:rxgains2gelnagaina0", "4", 0 },
	{ "1:rxgains2gtrisoa0", "7", 0 },
	{ "1:rxgains2gtrelnabypa0", "1", 0 },
	{ "1:maxp2ga1", "102", 0 },
	{ "1:rxgains2gelnagaina1", "4", 0 },
	{ "1:rxgains2gtrisoa1", "7", 0 },
	{ "1:rxgains2gtrelnabypa1", "1", 0 },
	{ "1:maxp2ga2", "102", 0 },
	{ "1:rxgains2gelnagaina2", "4", 0 },
	{ "1:rxgains2gtrisoa2", "7", 0 },
	{ "1:rxgains2gtrelnabypa2", "1", 0 },
	{ "1:ledbh10", "7", 0 },
	{ "devpath0", "pcie/1/3", 0 },
	{ "0:devpath0", "sb/1/", 0 },
	{ "0:boardrev", "0x1421", 0 },
	{ "0:boardvendor", "0x14e4", 0 },
	{ "0:devid", "0x43bc", 0 },
	{ "0:sromrev", "11", 0 },
	{ "0:boardflags", "0x30040000", 0 },
	{ "0:boardflags2", "0x00220102", 0 },
	{ "0:venid", "0x14e4", 0 },
	{ "0:boardflags3", "0x0", 0 },
	{ "0:aa5g", "7", 0 },
	{ "0:aga0", "0x0", 0 },
	{ "0:aga1", "0x0", 0 },
	{ "0:aga2", "0x0", 0 },
	{ "0:txchain", "7", 0 },
	{ "0:rxchain", "7", 0 },
	{ "0:antswitch", "0", 0 },
	{ "0:femctrl", "3", 0 },
	{ "0:tssiposslope5g", "1", 0 },
	{ "0:epagain5g", "0", 0 },
	{ "0:pdgain5g", "4", 0 },
	{ "0:tworangetssi5g", "0", 0 },
	{ "0:papdcap5g", "0", 0 },
	{ "0:gainctrlsph", "0", 0 },
	{ "0:tempthresh", "125", 0 },
	{ "0:tempoffset", "255", 0 },
	{ "0:rawtempsense", "0x1ff", 0 },
	{ "0:tempsense_slope", "0xff", 0 },
	{ "0:tempcorrx", "0x3f", 0 },
	{ "0:tempsense_option", "0x3", 0 },
	{ "0:xtalfreq", "40000", 0 },
	{ "0:phycal_tempdelta", "15", 0 },
	{ "0:temps_period", "5", 0 },
	{ "0:temps_hysteresis", "5", 0 },
	{ "0:pdoffset40ma0", "4369", 0 },
	{ "0:pdoffset40ma1", "4369", 0 },
	{ "0:pdoffset40ma2", "4369", 0 },
	{ "0:pdoffset80ma0", "0", 0 },
	{ "0:pdoffset80ma1", "0", 0 },
	{ "0:pdoffset80ma2", "0", 0 },
	{ "0:subband5gver", "0x4", 0 },
	{ "0:mcsbw1605glpo", "0", 0 },
	{ "0:mcsbw1605gmpo", "0", 0 },
	{ "0:mcsbw1605ghpo", "0", 0 },
	{ "0:mcslr5glpo", "0", 0 },
	{ "0:mcslr5gmpo", "0", 0 },
	{ "0:mcslr5ghpo", "0", 0 },
	{ "0:dot11agduphrpo", "0", 0 },
	{ "0:dot11agduplrpo", "0", 0 },
	{ "0:rxgains5gmelnagaina0", "2", 0 },
	{ "0:rxgains5gmtrisoa0", "5", 0 },
	{ "0:rxgains5gmtrelnabypa0", "1", 0 },
	{ "0:rxgains5ghelnagaina0", "2", 0 },
	{ "0:rxgains5ghtrisoa0", "5", 0 },
	{ "0:rxgains5ghtrelnabypa0", "1", 0 },
	{ "0:rxgains5gelnagaina0", "2", 0 },
	{ "0:rxgains5gtrisoa0", "5", 0 },
	{ "0:rxgains5gtrelnabypa0", "1", 0 },
	{ "0:maxp5ga0", "94,94,90,90", 0 },
	{ "0:rxgains5gmelnagaina1", "2", 0 },
	{ "0:rxgains5gmtrisoa1", "5", 0 },
	{ "0:rxgains5gmtrelnabypa1", "1", 0 },
	{ "0:rxgains5ghelnagaina1", "2", 0 },
	{ "0:rxgains5ghtrisoa1", "5", 0 },
	{ "0:rxgains5ghtrelnabypa1", "1", 0 },
	{ "0:rxgains5gelnagaina1", "2", 0 },
	{ "0:rxgains5gtrisoa1", "5", 0 },
	{ "0:rxgains5gtrelnabypa1", "1", 0 },
	{ "0:maxp5ga1", "94,94,90,90", 0 },
	{ "0:rxgains5gmelnagaina2", "2", 0 },
	{ "0:rxgains5gmtrisoa2", "5", 0 },
	{ "0:rxgains5gmtrelnabypa2", "1", 0 },
	{ "0:rxgains5ghelnagaina2", "2", 0 },
	{ "0:rxgains5ghtrisoa2", "5", 0 },
	{ "0:rxgains5ghtrelnabypa2", "1", 0 },
	{ "0:rxgains5gelnagaina2", "2", 0 },
	{ "0:rxgains5gtrisoa2", "5", 0 },
	{ "0:rxgains5gtrelnabypa2", "1", 0 },
	{ "0:maxp5ga2", "94,94,90,90", 0 },
	{ "0:ledbh10", "7", 0 },
	{ "devpath2", "pcie/2/1", 0 },
	{ "2:devpath2", "sb/1/", 0 },
	{ "2:boardrev", "0x1421", 0 },
	{ "2:boardvendor", "0x14e4", 0 },
	{ "2:devid", "0x43bc", 0 },
	{ "2:sromrev", "11", 0 },
	{ "2:boardflags", "0x30040000", 0 },
	{ "2:boardflags2", "0x00220102", 0 },
	{ "2:venid", "0x14e4", 0 },
	{ "2:boardflags3", "0x0", 0 },
	{ "2:aa5g", "7", 0 },
	{ "2:aga0", "0x0", 0 },
	{ "2:aga1", "0x0", 0 },
	{ "2:aga2", "0x0", 0 },
	{ "2:txchain", "7", 0 },
	{ "2:rxchain", "7", 0 },
	{ "2:antswitch", "0", 0 },
	{ "2:femctrl", "3", 0 },
	{ "2:tssiposslope5g", "1", 0 },
	{ "2:epagain5g", "0", 0 },
	{ "2:pdgain5g", "4", 0 },
	{ "2:tworangetssi5g", "0", 0 },
	{ "2:papdcap5g", "0", 0 },
	{ "2:gainctrlsph", "0", 0 },
	{ "2:tempthresh", "120", 0 },
	{ "2:tempoffset", "255", 0 },
	{ "2:rawtempsense", "0x1ff", 0 },
	{ "2:tempsense_slope", "0xff", 0 },
	{ "2:tempcorrx", "0x3f", 0 },
	{ "2:tempsense_option", "0x3", 0 },
	{ "2:xtalfreq", "40000", 0 },
	{ "2:phycal_tempdelta", "15", 0 },
	{ "2:temps_period", "5", 0 },
	{ "2:temps_hysteresis", "5", 0 },
	{ "2:pdoffset40ma0", "4369", 0 },
	{ "2:pdoffset40ma1", "4369", 0 },
	{ "2:pdoffset40ma2", "4369", 0 },
	{ "2:pdoffset80ma0", "0", 0 },
	{ "2:pdoffset80ma1", "0", 0 },
	{ "2:pdoffset80ma2", "0", 0 },
	{ "2:subband5gver", "0x4", 0 },
	{ "2:mcsbw1605glpo", "0", 0 },
	{ "2:mcsbw1605gmpo", "0", 0 },
	{ "2:mcsbw1605ghpo", "0", 0 },
	{ "2:mcslr5glpo", "0", 0 },
	{ "2:mcslr5gmpo", "0", 0 },
	{ "2:mcslr5ghpo", "0", 0 },
	{ "2:dot11agduphrpo", "0", 0 },
	{ "2:dot11agduplrpo", "0", 0 },
	{ "2:rxgains5gmelnagaina0", "2", 0 },
	{ "2:rxgains5gmtrisoa0", "5", 0 },
	{ "2:rxgains5gmtrelnabypa0", "1", 0 },
	{ "2:rxgains5ghelnagaina0", "2", 0 },
	{ "2:rxgains5ghtrisoa0", "5", 0 },
	{ "2:rxgains5ghtrelnabypa0", "1", 0 },
	{ "2:rxgains5gelnagaina0", "2", 0 },
	{ "2:rxgains5gtrisoa0", "5", 0 },
	{ "2:rxgains5gtrelnabypa0", "1", 0 },
	{ "2:maxp5ga0", "90,90,106,106", 0 },
	{ "2:rxgains5gmelnagaina1", "2", 0 },
	{ "2:rxgains5gmtrisoa1", "5", 0 },
	{ "2:rxgains5gmtrelnabypa1", "1", 0 },
	{ "2:rxgains5ghelnagaina1", "2", 0 },
	{ "2:rxgains5ghtrisoa1", "5", 0 },
	{ "2:rxgains5ghtrelnabypa1", "1", 0 },
	{ "2:rxgains5gelnagaina1", "2", 0 },
	{ "2:rxgains5gtrisoa1", "5", 0 },
	{ "2:rxgains5gtrelnabypa1", "1", 0 },
	{ "2:maxp5ga1", "90,90,106,106", 0 },
	{ "2:rxgains5gmelnagaina2", "2", 0 },
	{ "2:rxgains5gmtrisoa2", "5", 0 },
	{ "2:rxgains5gmtrelnabypa2", "1", 0 },
	{ "2:rxgains5ghelnagaina2", "2", 0 },
	{ "2:rxgains5ghtrisoa2", "5", 0 },
	{ "2:rxgains5ghtrelnabypa2", "1", 0 },
	{ "2:rxgains5gelnagaina2", "2", 0 },
	{ "2:rxgains5gtrisoa2", "5", 0 },
	{ "2:rxgains5gtrelnabypa2", "1", 0 },
	{ "2:maxp5ga2", "90,90,106,106", 0 },
	{ "2:ledbh10", "7", 0 },
	{ 0, 0, 0 }
};
#elif defined(RTAC88U) || defined(RTAC3100) || defined(RTAC5300) || defined(GTAC5300) || defined(RTAC86U) || defined(AC2900)
struct nvram_tuple bcm4360ac_defaults[] = {
	{ 0, 0, 0 }
};
#elif defined(RTAC1200G) || defined(RTAC1200GP)
struct nvram_tuple bcm4360ac_defaults[] = {
	{ "0:xtalfreq", "20000", 0 },
	{ "0:opo", "68", 0 },
	{ "0:aa2g", "0x3", 0 },
	{ "0:ag0", "0x2", 0 },
	{ "0:ag1", "0x2", 0 },
	{ "0:ag2", "0xff", 0 },
	{ "0:ag3", "0xff", 0 },
	{ "0:bxa2g", "0x0", 0 },
	{ "0:rssisav2g", "0x0", 0 },
	{ "0:rssismc2g", "0x0", 0 },
	{ "0:rssismf2g", "0x0", 0 },
	{ "0:tri2g", "0x0", 0 },
	{ "0:rxpo2g", "0x0", 0 },
	{ "0:txchain", "0x3", 0 },
	{ "0:rxchain", "0x3", 0 },
	{ "0:antswitch", "0x0", 0 },
	{ "0:tssipos2g", "0x1", 0 },
//	{ "0:extpagain2g", "0x0", 0 },
	{ "0:pdetrange2g", "0x3", 0 },
	{ "0:triso2g", "0x4", 0 },
	{ "0:antswctl2g", "0x0", 0 },
	{ "0:elna2g", "0x2", 0 },
	{ "0:itt2ga0", "0x20", 0 },
	{ "0:itt2ga1", "0x20", 0 },
	{ "0:ledbh0", "11", 0 },
	{ "0:ledbh1", "11", 0 },
	{ "0:ledbh2", "11", 0 },
	{ "0:ledbh3", "7", 0 },
	{ "0:leddc", "0xFFFF", 0 },
	{ "0:temps_period", "5", 0 },
	{ "0:tempthresh", "120", 0 },
	{ "0:temps_hysteresis", "5", 0 },
	{ "0:phycal_tempdelta", "0", 0 },
	{ "0:tempoffset", "0", 0 },
	{ "sb/1/aa2g", "0", 0 },
	{ "sb/1/aa5g", "3", 0 },
	{ "sb/1/aga0", "0", 0 },
	{ "sb/1/aga1", "0", 0 },
	{ "sb/1/aga2", "0", 0 },
	{ "sb/1/txchain", "3", 0 },
	{ "sb/1/rxchain", "3", 0 },
	{ "sb/1/antswitch", "0", 0 },
	{ "sb/1/femctrl", "15", 0 },
	{ "sb/1/subband5gver", "4", 0 },
	{ "sb/1/gainctrlsph", "0", 0 },
	{ "sb/1/papdcap5g", "0", 0 },
	{ "sb/1/tworangetssi5g", "0", 0 },
	{ "sb/1/pdgain5g", "0", 0 },
//	{ "sb/1/epagain5g", "0", 0 },
	{ "sb/1/tssiposslope5g", "1", 0 },
	{ "sb/1/AvVmid_c0", "2,135,2,135,2,135,2,135,2,135", 0 },
	{ "sb/1/AvVmid_c1", "2,145,2,145,2,145,2,145,2,145", 0 },
	{ "sb/1/paparambwver", "3", 0 },
	{ "paparambwver", "3", 0 },
	{ "sb/1/pdoffset5gsubbanda0", "0x0000", 0 },
	{ "sb/1/pdoffset5gsubbanda1", "0x0000", 0 },
	{ "sb/1/rxgains5gelnagaina0", "3", 0 },
	{ "sb/1/rxgains5gtrelnabypa0", "1", 0 },
//	{ "sb/1/rxgains5gtrisoa0", "8", 0 },
	{ "sb/1/rxgains5gmelnagaina0", "3", 0 },
	{ "sb/1/rxgains5gmtrelnabypa0", "1", 0 },
//	{ "sb/1/rxgains5gmtrisoa0", "8", 0 },
	{ "sb/1/rxgains5ghelnagaina0", "3", 0 },
	{ "sb/1/rxgains5ghtrelnabypa0", "1", 0 },
//	{ "sb/1/rxgains5ghtrisoa0", "8", 0 },
	{ "sb/1/rxgains5gelnagaina1", "3", 0 },
	{"sb/1/rxgains5gtrelnabypa1", "1", 0 },
//	{"sb/1/rxgains5gtrisoa1", "8", 0 },
	{"sb/1/rxgains5gmelnagaina1", "3", 0 },
	{"sb/1/rxgains5gmtrelnabypa1", "1", 0 },
//	{"sb/1/rxgains5gmtrisoa1", "8", 0 },
	{"sb/1/rxgains5ghelnagaina1", "3", 0 },
	{"sb/1/rxgains5ghtrelnabypa1", "1", 0 },
//	{"sb/1/rxgains5ghtrisoa1", "5", 0 },
	{"sb/1/rssi_delta_5gl_c0", "-3,0,-5,0,-5,0", 0 },
	{"sb/1/rssi_delta_5gml_c0", "-4,0,-5,0,-5,0", 0 },
	{"sb/1/rssi_delta_5gmu_c0", "-4,0,-5,0,-5,0", 0 },
	{"sb/1/rssi_delta_5gh_c0", "-4,0,-6,0,-5,0", 0 },
	{"sb/1/rssi_delta_5gl_c1", "-3,0,-5,0,-5,0", 0 },
	{"sb/1/rssi_delta_5gml_c1", "-4,0,-5,0,-5,0", 0 },
	{"sb/1/rssi_delta_5gmu_c1", "-4,0,-5,0,-5,0", 0 },
	{"sb/1/rssi_delta_5gh_c1", "-4,0,-6,0,-5,0", 0 },
	{"sb/1/rssi_delta_2g_c0", "4,5,4,5", 0 },
	{"sb/1/rssi_delta_2g_c1", "2,3,2,3", 0 },
	{"sb/1/ledbh0", "11", 0 },
	{"sb/1/ledbh1", "11", 0 },
	{"sb/1/ledbh2", "11", 0 },
	{"sb/1/ledbh3", "11", 0 },
	{"sb/1/ledbh11", "0x7", 0 },
	{"sb/1/leddc", "0xFFFF", 0 },
	{"sb/1/temps_period", "5", 0 },
	{"sb/1/tempthresh", "120", 0 },
	{"sb/1/temps_hysteresis", "5", 0 },
	{"sb/1/phycal_tempdelta", "15", 0 },
	{"sb/1/tempoffset", "0", 0 },
	{0, 0, 0 }
};
#else
struct nvram_tuple bcm4360ac_defaults[] = {
	{ "0:ledbh10", "7", 0 },
	{ "1:ledbh10", "7", 0 },
	{ "0:temps_period", "5", 0 },
	{ "0:tempthresh", "120", 0 },
	{ "0:temps_hysteresis", "5", 0 },
	{ "0:phycal_tempdelta", "0", 0 },
	{ "0:tempoffset", "0", 0 },
	{ "1:temps_period", "5", 0 },
	{ "1:tempthresh", "120", 0 },
	{ "1:temps_hysteresis", "5", 0 },
	{ "1:phycal_tempdelta", "0", 0 },
	{ "1:tempoffset", "0", 0 },
	{ 0, 0, 0 }
};
#endif
#endif
#endif

/* nvram override default setting for Media Router */
struct nvram_tuple router_defaults_override_type1[] = {
	{ "router_disable", "1", 0 },		/* lan_proto=static lan_stp=0 wan_proto=disabled */
	{ "lan_stp", "0", 0 },			/* LAN spanning tree protocol */
	{ "wl_wmf_bss_enable", "1", 0 },	/* WMF Enable for IPTV Media or WiFi+PLC */
	{ "wl_reg_mode", "h", 0 },		/* Regulatory: 802.11H(h) */
#ifndef RTCONFIG_BCM7
	{ "wl_taf_enable", "1", 0 },		/* Enable TAF */
	{ "wl_taf_rule", "0x15", 0 },		/* Default TAF rule on SSID, RATE and AID */

	/* EBOS feature Media router default */
	{ "wl_ebos_enable", "0", 0 },		/* EBOS feature on */
	{ "wl_ebos_flags", "0x68", 0 },		/* 104(0x68) video links */
	{ "wl_ebos_transit", "-1", 0 },		/* transit limit for video links */
	{ "wl_ebos_prr_flags", "0xa41", 0 },	/* pseudo-round robin data links */
	{ "wl_ebos_prr_threshold", "0x0f000000", 0 },	/* pseudo-round robin threshold */
	{ "wl_ebos_prr_transit", "-1", 0 },	/* pseudo-round robin transit limit */

	/* Airtime fairness */
	{ "wl_atf", "1", 0 },			/* ATF feature on */
#else
	{ "wl_taf_enable", "0", 0 },		/* Disable TAF */
#endif
#ifdef __CONFIG_EMF__
	{ "emf_enable", "1", 0 },		/* Enable EMF by default */
	{ "wl_wmf_ucigmp_query", "1", 0 },	/* Enable Converting IGMP Query to ucast */
#ifdef RTCONFIG_BCMARM
	{ "wl_wmf_ucast_upnp", "1", 0 },	/* Enable upnp to ucast conversion */
	{ "wl_wmf_igmpq_filter", "1", 0 },	/* Enable igmp query filter */
#endif
#endif
	{ "wl_frameburst", "on", 0 },		/* BRCM Frambursting mode (off|on) */
#ifdef RTCONFIG_BCMWL6
	{ "wl_acs_fcs_mode", "1", 0 },		/* Enable acsd fcs mode */
#ifndef RTCONFIG_BCM7
	{ "wl_acs_dfs", "2", 0 },		/* Enable first DFS chan Selection */
#endif
	{ "wl_dcs_csa_unicast", "1", 0 },	/* Enable unicast CSA */
	/* Exclude ACSD to select 140l, 144u, 140/80, 144/80 to compatible with Ducati 11N */
	{ "wl_acs_excl_chans", "0xd98e,0xd88e,0xe28a,0xe38a", 0 },
#ifdef RTCONFIG_BCMARM
#ifndef RTCONFIG_BCM7
	{ "frameburst_dyn", "0", 0 },		/* Frameburst controlled dynamically if on */
#endif
	{ "wl_pspretend_retry_limit", "5", 0 }, /* Enable PsPretend */
#ifndef RTCONFIG_BCM7
	{ "wl_pspretend_threshold", "0", 0 },	/* Disable PsPretend Threshold */
#endif
#endif
#endif
	{ "wl_cal_period", "0", 0 },		/* Disable periodic cal */
	{ "wl_amsdu", "off", 0 },		/* Default IPTV AMSDU setting */
#ifdef RTCONFIG_BCMARM
#if !defined(RTCONFIG_BCM_7114) && !defined(HND_ROUTER)
	{ "wl_rx_amsdu_in_ampdu", "off", 0 },	/* Media RX AMSDU In AMPDU setting */
#endif
	{ "wl_psta_inact", "0", 0 },		/* PSTA inactivity timer */
	{ "wl_srl", "11", 0},			/* Short Retry Limit */
	{ "wl_lrl", "11", 0},			/* Long Retry Limit */
#endif
	{ 0, 0, 0 }
};

/* Translates from, for example, wl0_ (or wl0.1_) to wl_. */
/* Only single digits are currently supported */

static void
fix_name(const char *name, char *fixed_name)
{
	char *pSuffix = NULL;

	/* Translate prefix wlx_ and wlx.y_ to wl_ */
	/* Expected inputs are: wld_root, wld.d_root, wld.dd_root
	 * We accept: wld + '_' anywhere
	 */
	pSuffix = strchr(name, '_');

	if ((strncmp(name, "wl", 2) == 0) && isdigit(name[2]) && (pSuffix != NULL)) {
		strcpy(fixed_name, "wl");
		strcpy(&fixed_name[2], pSuffix);
		return;
	}

	/* No match with above rules: default to input name */
	strcpy(fixed_name, name);
}

extern char *tcode_default_get(const char *name);

/*
 * Find nvram param name; return pointer which should be treated as const
 * return NULL if not found.
 *
 * NOTE: This routine special-cases the variable wl_bss_enabled. It will
 * return the normal default value if asked for wl_ or wl0_. But it will
 * return 0 if asked for a virtual BSS reference like wl0.1_.
 */
char *
nvram_default_get(const char *name)
{
	int idx;
	char fixed_name[NVRAM_MAX_VALUE_LEN];

	fix_name(name, fixed_name);
	if (strcmp(fixed_name, "wl_bss_enabled") == 0) {
		if (name[3] == '.' || name[4] == '.') { /* Virtual interface */
			return "0";
		}
	}
#ifdef RTCONFIG_BCMARM
#ifndef RTCONFIG_BCM7
#ifdef __CONFIG_HSPOT__
	if (strcmp(fixed_name, "wl_bss_hs2_enabled") == 0) {
		if (name[3] == '.' || name[4] == '.') { /* Virtual interface */
			return "0";
		}
	}
#endif /* __CONFIG_HSPOT__ */
#endif
	if (!strcmp(nvram_safe_get("devicemode"), "1")) {
		for (idx = 0; router_defaults_override_type1[idx].name != NULL; idx++) {
			if (strcmp(router_defaults_override_type1[idx].name, fixed_name) == 0) {
				return router_defaults_override_type1[idx].value;
			}
		}
	}
#endif
#ifdef RTCONFIG_TCODE
	if (strncmp(name, "lan_", 4) == 0 || strncmp(name, "dhcp_", 5) == 0) {
		char *value = tcode_default_get(fixed_name);
		if (value) {
			return value;
		}
	}
#endif

	/* check name wlx_xxx first */
	if ((strncmp(name, "wl", 2) == 0) && isdigit(name[2]) && (name[3] == '_'))
	for (idx = 0; router_defaults[idx].name != NULL; idx++) {
		if (strcmp(router_defaults[idx].name, name) == 0) {
			return router_defaults[idx].value;
		}
	}

	for (idx = 0; router_defaults[idx].name != NULL; idx++) {
		if (strcmp(router_defaults[idx].name, fixed_name) == 0) {
			return router_defaults[idx].value;
		}
	}

	return NULL;
}

/* validate/restore all per-interface related variables */
void
nvram_validate_all(char *prefix, bool restore)
{
	struct nvram_tuple *t;
	char tmp[100];
	char *v;

	return;

	for (t = router_defaults; t->name; t++) {
		if (!strncmp(t->name, "wl_", 3)) {
			strcat_r(prefix, &t->name[3], tmp);
			if (!restore && nvram_get(tmp))
				continue;
			v = nvram_get(t->name);
			nvram_set(tmp, v ? v : t->value);
		}
	}

	/* override router type1 nvram setting */
	if (!strcmp(nvram_safe_get("devicemode"), "1")) {
		for (t = router_defaults_override_type1; t->name; t++) {
			if (!strncmp(t->name, "wl_", 3)) {
				strcat_r(prefix, &t->name[3], tmp);
				if (!restore && nvram_get(tmp))
					continue;
				v = nvram_get(t->name);
				nvram_set(tmp, v ? v : t->value);
			}
		}
	}
}

/* restore specific per-interface variable */
void
nvram_restore_var(char *prefix, char *name)
{
	struct nvram_tuple *t;
	char tmp[100];

	for (t = router_defaults; t->name; t++) {
		if (!strncmp(t->name, "wl_", 3) && !strcmp(&t->name[3], name)) {
			nvram_set(strcat_r(prefix, name, tmp), t->value);
			break;
		}
	}

	/* override router type1 setting */
	if (!strcmp(nvram_safe_get("devicemode"), "1")) {
		for (t = router_defaults_override_type1; t->name; t++) {
			if (!strncmp(t->name, "wl_", 3) && !strcmp(&t->name[3], name)) {
				nvram_set(strcat_r(prefix, name, tmp), t->value);
				break;
			}
		}
	}
}
