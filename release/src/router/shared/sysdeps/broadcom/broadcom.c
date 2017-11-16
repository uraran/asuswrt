#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <bcmendian.h>
#include <bcmnvram.h>
#include <wlutils.h>
#include "utils.h"
#include "shutils.h"
#include "shared.h"

#ifdef RTCONFIG_AMAS 
#include <json.h>
#endif

#ifdef RTCONFIG_AMAS
#include <amas_path.h>
#endif

#ifdef LINUX26
#define GPIO_IOCTL
#endif

// --- move begin ---
#ifdef GPIO_IOCTL

#include <sys/ioctl.h>
#include <linux_gpio.h>
#include <time.h>

static int _gpio_ioctl(int f, int gpioreg, unsigned int mask, unsigned int val)
{
	struct gpio_ioctl gpio;
                                                                                                                     
	gpio.val = val;
	gpio.mask = mask;

	if (ioctl(f, gpioreg, &gpio) < 0) {
		_dprintf("Invalid gpioreg %d\n", gpioreg);
		return -1;
	}
	return (gpio.val);
}

static int _gpio_open()
{
	int f = open("/dev/gpio", O_RDWR);
	if (f < 0)
		_dprintf ("Failed to open /dev/gpio\n");
	return f;
}

int gpio_open(uint32_t mask)
{
	uint32_t bit;
	int i;
	int f = _gpio_open();

	if ((f >= 0) && mask) {
		for (i = 0; i <= 15; i++) {
			bit = 1 << i;
			if ((mask & bit) == bit) {
				_gpio_ioctl(f, GPIO_IOC_RESERVE, bit, bit);
				_gpio_ioctl(f, GPIO_IOC_OUTEN, bit, 0);
			}
		}
		close(f);
		f = _gpio_open();
	}

	return f;
}

void gpio_write(uint32_t bitvalue, int en)
{
	int f;
	uint32_t bit;

	bit = 1<< bitvalue;

	if ((f = gpio_open(0)) < 0) return;

	_gpio_ioctl(f, GPIO_IOC_RESERVE, bit, bit);
	_gpio_ioctl(f, GPIO_IOC_OUTEN, bit, bit);
	_gpio_ioctl(f, GPIO_IOC_OUT, bit, en ? bit : 0);
	close(f);
}

uint32_t _gpio_read(int f)
{
	uint32_t r;
//	r = _gpio_ioctl(f, GPIO_IOC_IN, 0xFFFF, 0);
	r = _gpio_ioctl(f, GPIO_IOC_IN, 0x07FF, 0);
	if (r < 0) r = ~0;
	return r;
}

uint32_t gpio_read(void)
{
	int f;
	uint32_t r;

	if ((f = gpio_open(0)) < 0) return ~0;
	r = _gpio_read(f);
	close(f);
	return r;
}

#else

int gpio_open(uint32_t mask)
{
	int f = open(DEV_GPIO(in), O_RDONLY|O_SYNC);
	if (f < 0)
		_dprintf ("Failed to open %s\n", DEV_GPIO(in));
	return f;
}

void gpio_write(uint32_t bitvalue, int en)
{
	int f;
	uint32_t r;
	uint32_t bit;

	bit = 1<<bitvalue;

	if ((f = open(DEV_GPIO(control), O_RDWR)) < 0) return;
	read(f, &r, sizeof(r));
	r &= ~bit;
	write(f, &r, sizeof(r));
	close(f);

	if ((f = open(DEV_GPIO(outen), O_RDWR)) < 0) return;
	read(f, &r, sizeof(r));
	r |= bit;
	write(f, &r, sizeof(r));
	close(f);

	if ((f = open(DEV_GPIO(out), O_RDWR)) < 0) return;
	read(f, &r, sizeof(r));
	if (en) r |= bit;
		else r &= ~bit;
	write(f, &r, sizeof(r));
	close(f);
}

uint32_t _gpio_read(int f)
{
	uint32_t v;
	return (read(f, &v, sizeof(v)) == sizeof(v)) ? v : ~0;
}

uint32_t gpio_read(void)
{
	int f;
	uint32_t r;

	if ((f = open(DEV_GPIO(in), O_RDONLY)) < 0) return ~0;
	r = _gpio_read(f);
	close(f);
	return r;
}


#endif

#ifdef RTCONFIG_AMAS 
static bool g_swap = FALSE;
#define htod32(i) (g_swap?bcmswap32(i):(uint32)(i))
#define dtoh32(i) (g_swap?bcmswap32(i):(uint32)(i))
#define dtoh16(i) (g_swap?bcmswap16(i):(uint16)(i))
char *get_pap_bssid(int unit)
{
	unsigned char bssid[6] = {0};
	char tmp[128], prefix[] = "wlXXXXXXXXXX_";
	char *name;
	static char bssid_str[sizeof("00:00:00:00:00:00XXX")];

	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	if (wl_ioctl(name, WLC_GET_BSSID, bssid, sizeof(bssid)) == 0) {
		if ( !(!bssid[0] && !bssid[1] && !bssid[2] && !bssid[3] && !bssid[4] && !bssid[5]) ) {
			snprintf(bssid_str, sizeof(bssid_str), "%02X:%02X:%02X:%02X:%02X:%02X", 
				(unsigned char)bssid[0], (unsigned char)bssid[1],
				(unsigned char)bssid[2], (unsigned char)bssid[3],
				(unsigned char)bssid[4], (unsigned char)bssid[5]);
		}
	}

	return bssid_str;
}

sta_info_t *wl_sta_info(char *ifname, struct ether_addr *ea)
{
	static char buf[sizeof(sta_info_t)];
	sta_info_t *sta = NULL;

	strcpy(buf, "sta_info");
	memcpy(buf + strlen(buf) + 1, (void *)ea, ETHER_ADDR_LEN);

	if (!wl_ioctl(ifname, WLC_GET_VAR, buf, sizeof(buf))) {
		sta = (sta_info_t *)buf;
		sta->ver = dtoh16(sta->ver);

		/* Report unrecognized version */
		if (sta->ver > WL_STA_VER) {
			dbg(" ERROR: unknown driver station info version %d\n", sta->ver);
			return NULL;
		}

		sta->len = dtoh16(sta->len);
		sta->cap = dtoh16(sta->cap);
#ifdef RTCONFIG_BCMARM
		sta->aid = dtoh16(sta->aid);
#endif
		sta->flags = dtoh32(sta->flags);
		sta->idle = dtoh32(sta->idle);
		sta->rateset.count = dtoh32(sta->rateset.count);
		sta->in = dtoh32(sta->in);
		sta->listen_interval_inms = dtoh32(sta->listen_interval_inms);
#ifdef RTCONFIG_BCMARM
		sta->ht_capabilities = dtoh16(sta->ht_capabilities);
		sta->vht_flags = dtoh16(sta->vht_flags);
#endif
	}

	return sta;
}

#define ETHER_ADDR_STR_LEN       18
int get_wl_sta_list(void)
{
	char tmp[128], prefix[] = "wlXXXXXXXXXX_";
	char *name;
	struct maclist *auth = NULL;
	int mac_list_size;
	int i;
	char ea[ETHER_ADDR_STR_LEN];
	char name_vif[] = "wlX.Y_XXXXXXXXXX";
	int ii;
	sta_info_t *sta;
	int unit = 0;
	int unit_in = 0;
	char word[256], *next;
	char word_in[256], *next_in;
	char brMac[32] = {0};
	char ifAlias[16] = {0};
	json_object *root = NULL;
	json_object *brMacObj = NULL;
	json_object *bandObj = NULL;
	json_object *staObj = NULL;
	int ret = 0;
	time_t ts;
	int pass_entry = 0;

	time(&ts);
	
	snprintf(brMac, sizeof(brMac), "%s", get_lan_hwaddr());

	root = json_object_new_object();
	brMacObj = json_object_new_object();

	foreach (word, nvram_safe_get("wl_ifnames"), next) {
		bandObj = NULL;
		snprintf(prefix, sizeof(prefix), "wl%d_", unit);
		name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

#ifdef RTCONFIG_WIRELESSREPEATER
		if ((sw_mode() == SW_MODE_REPEATER)
			&& (nvram_get_int("wlc_band") == unit))
		{
			memset(name_vif, 0, sizeof(name_vif));
			snprintf(name_vif, sizeof(name_vif), "wl%d.%d", unit, 1);
			name = name_vif;
		}
#endif

		if (!strlen(name))
			goto exit;

		/* buffers and length */
		mac_list_size = sizeof(auth->count) + MAX_STA_COUNT * sizeof(struct ether_addr);
		auth = malloc(mac_list_size);

		if (!auth)
			goto exit;

		memset(auth, 0, mac_list_size);

		/* query wl for authenticated sta list */
		strcpy((char*)auth, "authe_sta_list");
		if (wl_ioctl(name, WLC_GET_VAR, auth, mac_list_size))
			goto exit;

		memset(ifAlias, 0, sizeof(ifAlias));
		if_nametoalias(name, &ifAlias[0], sizeof(ifAlias));

		/* build authenticated sta list */
		for (i = 0; i < auth->count; ++i) {
			sta = wl_sta_info(name, &auth->ea[i]);
			if (!sta) continue;
			if (!(sta->flags & WL_STA_ASSOC) && !sta->in) continue;
	
			ether_etoa((void *)&auth->ea[i], ea);

			/* filter sta's mac is same as ours */
#if defined(RTCONFIG_BCMARM) && defined(RTCONFIG_PROXYSTA) && defined(RTCONFIG_DPSTA)
			unit_in = 0;
			pass_entry = 0;
			foreach (word_in, nvram_safe_get("wl_ifnames"), next_in) {
				SKIP_ABSENT_BAND_AND_INC_UNIT(unit);
				if (!strcmp(ea, get_pap_bssid(unit_in))) {
					pass_entry = 1;
					break;
				}
				unit_in++;
			}

			if (pass_entry) continue;
#endif

			if (!bandObj)
				bandObj = json_object_new_object();
				staObj = json_object_new_object();
				json_object_object_add(bandObj, ea, staObj);	
		}
		
		if (bandObj)
			json_object_object_add(brMacObj, ifAlias, bandObj);

		for (i = 1; i < 4; i++) {
			bandObj = NULL;
#ifdef RTCONFIG_WIRELESSREPEATER
			if ((sw_mode() == SW_MODE_REPEATER)
				&& (unit == nvram_get_int("wlc_band")) && (i == 1))
				break;
#endif
			memset(prefix, 0, sizeof(prefix));
			snprintf(prefix, sizeof(prefix), "wl%d.%d_", unit, i);
			if (nvram_match(strcat_r(prefix, "bss_enabled", tmp), "1"))
			{
				memset(name_vif, 0, sizeof(name_vif));
				snprintf(name_vif, sizeof(name_vif), "wl%d.%d", unit, i);

				memset(auth, 0, mac_list_size);

				memset(ifAlias, 0, sizeof(ifAlias));
       		 		if_nametoalias(name_vif, &ifAlias[0], sizeof(ifAlias)); 

				/* query wl for authenticated sta list */
				strcpy((char*)auth, "authe_sta_list");
				if (wl_ioctl(name_vif, WLC_GET_VAR, auth, mac_list_size))
					goto exit;

				for (ii = 0; ii < auth->count; ii++) {
					sta = wl_sta_info(name_vif, &auth->ea[ii]);
					if (!sta) continue;
					if (!(sta->flags & WL_STA_ASSOC) && !sta->in) continue;

					ether_etoa((void *)&auth->ea[ii], ea);

					/* filter sta's mac is same as ours */
#if defined(RTCONFIG_BCMARM) && defined(RTCONFIG_PROXYSTA) && defined(RTCONFIG_DPSTA)
					unit_in = 0;
					pass_entry = 0;
					foreach (word_in, nvram_safe_get("wl_ifnames"), next_in) {
						SKIP_ABSENT_BAND_AND_INC_UNIT(unit);
						if (!strcmp(ea, get_pap_bssid(unit_in))) {
							pass_entry = 1;
							break;
						}
						unit_in++;
					}

					if (pass_entry) continue;
#endif
	
					if (!bandObj)
					bandObj = json_object_new_object();
					staObj = json_object_new_object();
					json_object_object_add(bandObj, ea, staObj);
				}

				if (bandObj)
					json_object_object_add(brMacObj, ifAlias, bandObj);
			}
		}

		if (auth) {
			free(auth);
			auth = NULL;
		}

		unit++;
	}

	if (brMacObj) {
		json_object_object_add(root, brMac, brMacObj);
		json_object_to_file(WLSTA_JSON_FILE, root);
	}
	ret = 1;
	/* error/exit */
exit:
	json_object_put(root);

	if (auth) free(auth);

	return ret;
}

int wlif_status(char *ifname)
{
	unsigned char bssid[6];
	unsigned char bssid_null[6] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	char ure_mac[18]={0};
	
	if (wl_ioctl(ifname, WLC_GET_BSSID, &bssid, ETHER_ADDR_LEN) < 0)
	{
		cprintf("can't get %s status\n", ifname);
		return -1;
	}
	else{		
		memset(ure_mac, 0x00, sizeof(ure_mac));
		sprintf(ure_mac, "%02X:%02X:%02X:%02X:%02X:%02X",
							(unsigned char)bssid[0],
							(unsigned char)bssid[1],
							(unsigned char)bssid[2],
							(unsigned char)bssid[3],
							(unsigned char)bssid[4],
							(unsigned char)bssid[5]);
		//cprintf("ure_mac= %s\n", ure_mac);

		if (!memcmp(&bssid, bssid_null, 6)){
			return 0;
		}
		else
			return 1;		
	}
	return -1;
}

int get_wlan_service_status(int bssidx, int vifidx)
{
	FILE *fp = NULL;
	char maxassoc_file[128]={0};
	char buf[64]={0};
	char maxassoc[64]={0};
	char tmp[128] = {0}, prefix[] = "wlXXXXXXXXXX_";
	char *ifname = NULL;

	if(vifidx > 0)
		snprintf(prefix, sizeof(prefix), "wl%d.%d_", bssidx, vifidx);
	else
		snprintf(prefix, sizeof(prefix), "wl%d", bssidx);

	ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));


	snprintf(maxassoc_file, sizeof(maxassoc_file), "/tmp/maxassoc.%s", ifname);

	doSystem("wl -i %s maxassoc > %s", ifname, maxassoc_file);

	if ((fp = fopen(maxassoc_file, "r")) != NULL) {
		fscanf(fp, "%s", buf);
		fclose(fp);
	}
	sscanf(buf, "%s", maxassoc);

	return atoi(maxassoc);
}

void set_wlan_service_status(int bssidx, int vifidx, int enabled)
{

	char tmp[128]={0}, prefix[] = "wlXXXXXXXXXX_", wlprefix[] = "wlXXXXXXXXXX_";
	char *ifname = NULL;

	if(vifidx > 0)
		snprintf(prefix, sizeof(prefix), "wl%d.%d_", bssidx, vifidx);
	else
		snprintf(prefix, sizeof(prefix), "wl%d", bssidx);

	memset(wlprefix, 0x00, sizeof(wlprefix));
	snprintf(wlprefix, sizeof(wlprefix), "wl%d_", bssidx);	

	ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));	

	if (enabled == 0) {
		doSystem("wl -i %s deauthenticate", ifname);
		doSystem("wl -i %s maxassoc 0", ifname);
	}
	else {
#ifdef HND_ROUTER
		doSystem("wl -i %s maxassoc %d", ifname, nvram_get_int(strcat_r(wlprefix, "cfg_maxassoc", tmp)));
#else										
		doSystem("wl -i %s maxassoc %d", ifname, nvram_get_int(strcat_r(wlprefix, "maxassoc", tmp)));
#endif			
	}
}

void set_wl_macfilter(int bssidx, int vifidx, struct maclist_r *client_list, int block)
{
		int ret = 0, size = 0, i = 0, cnt = 0, match = 0;
		char *wlif_name = NULL;
		char tmp[128], prefix[] = "wlXXXXXXXXXX_";
		struct ether_addr *ea = NULL;

		int macmode = 0;
		char maclist_buf[WLC_IOCTL_MAXLEN] = {0};
		struct maclist *maclist = (struct maclist *) maclist_buf;
		struct maclist *static_maclist= NULL;

		if(vifidx > 0)
			snprintf(prefix, sizeof(prefix), "wl%d.%d_", bssidx, vifidx);
		else
			snprintf(prefix, sizeof(prefix), "wl%d", bssidx);

		wlif_name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));


		ret = wl_ioctl(wlif_name, WLC_GET_MACMODE, &macmode, sizeof(macmode));
		if(ret < 0) 
		{
			dbG("[WARNING] %s get macmode error!!!\n", wlif_name);
			return;
		}

		cprintf("[%s] macmode = %s\n",
		wlif_name,
		macmode==WLC_MACMODE_DISABLED ? "DISABLE" :
		macmode==WLC_MACMODE_DENY ? "DENY" : "ALLOW");

		ret = wl_ioctl(wlif_name, WLC_GET_MACLIST, (void *)maclist, sizeof(maclist_buf));
		if(ret < 0) 
		{
			cprintf("[WARNING] %s get maclist error!!!\n", wlif_name);
			return;
		}

		if (maclist->count > 0 && maclist->count < 128) 
		{
			size = sizeof(uint) + sizeof(struct ether_addr) * (maclist->count);

			static_maclist = (struct maclist *)malloc(size);

			if (!(static_maclist)) 
			{
				cprintf("%s malloc [%d] failure... \n", __FUNCTION__, size);
				return;
			}

			memcpy(static_maclist, maclist, size);
			static_maclist->count = maclist->count;
			memset(maclist, 0x00, sizeof(maclist_buf));
			//maclist = static_maclist;

			for (size = 0; size < maclist->count; size++) {
				cprintf("%d [%s] (%d)mac:"MACF"\n",__LINE__, wlif_name, size, ETHER_TO_MACF(maclist->ea[size]));
			}	

		} 
		else if (maclist->count != 0) 
		{
			cprintf("Err: %s maclist cnt [%d] too large\n",
			wlif_name, maclist->count);
			return;
		}
		else if (maclist->count == 0) {
			cprintf("maclist is empty\n");
		}

		if (block == 0 && macmode == WLC_MACMODE_DISABLED)
			macmode = WLC_MACMODE_DISABLED;
		else if (macmode == WLC_MACMODE_DENY || macmode == WLC_MACMODE_DISABLED)
			macmode = WLC_MACMODE_DENY;
		else
			macmode = WLC_MACMODE_ALLOW;


		if (macmode == WLC_MACMODE_DENY || macmode == WLC_MACMODE_DISABLED) 
		{
			if (block == 1) 
			{              	
				if (static_maclist) 
				{
					cprintf("Deny mode: Adding static maclist\n");
					maclist->count = static_maclist->count;
					memcpy(maclist_buf, static_maclist,
					sizeof(uint) + ETHER_ADDR_LEN * (maclist->count));

					ea = &(maclist->ea[maclist->count]);

					for (cnt = 0; cnt < client_list->count; cnt++) {
						cprintf("Deny mode: static mac[%d] addr:"MACF"\n", cnt,ETHER_TO_MACF(client_list->ea[cnt]));
						match = 0;
						for  (i = 0; i < static_maclist->count ; i++) {
							cprintf("Checking "MACF"\n", ETHER_TO_MACF(static_maclist->ea[i]));
							if (memcmp(&(client_list->ea[cnt]), &(static_maclist->ea[i]), sizeof(struct ether_addr)) == 0) {
								cprintf("MATCH maclist "MACF"\n", ETHER_TO_MACF(client_list->ea[cnt]));
								match = 1;
								break;
							}
						}
						if (!match) {
							memcpy(ea, &(client_list->ea[cnt]), sizeof(struct ether_addr));
							maclist->count++;
							cprintf("Adding to Deny list: cnt[%d] addr:"MACF"\n",maclist->count, ETHERP_TO_MACF(ea));
							ea++;
						}
					}
				}
				else {
					if (client_list->count > 0) {
					maclist->count = client_list->count;
					memcpy(maclist, client_list, sizeof(uint) + ETHER_ADDR_LEN * (client_list->count));	                			                	
					}
				}
			}
			else if (block == 0) 
			{

				ea = &(maclist->ea[0]);
				maclist->count = 0;
				if (static_maclist) 
				{

					for (cnt = 0; cnt < static_maclist->count; cnt++) {
						cprintf("%d:Deny mode: static mac[%d] addr:"MACF"\n",__LINE__, cnt,
						ETHER_TO_MACF(static_maclist->ea[cnt]));
						match = 0;
						for  (i = 0; i < client_list->count ; i++) {
							cprintf("Checking "MACF"\n", ETHER_TO_MACF(client_list->ea[i]));
							if (memcmp(&(client_list->ea[i]), &(static_maclist->ea[cnt]),  sizeof(struct ether_addr)) == 0) 
							{
								cprintf("MATCH maclist "MACF"\n", ETHER_TO_MACF(client_list->ea[i]));
								match = 1;
								break;
							}
							else {
								cprintf("Remove from Deny list: cnt[%d] addr:"MACF"\n",
								i,ETHER_TO_MACF(client_list->ea[i]));
							}
						}
						if (!match) 
						{
							memcpy(ea, &(static_maclist->ea[cnt]), sizeof(struct ether_addr));
							maclist->count++;
							cprintf("Adding to Deny list: cnt[%d] addr:"MACF"\n",
							maclist->count, ETHERP_TO_MACF(ea));
							ea++;
						}	                
					}
				}
			}      
		}
		else {  //ALLOW MODE

			if (block == 1) 
			{
				ea = &(maclist->ea[0]);
				maclist->count = 0;

				if (static_maclist) 
				{

					for (cnt = 0; cnt < static_maclist->count; cnt++) {
						cprintf("%d:Allow mode: static mac[%d] addr:"MACF"\n",__LINE__, cnt,
						ETHER_TO_MACF(static_maclist->ea[cnt]));
						match = 0;
						for  (i = 0; i < client_list->count ; i++) {
							cprintf("%d Checking client_list :"MACF", static_maclist:"MACF"\n",__LINE__, ETHER_TO_MACF(client_list->ea[i]), ETHER_TO_MACF(static_maclist->ea[cnt]));
							if (memcmp(&(client_list->ea[i]), &(static_maclist->ea[cnt]),  sizeof(struct ether_addr)) == 0) {
							cprintf("%d MATCH maclist "MACF"\n", __LINE__,ETHER_TO_MACF(static_maclist->ea[cnt]));
							match = 1;
							break;
							}
						}	                        
						if (!match) {
							memcpy(ea, &(static_maclist->ea[cnt]), sizeof(struct ether_addr));
							maclist->count++;
							cprintf("%d Adding to Allow list: cnt[%d] addr:"MACF"\n",__LINE__,
							maclist->count, ETHERP_TO_MACF(ea));
							ea++;
						}	                
					}
				}
			}
			else if (block == 0) 
			{
				if (static_maclist) 
				{
					cprintf("Allow mode: Adding static maclist\n");
					maclist->count = static_maclist->count;
					memcpy(maclist_buf, static_maclist,
					sizeof(uint) + ETHER_ADDR_LEN * (maclist->count));


					ea = &(maclist->ea[maclist->count]);

					for (cnt = 0; cnt < client_list->count; cnt++) {
						cprintf("Allow mode: static mac[%d] addr:"MACF"\n", cnt,ETHER_TO_MACF(client_list->ea[cnt]));
						match = 0;
						
						for  (i = 0; i < static_maclist->count ; i++) {
							cprintf("Checking "MACF"\n", ETHER_TO_MACF(static_maclist->ea[i]));
							if (memcmp(&(client_list->ea[cnt]), &(static_maclist->ea[i]), sizeof(struct ether_addr)) == 0) {
								cprintf("MATCH maclist "MACF"\n", ETHER_TO_MACF(client_list->ea[cnt]));
								match = 1;
								break;
							}
						}
						if (!match) {
							memcpy(ea, &(client_list->ea[cnt]), sizeof(struct ether_addr));
							maclist->count++;
							cprintf("Adding to Allow list: cnt[%d] addr:"MACF"\n",
							maclist->count, ETHERP_TO_MACF(ea));
							ea++;
						}
					}
				}
				else {
					if (client_list->count > 0) {
						maclist->count = client_list->count;
						memcpy(maclist, client_list, sizeof(uint) + ETHER_ADDR_LEN * (client_list->count));	                			                	
					}
				}
			}
		}

		cprintf("maclist count[%d] \n", maclist->count);
		for (cnt = 0; cnt < maclist->count; cnt++) {
			cprintf("%d maclist: "MACF"\n",__LINE__,
			ETHER_TO_MACF(maclist->ea[cnt]));
		}

		doSystem("wl -i %s mac none",wlif_name);

		if (maclist->count > 0) {
			ret = wl_ioctl(wlif_name, WLC_SET_MACMODE, &macmode, sizeof(macmode));
			if(ret < 0) {
				cprintf("[WARNING] %s set macmode[%d] error!!!\n", wlif_name, macmode);
				goto err;
			}

			ret = wl_ioctl(wlif_name, WLC_SET_MACLIST, maclist, sizeof(maclist_buf));
			if (ret < 0) {
				cprintf("Err: [%s] set maclist...\n", wlif_name);
			}
		}
err:
		if (static_maclist != NULL)
			free(static_maclist);
}
#endif





