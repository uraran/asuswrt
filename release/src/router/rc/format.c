#include <rc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(RTCONFIG_VPN_FUSION)
#include <vpnc_fusion.h>
extern int vpnc_load_profile(VPNC_PROFILE *list, const int list_size);
#endif

void adjust_url_urlelist(void)
{
	char *nv, *nvp, *b, *chk;
	char *url;
	char  replacebox[2048], rerule[256];
	int   cnt = 0;
	int   RESAVE = OFF;
	
	nv = nvp = strdup(nvram_safe_get("url_rulelist"));
	
	/* Change setting format 
	From:[<str1<str2<str3]
	To:  [<1>ALL>str1<1>ALL>str2<1>ALL>str3] 
	*/
	memset(replacebox, 0, sizeof(replacebox));
	while (nvp && (b = strsep(&nvp, "<")) != NULL) {
		chk = strdup(b);
		//dbg("[%s(%d)] %s\n", __FUNCTION__, __LINE__, chk);
		while( *chk != '\0') {
			if(*chk == '>') cnt++;
			chk++;
		}
		if (cnt != 2) {
			if (vstrsep(b, ">", &url) != 1) continue;
			if (*url) {
				memset(rerule, 0, sizeof(rerule));
				snprintf(rerule, sizeof(rerule), "<1>ALL>%s", url);
				//dbg("[%s(%d)] %s\n", __FUNCTION__, __LINE__, rerule);
				strcat(replacebox, rerule);
				RESAVE = ON;
			}
		}
		cnt = 0;
	}
	if (RESAVE) 
		nvram_set("url_rulelist", replacebox);
	free(nv);
}

void adjust_ddns_config(void)
{
	char *server;

	server = nvram_safe_get("ddns_server_x");
	if(!strcmp(server, "WWW.GOOGLE-DDNS.COM") || 
	   !strcmp(server, "WWW.GOOGLE-DOMAINS.COM")) {
		nvram_set("ddns_server_x", "DOMAINS.GOOGLE.COM");
	}
	
}

void adjust_access_restrict_config(void)
{
	char *nv, *nvp, *b, *chk;
	char *ipAddr;
	char *http_list;
	char *restrict_list;
	char  replacebox[2048], rerule[256];
	int   cnt = 0;
	int   RESAVE = OFF;
	int   nCount = 0;
	
	http_list = nvram_safe_get("http_clientlist");
	restrict_list = nvram_safe_get("restrict_rulelist");
	
	if ( (restrict_list[0] == '\0' && nvram_get_int("enable_acc_restriction") == 0) && 
	     (http_list[0] != '\0') ) {
		
		nvram_set_int("enable_acc_restriction", nvram_get_int("http_client"));
		
		nv = nvp = strdup(nvram_safe_get("http_clientlist"));
		
		/* Change setting format 
		From:[<ipAddr1<ipAddr2<ipAddr3]
		To:  [<1>ipAddr1>7<1>ipAddr2>7<1>ipAddr3>7]
		*/
		memset(replacebox, 0, sizeof(replacebox));
		while (nvp && (b = strsep(&nvp, "<")) != NULL) {
			chk = strdup(b);
			//dbg("[%s(%d)] %s\n", __FUNCTION__, __LINE__, chk);
			while( *chk != '\0') {
				if(*chk == '>') cnt++;
				chk++;
			}
			if (cnt != 2) {
				if (vstrsep(b, ">", &ipAddr) != 1) continue;
				if (*ipAddr) {
					memset(rerule, 0, sizeof(rerule));
					snprintf(rerule, sizeof(rerule), "<1>%s>7", ipAddr);
					//dbg("[%s(%d)] %s\n", __FUNCTION__, __LINE__, rerule);
					nCount = sizeof(replacebox) - strlen(replacebox);
					strncat(replacebox, rerule, nCount-1);
					RESAVE = ON;
				}
			}
			cnt = 0;
		}
		if (RESAVE) 
			nvram_set("restrict_rulelist", replacebox);
		free(nv);
	}
}

#if defined(RTCONFIG_VPN_FUSION)
static VPNC_PROFILE vpnc_profile_tmp[MAX_VPNC_PROFILE] = {0};
static int vpnc_prof_cnt_tmp;

static int _find_active_vpnc_id()
{
	char *vpnc_proto, *vpnc_heartbeat_x, *vpn_clientx_eas, *vpnc_pppoe_username, *vpnc_pppoe_passwd;
	int i, proto_id = -1, ovpn_idx;
	
	vpnc_proto = nvram_safe_get("vpnc_proto");
	
	if(vpnc_proto[0] == '\0' || !strcmp(vpnc_proto, "disable"))
		return -1;	//no active vpn client

	if(!strcmp(vpnc_proto, "pptp"))
		proto_id = VPNC_PROTO_PPTP;
	if(!strcmp(vpnc_proto, "l2tp"))
		proto_id = VPNC_PROTO_L2TP;
	if(!strcmp(vpnc_proto, "openvpn"))
		proto_id = VPNC_PROTO_OVPN;
	
	switch(proto_id)		
	{
		case VPNC_PROTO_PPTP:
		case VPNC_PROTO_L2TP:
			{
				vpnc_heartbeat_x = nvram_safe_get("vpnc_heartbeat_x");
				vpnc_pppoe_username = nvram_safe_get("vpnc_pppoe_username");
				vpnc_pppoe_passwd = nvram_safe_get("vpnc_pppoe_passwd");
				for(i = 0; i < vpnc_prof_cnt_tmp; ++i)
				{
					if(vpnc_profile_tmp[i].protocol == proto_id && 
						!strcmp(vpnc_profile_tmp[i].basic.server, vpnc_heartbeat_x) &&
						!strcmp(vpnc_profile_tmp[i].basic.username, vpnc_pppoe_username) &&
						!strcmp(vpnc_profile_tmp[i].basic.password, vpnc_pppoe_passwd))
					{
						return i;
					}
				}
			}
			break;
		case VPNC_PROTO_OVPN:
			{
				vpn_clientx_eas = nvram_safe_get("vpn_clientx_eas");
				ovpn_idx = atoi(vpn_clientx_eas);
				
				for(i = 0; i < vpnc_prof_cnt_tmp; ++i)
				{
					if(vpnc_profile_tmp[i].protocol == proto_id && 
						vpnc_profile_tmp[i].config.ovpn.ovpn_idx == ovpn_idx)
					{
						return i;
					}
				}
			}
			break;
		default:
			break;
	}
	return -1;
}

void adjust_vpnc_config(void)
{
	char *vpnc_clientlist, *vpnc_clientlist_ex, *vpnc_default_wan;
	char *vpnc_dev_policy_list;
	int active_id, i, default_wan_idx = 0;
	char buf[1024];
	char *nv = NULL, *nvp = NULL, *b = NULL, *mac, *static_ip;

	vpnc_clientlist = nvram_safe_get("vpnc_clientlist");
	if(vpnc_clientlist[0] != '\0')
	{
		vpnc_prof_cnt_tmp = vpnc_load_profile(vpnc_profile_tmp, MAX_VPNC_PROFILE);

		vpnc_clientlist_ex = nvram_safe_get("vpnc_clientlist_ex");
		vpnc_default_wan = nvram_safe_get("vpnc_default_wan");

		if(vpnc_clientlist_ex[0] == '\0')	//no this attribute
		{
			//find active id
			active_id = _find_active_vpnc_id();

			memset(buf, 0, sizeof(buf));
			
			for(i = 0; i < vpnc_prof_cnt_tmp; ++i)
			{
				snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), i? "<%d>%d>>": "%d>%d>>", 
					(i == active_id)? 1: 0, VPNC_UNIT_BASIC + i);
				if(i == active_id)
				{
					default_wan_idx = VPNC_UNIT_BASIC + i;
				}
			}
			
			nvram_set("vpnc_clientlist_ex", buf);
			snprintf(buf, sizeof(buf), "%d", default_wan_idx);
			nvram_set("vpnc_default_wan", buf);			
		}		
	}

	vpnc_dev_policy_list = nvram_safe_get("vpnc_dev_policy_list");
	
	if(vpnc_dev_policy_list[0] == '\0' )	//have dhcp_staticlist but no vpnc_dev_policy_list
	{
		nv = nvp = strdup(nvram_safe_get("dhcp_staticlist"));
		memset(buf, 0, sizeof(buf));
		i = 0;
		
		while (nv && (b = strsep(&nvp, "<")) != NULL ) {
			if (vstrsep(b, ">", &mac, &static_ip) < 2)
				continue;

			snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), i?"<0>%s>>0": "0>%s>>0", mac);

			++i;
		}
		free(nv);

		nvram_set("vpnc_dev_policy_list", buf);
	}
	
}
#endif

