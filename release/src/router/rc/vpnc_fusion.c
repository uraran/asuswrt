/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Copyright 2004, ASUSTeK Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND ASUS GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 */
#include "rc.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>															
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>

#include "vpnc_fusion.h"

VPNC_PROFILE vpnc_profile[MAX_VPNC_PROFILE] = {0};

int vpnc_profile_num = 0;

static int vpnc_get_dev_policy_list(VPNC_DEV_POLICY *list, const int list_size);
int vpnc_set_policy_by_ifname(const char *vpnc_ifname, const int action);
int stop_vpnc_by_unit(const int unit);
int set_routing_table(const int cmd, const int vpnc_id);
int set_routing_rule_by_mac(const VPNC_ROUTE_CMD cmd, const char *mac, const int vpnc_id);

int vpnc_pppstatus(const int unit)
{
	char statusfile[sizeof("/var/run/ppp-vpnXXXXXXXXXX.status")];

	snprintf(statusfile, sizeof(statusfile), "/var/run/ppp-vpn%d.status", unit);
	return _pppstatus(statusfile);
}

int
start_vpnc(void)	//start up all active vpnc profile
{
	int i;
	
	//init vpnc profile
	vpnc_init();

	for(i = 0; i < vpnc_profile_num; ++i)
	{
		if(vpnc_profile[i].active)
			start_vpnc_by_unit(i);
	}
	return 0;
}

void
stop_vpnc(void)	//start up all active vpnc profile
{
	int i;
	
	//init vpnc profile
	vpnc_init();

	for(i = 0; i < vpnc_profile_num; ++i)
	{
		if(vpnc_profile[i].active)
			stop_vpnc_by_unit(i);
	}
	
	return;
}

/*******************************************************************
* NAME: set_internet_as_default_wan
* AUTHOR: Andy Chiu
* CREATE DATE: 2017/1/24
* DESCRIPTION: set the default routing rule for internet wan
* INPUT:  is_default_wan: 0:not default wan, 1: default wan
* OUTPUT:  
* RETURN:  0: success, -1: failed
* NOTE:
*******************************************************************/
int set_internet_as_default_wan(const int is_default_wan )
{
	char wan_prefix[] = "wanXXXXXXXX_";
	char tmp[100];
	char *wan_ifname = NULL, *wan_proto = NULL;

	snprintf(wan_prefix, sizeof(wan_prefix), "wan%d_", wan_primary_ifunit());

	//get wan interface name
	wan_proto = nvram_safe_get(strcat_r(wan_prefix, "proto", tmp));

	if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static"))
		wan_ifname = nvram_safe_get(strcat_r(wan_prefix, "ifname", tmp));
	else
		wan_ifname = nvram_safe_get(strcat_r(wan_prefix, "pppoe_ifname", tmp));

	_dprintf("[%s, %d]wan_ifname(%s), is_default_wan(%d)\n", __FUNCTION__, __LINE__, wan_ifname, is_default_wan);

	if(is_default_wan)	//set internet as default route
	{
		/* Reset default gateway route */
		if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static")) {
			route_del(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
			route_add(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
		}
		else if (!strcmp(wan_proto, "pppoe") || !strcmp(wan_proto, "pptp") || !strcmp(wan_proto, "l2tp"))
		{
			char *wan_xgateway = nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp));

			route_del(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
			route_add(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");

			if (strlen(wan_xgateway) > 0 && strcmp(wan_xgateway, "0.0.0.0")) {
				char *wan_xifname = nvram_safe_get(strcat_r(wan_prefix, "ifname", tmp));

				route_del(wan_xifname, 3, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");
				route_add(wan_xifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");
			}			
		}

	}
	else	//remove internet default route rule
	{
		/* Reset default gateway route via PPPoE interface */
		if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static")) {
			route_del(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
			route_add(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
		}
		else if (!strcmp(wan_proto, "pppoe") || !strcmp(wan_proto, "pptp") || !strcmp(wan_proto,  "l2tp"))
		{
			char *wan_xgateway = nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp));
			route_del(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
			route_add(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");

			if (strlen(wan_xgateway) > 0 && strcmp(wan_xgateway, "0.0.0.0")) {
				char *wan_xifname =  nvram_safe_get(strcat_r(wan_prefix, "ifname", tmp));
				route_del(wan_xifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");
				route_add(wan_xifname, 3, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");
			}
		}
	}
	return 0;
}

/*******************************************************************
* NAME: _set_vpnc_as_default_wan
* AUTHOR: Andy Chiu
* CREATE DATE: 2017/1/24
* DESCRIPTION: set vpn client connection as default wan
* INPUT:  unit: the index of vpn client. is_default_wan: 0: not default wan, 1:default wan.
* OUTPUT:  
* RETURN:  0:success, -1:failed
* NOTE:
*******************************************************************/
static int _set_vpnc_as_default_wan(const int unit, const int is_default_wan)
{
	char vpnc_prefix[] = "vpncXXXXXXX_";
	char *vpnc_ifname = NULL;
	char tmp[100];

	snprintf(vpnc_prefix, sizeof(vpnc_prefix), "vpnc%d_", unit);

	vpnc_ifname = nvram_safe_get(strcat_r(vpnc_prefix, "ifname", tmp));
	
	//check vpnc ifname
	if(!vpnc_ifname || vpnc_ifname[0] == '\0')
	{
		_dprintf("[%s, %d] Can not find interface with unit %d\n", __FUNCTION__, __LINE__, unit);
		return -1;
	}

	if(is_default_wan)	//Add default routing rule
		route_add(vpnc_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(vpnc_prefix, "gateway", tmp)), "0.0.0.0");
	else	//remove the deault routing rule
		route_del(vpnc_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(vpnc_prefix, "gateway", tmp)), "0.0.0.0");

	return 0;
		
}

/*******************************************************************
* NAME: change_default_wan_as_vpnc_updown
* AUTHOR: Andy Chiu
* CREATE DATE: 2017/1/24
* DESCRIPTION: change the default wan when a vpn client connection established or lost.
* INPUT:  unit: index of vpn client. up: 1: connected, 0:disconnected
* OUTPUT:  
* RETURN:  0: success, -1:failed
* NOTE:
*******************************************************************/
int change_default_wan_as_vpnc_updown(const int unit, const int up)
{
	char tmp[100];

	snprintf(tmp, sizeof(tmp), "%d", unit);

	if(nvram_match("vpnc_default_wan", tmp))
	{
		if(up)
			_set_vpnc_as_default_wan(unit, 1);
		else
			_set_vpnc_as_default_wan(unit, 0);
	}
	return 0;
}

/*******************************************************************
* NAME: change_default_wan
* AUTHOR: Andy Chiu
* CREATE DATE: 2017/1/24
* DESCRIPTION: when web ui change default wan, call this function to handle default wan
* INPUT:  
* OUTPUT:  
* RETURN:  0:success, -1:failed
* NOTE:
*******************************************************************/
int change_default_wan()
{
	char tmp[100];
	int default_wan_ori, default_wan_new;

	//get default_wan
	default_wan_ori = nvram_get_int("vpnc_default_wan");
	default_wan_new = nvram_get_int("vpnc_default_wan_tmp");

	//_dprintf("[%s, %d]vpnc_default_wan old=%d, new=%d\n", __FUNCTION__, __LINE__, default_wan_ori, default_wan_new);

	if(!default_wan_new)	//vpnc - >internet
	{
		set_internet_as_default_wan(1);

		if(default_wan_ori)
			_set_vpnc_as_default_wan(default_wan_ori, 0);
	}
	else if( !default_wan_ori)	//internet -> vpnc
	{
		set_internet_as_default_wan(0);

		if(default_wan_new)
			_set_vpnc_as_default_wan(default_wan_new, 1);
	}
	else	//vpnc -> vpnc
	{
		if(default_wan_ori)
			_set_vpnc_as_default_wan(default_wan_ori, 0);

		if(default_wan_new)
			_set_vpnc_as_default_wan(default_wan_new, 1);
		
	}

	snprintf(tmp, sizeof(tmp), "%d", default_wan_new);
	nvram_set("vpnc_default_wan", tmp);
	return 0;
}

int vpnc_ppp_linkunit(const char *linkname)
{
	if(!linkname)
		return -1;
	else if (strncmp(linkname, "vpn", 3))
		return -1;
	else if (!isdigit(linkname[3]))
		return -1;
	else
		return atoi(linkname + 3);
}

int vpnc_ppp_linkunit_by_ifname(const char *ifname)
{
	if(!ifname)
		return -1;
	else if (strncmp(ifname, "ppp", 3))
		return -1;
	else if (!isdigit(ifname[3]))
		return -1;
	else
		return atoi(ifname + 3);
}


void update_vpnc_state(const int vpnc_idx, const int state, const int reason)
{
	char tmp[100];
	char prefix[12];

	_dprintf("%s(%d, %d, %d)\n", __FUNCTION__, vpnc_idx, state, reason);

	//create prefix
	snprintf(prefix, sizeof(prefix), "vpnc%d_", vpnc_idx);

	nvram_set_int(strcat_r(prefix, "state_t", tmp), state);
	nvram_set_int(strcat_r(prefix, "sbstate_t", tmp), 0);

	if (state == WAN_STATE_STOPPED) {
		// Save Stopped Reason
		// keep ip info if it is stopped from connected
		nvram_set_int(strcat_r(prefix, "sbstate_t", tmp), reason);
	}
	else if(state == WAN_STATE_STOPPING) {
		snprintf(tmp, sizeof(tmp), "/var/run/ppp-vpn%d.status", vpnc_idx);
		unlink(tmp);
	}
}

int vpnc_update_resolvconf(const int unit)
{
	FILE *fp;
	char tmp[32];
	char vpnc_prefix[] = "vpncXXXX_";
	char word[256], *next;
	int lock;
	char *wan_dns, *wan_xdns;

	lock = file_lock("resolv");

	if (!(fp = fopen("/tmp/resolv.conf", "w+"))) {
		perror("/tmp/resolv.conf");
		file_unlock(lock);
		return errno;
	}

	snprintf(vpnc_prefix, sizeof(vpnc_prefix), "vpnc%d_", unit);

	wan_dns = nvram_safe_get(strcat_r(vpnc_prefix, "dns", tmp));
	wan_xdns = nvram_safe_get(strcat_r(vpnc_prefix, "xdns", tmp));

	foreach(word, (*wan_dns ? wan_dns : wan_xdns), next)
		fprintf(fp, "nameserver %s\n", word);

	fclose(fp);

	file_unlock(lock);

	reload_dnsmasq();

	return 0;
}

void vpnc_add_firewall_rule(const int unit, const char *vpnc_ifname)
{
	char tmp[100], vpnc_prefix[] = 
"vpncXXXX_", wan_prefix[] = "wanXXXXXXXXXX_";
	char *wan_proto = NULL;
	char lan_if[IFNAMSIZ+1];

	if(!vpnc_ifname)
		return;

	//generate prefix, vpncX_
	snprintf(vpnc_prefix, sizeof(vpnc_prefix), "vpnc%d_", unit);

	//get lan interface name
	snprintf(lan_if, sizeof(lan_if), "%s", nvram_safe_get("lan_ifname"));

	if (check_if_file_exist(strcat_r("/tmp/ppp/link.", vpnc_ifname, tmp)))
	{
		snprintf(wan_prefix, sizeof(wan_prefix), "wan%d_", wan_primary_ifunit());
		wan_proto = nvram_safe_get(strcat_r(wan_prefix, "proto", tmp));
		if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static"))
			//eval("iptables", "-I", "FORWARD", "-p", "tcp", "--syn", "-j", "TCPMSS", "--clamp-mss-to-pmtu");
			eval("iptables", "-I", "FORWARD", "-p", "tcp", "--tcp-flags", "SYN,RST", "SYN", "-j", "TCPMSS", "--clamp-mss-to-pmtu");
#ifdef RTCONFIG_BCMARM
		else	/* mark tcp connection to bypass CTF */
#ifdef HND_ROUTER
			if (nvram_match("fc_disable", "0") && nvram_match("fc_pt_war", "1"))
#else
			if (nvram_match("ctf_disable", "0"))
#endif
			eval("iptables", "-t", "mangle", "-A", "FORWARD", "-p", "tcp", 
				"-m", "state", "--state", "NEW","-j", "MARK", "--set-mark", "0x01/0x7");
#endif

		eval("iptables", "-A", "FORWARD", "-o", (char*)vpnc_ifname, "!", "-i", lan_if, "-j", "DROP");
		eval("iptables", "-t", "nat", "-I", "PREROUTING", "-d", 
			nvram_safe_get(strcat_r(vpnc_prefix, "ipaddr", tmp)), "-j", "VSERVER");
		eval("iptables", "-t", "nat", "-I", "POSTROUTING", "-o", 
			(char*)vpnc_ifname, "!", "-s", nvram_safe_get(strcat_r(vpnc_prefix, "ipaddr", tmp)), "-j", "MASQUERADE");

		//Add dev policy
		vpnc_set_policy_by_ifname(vpnc_ifname, 1);
	}
}

void
vpnc_up(const int unit, const char *vpnc_ifname)
{
	char tmp[100], prefix[] = "vpnc_", wan_prefix[] = "wanXXXXXXXXXX_", vpnc_prefix[] = "vpncXXXX_";
	char *wan_ifname = NULL, *wan_proto = NULL;
	int default_wan;	
	
	if(!vpnc_ifname)
		return;

	_dprintf("[%s, %d]unit=%d, vpnc_ifname=%s\n", __FUNCTION__, __LINE__, unit, vpnc_ifname);
	
	//get vpnc_prefix and wan_prefix
	snprintf(vpnc_prefix, sizeof(vpnc_prefix), "vpnc%d_", unit);
	snprintf(wan_prefix, sizeof(wan_prefix), "wan%d_", wan_primary_ifunit());

	//get wan interface name
	wan_proto = nvram_safe_get(strcat_r(wan_prefix, "proto", tmp));

	if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static"))
		wan_ifname = nvram_safe_get(strcat_r(wan_prefix, "ifname", tmp));
	else
		wan_ifname = nvram_safe_get(strcat_r(wan_prefix, "pppoe_ifname", tmp));

	//get default_wan
	default_wan = nvram_get_int("default_wan");	

	if(default_wan == unit)
	{
		/* Reset default gateway route via PPPoE interface */
		if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static")) {			
			route_del(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
			route_add(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
		}
		else if (!strcmp(wan_proto, "pppoe") || !strcmp(wan_proto, "pptp") || !strcmp(wan_proto,  "l2tp"))
		{
			char *wan_xgateway = nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp));
			route_del(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
			route_add(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");

			if (strlen(wan_xgateway) > 0 && strcmp(wan_xgateway, "0.0.0.0")) {
				char *wan_xifname =  nvram_safe_get(strcat_r(wan_prefix, "ifname", tmp));
				route_del(wan_xifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");
				route_add(wan_xifname, 3, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");
			}
		}
		/* Add the default gateway of VPN client */
		route_add((char*)vpnc_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(prefix, "gateway", tmp)), "0.0.0.0");
		/* Remove route to the gateway - no longer needed */
		route_del((char*)vpnc_ifname, 0, nvram_safe_get(strcat_r(prefix, "gateway", tmp)), NULL, "255.255.255.255");
	}
	
	/* Add dns servers to resolv.conf */
	vpnc_update_resolvconf(unit);

	/* Add firewall rules for VPN client */
	vpnc_add_firewall_rule(unit, vpnc_ifname);

	update_vpnc_state(unit, WAN_STATE_CONNECTED, 0);	
}

int
vpnc_ipup_main(int argc, char **argv)
{
	FILE *fp;
	char *vpnc_ifname = safe_getenv("IFNAME");
	char *vpnc_linkname = safe_getenv("LINKNAME");
	char tmp[100], prefix[] = "vpnc_", vpnc_prefix[] ="vpncXXXX_";
	char buf[256], *value;
	int unit;

	_dprintf("%s():: %s\n", __FUNCTION__, argv[0]);

	/* Get unit from LINKNAME: vpn[UNIT] */
	if ((unit = vpnc_ppp_linkunit(vpnc_linkname)) < 0)
		return 0;

	snprintf(vpnc_prefix, sizeof(vpnc_prefix), "vpnc%d_", unit);

	_dprintf("%s: unit=%d ifname=%s\n", __FUNCTION__, unit, vpnc_ifname);

	/* Touch connection file */
	if (!(fp = fopen(strcat_r("/tmp/ppp/link.", vpnc_ifname, tmp), "a"))) {
		perror(tmp);
		return errno;
	}
	fclose(fp);

	if ((value = getenv("IPLOCAL"))) {
		if (nvram_invmatch(strcat_r(vpnc_prefix, "ipaddr", tmp), value))
			ifconfig(vpnc_ifname, IFUP, "0.0.0.0", NULL);
		_ifconfig(vpnc_ifname, IFUP, value, "255.255.255.255", getenv("IPREMOTE"), 0);
		nvram_set(strcat_r(vpnc_prefix, "ipaddr", tmp), value);
		//nvram_set(strcat_r(vpnc_prefix, "netmask", tmp), "255.255.255.255");
	}

	if ((value = getenv("IPREMOTE")))
	{
		nvram_set(strcat_r(vpnc_prefix, "gateway", tmp), value);
	}

	strcpy(buf, "");
	if ((value = getenv("DNS1")))
		snprintf(buf, sizeof(buf), "%s", value);
	if ((value = getenv("DNS2")))
		snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "%s%s", strlen(buf) ? " " : "", value);

	/* empty DNS means they either were not requested or peer refused to send them.
	 * lift up underlying xdns value instead, keeping "dns" filled */
	if (strlen(buf) == 0)
		snprintf(buf, sizeof(buf), "%s", nvram_safe_get(strcat_r(prefix, "xdns", tmp)));

	nvram_set(strcat_r(vpnc_prefix, "dns", tmp), buf);

	vpnc_up(unit, vpnc_ifname);

	//add routing table
#ifdef USE_MULTIPATH_ROUTE_TABLE	
	set_routing_table(1, unit);
#endif

	//set up default wan
	change_default_wan_as_vpnc_updown(unit, 1);

	_dprintf("%s:: done\n", __FUNCTION__);
	return 0;
}

void vpnc_del_firewall_rule(const int vpnc_idx, const char *vpnc_ifname)
{
	char tmp[100], prefix[] = "vpncXXXX_", wan_prefix[] = "wanXXXXXXXXXX_";
	char *wan_proto = NULL;
	char lan_if[IFNAMSIZ+1];
	
	if(!vpnc_ifname)
		return;

	snprintf(prefix, sizeof(prefix), "vpnc%d_", vpnc_idx);

	strcpy(lan_if, nvram_safe_get("lan_ifname"));

	snprintf(wan_prefix, sizeof(wan_prefix), "wan%d_", wan_primary_ifunit());
	wan_proto = nvram_safe_get(strcat_r(wan_prefix, "proto", tmp));
	if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static"))
		//eval("iptables", "-D", "FORWARD", "-p", "tcp", "--syn", "-j", "TCPMSS", "--clamp-mss-to-pmtu");
		eval("iptables", "-D", "FORWARD", "-p", "tcp", "--tcp-flags", "SYN,RST", "SYN", "-j", "TCPMSS", "--clamp-mss-to-pmtu");
#ifdef RTCONFIG_BCMARM
	else
#ifdef HND_ROUTER
		if (nvram_match("fc_disable", "0") && nvram_match("fc_pt_war", "1"))
#else
		if (nvram_match("ctf_disable", "0"))
#endif
		eval("iptables", "-t", "mangle", "-D", "FORWARD", "-p", "tcp", 
			"-m", "state", "--state", "NEW","-j", "MARK", "--set-mark", "0x01/0x7");
#endif

	eval("iptables", "-D", "FORWARD", "-o", (char*)vpnc_ifname, "!", "-i", lan_if, "-j", "DROP");
	eval("iptables", "-t", "nat", "-D", "PREROUTING", "-d", 
		nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)), "-j", "VSERVER");
	eval("iptables", "-t", "nat", "-D", "POSTROUTING", "-o", 
		(char*)vpnc_ifname, "!", "-s", nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)), "-j", "MASQUERADE");

	//delete device policies
	vpnc_set_policy_by_ifname(vpnc_ifname, 0);
	
}

void
vpnc_down(char *vpnc_ifname)
{
	char tmp[100], wan_prefix[] = "wanXXXXXXXXXX_", vpnc_prefix[] = "vpncXXXX_";
	char *wan_ifname = NULL, *wan_proto = NULL;
	int unit, default_wan;

	if(!vpnc_ifname)
		return;

	unit = vpnc_ppp_linkunit_by_ifname(vpnc_ifname);

	if(unit == -1)
		return;

	//get default_wan
	default_wan = nvram_get_int("default_wan");
	
	//init prefix
	snprintf(vpnc_prefix, sizeof(vpnc_prefix), "vpnc%d_", unit);
	snprintf(wan_prefix, sizeof(wan_prefix), "wan%d_", wan_primary_ifunit());

	///get wan interface name
	wan_proto = nvram_safe_get(strcat_r(wan_prefix, "proto", tmp));

	if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static"))
		wan_ifname = nvram_safe_get(strcat_r(wan_prefix, "ifname", tmp));
	else
		wan_ifname = nvram_safe_get(strcat_r(wan_prefix, "pppoe_ifname", tmp));

#if !defined(CONFIG_BCMWL5) && defined(RTCONFIG_DUALWAN)
	if (get_nr_wan_unit() > 1 && nvram_match("wans_mode", "lb")) {
		/* Reset default gateway route */
		if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static")) {
			route_del(wan_ifname, 2, "0.0.0.0", nvram_pf_safe_get(wan_prefix, "gateway"), "0.0.0.0");
		}
		else if (!strcmp(wan_proto, "pppoe") || !strcmp(wan_proto, "pptp") || !strcmp(wan_proto, "l2tp"))
		{
			char *wan_xgateway = nvram_pf_safe_get(wan_prefix, "xgateway");

			route_del(wan_ifname, 2, "0.0.0.0", nvram_pf_safe_get(wan_prefix, "gateway"), "0.0.0.0");
			if (strlen(wan_xgateway) > 0 && strcmp(wan_xgateway, "0.0.0.0")) {
				char *wan_xifname = nvram_pf_safe_get(wan_prefix, "ifname");

				route_del(wan_xifname, 3, "0.0.0.0", nvram_pf_safe_get(wan_prefix, "xgateway"), "0.0.0.0");
			}

			/* Delete route to pptp/l2tp's server */
			if (nvram_pf_get_int(vpnc_prefix, "dut_disc") && strcmp(wan_proto, "pppoe"))
				route_del(wan_ifname, 0, nvram_pf_safe_get(wan_prefix, "gateway"), "0.0.0.0", "255.255.255.255");
		}

		/* default route via default gateway */
		add_multi_routes();
	} else {
#endif
		if(default_wan != unit)
		{
			/* Delete route to pptp/l2tp's server */
			if (nvram_get_int(strcat_r(vpnc_prefix, "dut_disc", tmp)) && strcmp(wan_proto, "pppoe"))
				route_del(wan_ifname, 0, nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0", "255.255.255.255");			
		}
		else
		{
			/* Reset default gateway route */
			if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static")) {
				route_del(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
				route_add(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
			}
			else if (!strcmp(wan_proto, "pppoe") || !strcmp(wan_proto, "pptp") || !strcmp(wan_proto, "l2tp"))
			{
				char *wan_xgateway = nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp));

				route_del(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
				route_add(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");

				if (strlen(wan_xgateway) > 0 && strcmp(wan_xgateway, "0.0.0.0")) {
					char *wan_xifname = nvram_safe_get(strcat_r(wan_prefix, "ifname", tmp));

					route_del(wan_xifname, 3, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");
					route_add(wan_xifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");
				}

				/* Delete route to pptp/l2tp's server */
				if (nvram_get_int(strcat_r(vpnc_prefix, "dut_disc", tmp)) && strcmp(wan_proto, "pppoe"))
					route_del(wan_ifname, 0, nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0", "255.255.255.255");
			}
		}
#if !defined(CONFIG_BCMWL5) && defined(RTCONFIG_DUALWAN)
	}
#endif

	/* Delete firewall rules for VPN client */
	vpnc_del_firewall_rule(unit, vpnc_ifname);

	//Andy Chiu, 2016/11/25. Delete iptables rule for device policy list.
	//vpnc_set_policy_by_ifname(vpnc_ifname, 0);

}

/*
 * Called when link goes down
 */
int
vpnc_ipdown_main(int argc, char **argv)
{
	char *vpnc_ifname = safe_getenv("IFNAME");
	char *vpnc_linkname = safe_getenv("LINKNAME");
	char tmp[100];
	int unit;

	_dprintf("%s():: %s\n", __FUNCTION__, argv[0]);

	/* Get unit from LINKNAME: vpn[UNIT] */
	if ((unit = vpnc_ppp_linkunit(vpnc_linkname)) < 0)
		return 0;

	_dprintf("%s: unit=%d ifname=%s\n", __FUNCTION__, unit, vpnc_ifname);

	/* override wan_state to get real reason */
	update_vpnc_state(unit, WAN_STATE_STOPPED, vpnc_pppstatus(unit));

	vpnc_down(vpnc_ifname);

	/* Add dns servers to resolv.conf */
	update_resolvconf();

	unlink(strcat_r("/tmp/ppp/link.", vpnc_ifname, tmp));

#ifdef USE_MULTIPATH_ROUTE_TABLE
	//del routing table
	set_routing_table(0, unit);
#endif	

	//set up default wan
	change_default_wan_as_vpnc_updown(unit, 0);

	_dprintf("%s:: done\n", __FUNCTION__);
	return 0;
}

/*
 * Called when interface comes up
 */
int
vpnc_ippreup_main(int argc, char **argv)
{
	char *vpnc_ifname = safe_getenv("IFNAME");
	char *vpnc_linkname = safe_getenv("LINKNAME");
	char tmp[100], prefix[] = "vpncXXXX_";
	int unit;

	_dprintf("%s():: (%d)%s\n", __FUNCTION__, argc, argv[0]);

	/* Get unit from LINKNAME: vpn[UNIT] */
	if ((unit = vpnc_ppp_linkunit(vpnc_linkname)) < 0)
		return 0;

	_dprintf("%s: unit=%d ifname=%s\n", __FUNCTION__, unit, vpnc_ifname);

	/* Set vpnc_pppoe_ifname to real interface name */
	snprintf(prefix, sizeof(prefix), "vpnc%d_", unit);
	nvram_set(strcat_r(prefix, "ifname", tmp), vpnc_ifname);

	_dprintf("%s:: done\n", __FUNCTION__);
	return 0;
}

/*
 * Called when link closing with auth fail
 */
int
vpnc_authfail_main(int argc, char **argv)
{
	char *vpnc_ifname = safe_getenv("IFNAME");
	char *vpnc_linkname = safe_getenv("LINKNAME");
	int unit;

	_dprintf("%s():: %s\n", __FUNCTION__, argv[0]);

	/* Get unit from LINKNAME: ppp[UNIT] */
	if ((unit = vpnc_ppp_linkunit(vpnc_linkname)) < 0)
		return 0;

	_dprintf("%s: unit=%d ifname=%s\n", __FUNCTION__, unit, vpnc_ifname);

	/* override vpnc_state */
	update_vpnc_state(unit, WAN_STATE_STOPPED, WAN_STOPPED_REASON_PPP_AUTH_FAIL);

	_dprintf("%s:: done\n", __FUNCTION__);
	return 0;
}

/*******************************************************************
* NAME: _find_vpnc_idx_by_ovpn_unit
* AUTHOR: Andy Chiu
* CREATE DATE: 2017/2/23
* DESCRIPTION: get vpnc_idx by ovpn unit
* INPUT:  ovpn_unit: the unit of OpenVPN config
* OUTPUT:  
* RETURN:  -1: not found. Otherwise, matched profile vpnc_idx
* NOTE:
*******************************************************************/
static int _find_vpnc_idx_by_ovpn_unit(const int ovpn_unit)
{
	int i;
	VPNC_PROFILE *prof;
	for(i = 0; i < vpnc_profile_num; ++i)
	{
		prof = vpnc_profile + i;
		if(prof->protocol == VPNC_PROTO_OVPN && prof->config.ovpn.ovpn_idx == ovpn_unit)
			return prof->vpnc_idx;
	}
	return -1;
}

/*******************************************************************
* NAME: vpnc_ovpn_up_main
* AUTHOR: Andy Chiu
* CREATE DATE: 2017/2/22
* DESCRIPTION: callback for openvpn
* INPUT:  
* OUTPUT:  
* RETURN:  
* NOTE:
*******************************************************************/
int vpnc_ovpn_up_main(int argc, char **argv)
{
	int unit, vpnc_idx;
	char *ifname = safe_getenv("dev");
	char *ipaddr = safe_getenv("ifconfig_local");
	char *vpn_gateway = safe_getenv("route_vpn_gateway");
	
	char tmp[100], prefix[] = "vpncXXXX_";

	if(argc < 2)
	{
		_dprintf("[%s, %d]parameters error!\n", __FUNCTION__, __LINE__);
		return 0;
	}
	
	unit = atoi(argv[1]);

	// load vpnc profile list	
	vpnc_init();

	vpnc_idx = _find_vpnc_idx_by_ovpn_unit(unit);
	if(vpnc_idx != -1 )
	{
		snprintf(prefix, sizeof(prefix), "vpnc%d_", vpnc_idx);
		nvram_set(strcat_r(prefix, "ifname", tmp), ifname);
		if(ipaddr)
			nvram_set(strcat_r(prefix, "ipaddr", tmp), ipaddr);
		if(vpn_gateway)
			nvram_set(strcat_r(prefix, "gateway", tmp), vpn_gateway);
		nvram_set(strcat_r(prefix, "dns", tmp), "");	//clean dns
	}
	return 0;
	
}

/*******************************************************************
* NAME: vpnc_ovpn_down_main
* AUTHOR: Andy Chiu
* CREATE DATE: 2017/2/22
* DESCRIPTION: callback for openvpn
* INPUT:  
* OUTPUT:  
* RETURN:  
* NOTE:
*******************************************************************/
int vpnc_ovpn_down_main(int argc, char **argv)
{
	int unit, vpnc_idx;
	char *ifname = NULL;
	if(argc <= 2)
	{
		_dprintf("[%s, %d]parameters error!\n", __FUNCTION__, __LINE__);
		return 0;
	}

	unit = atoi(argv[1]);
	ifname = argv[2];

	// load vpnc profile list	
	vpnc_init();

	vpnc_idx = _find_vpnc_idx_by_ovpn_unit(unit);
	if(vpnc_idx != -1 )
	{
		/* override wan_state to get real reason */
		update_vpnc_state(vpnc_idx, WAN_STATE_STOPPED, 0);

		vpnc_down(ifname);

		/* Add dns servers to resolv.conf */
		update_resolvconf();

#ifdef USE_MULTIPATH_ROUTE_TABLE	
		set_routing_table(0, vpnc_idx);
#endif

		//set up default wan
		change_default_wan_as_vpnc_updown(vpnc_idx, 0);
	}
	return 0;
}

/*******************************************************************
* NAME: vpnc_ovpn_route_up_main
* AUTHOR: Andy Chiu
* CREATE DATE: 2017/2/22
* DESCRIPTION: callback for openvpn
* INPUT:  
* OUTPUT:  
* RETURN:  
* NOTE:
*******************************************************************/
int vpnc_ovpn_route_up_main(int argc, char **argv)
{
	int unit, vpnc_idx;
	char *ifname = NULL;
	if(argc <= 2)
	{
		_dprintf("[%s, %d]parameters error!\n", __FUNCTION__, __LINE__);
		return 0;
	}

	unit = atoi(argv[1]);
	ifname = argv[2];

	_dprintf("[%s, %d]openvpn unit = %d, ifname = %s\n", __FUNCTION__, __LINE__, unit, ifname);

	// load vpnc profile list	
	vpnc_init();

	vpnc_idx = _find_vpnc_idx_by_ovpn_unit(unit);
	if(vpnc_idx != -1 )
	{	
		vpnc_up(vpnc_idx, ifname);

#ifdef USE_MULTIPATH_ROUTE_TABLE	
		set_routing_table(1, vpnc_idx);
#endif
		//set up default wan
		change_default_wan_as_vpnc_updown(vpnc_idx, 1);
	}
	return 0;
}


/*******************************************************************
* NAME: vpnc_dump_dev_policy_list
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/11/23
* DESCRIPTION: dump VPNC_DEV_POLICY list
* INPUT:  list: an array to store policy, list_size: size of list 
* OUTPUT:  
* RETURN:  
* NOTE:
*******************************************************************/
static void
vpnc_dump_dev_policy_list(const VPNC_DEV_POLICY *list, const int list_size)
{
	int i;
	const VPNC_DEV_POLICY *policy;
	
	if(!list || list_size <= 0)
		return;

	_dprintf("[%s, %d]Start to dump!\n", __FUNCTION__, __LINE__);
	for(i = 0, policy = list; i < list_size; ++i, ++policy)
	{
		_dprintf("[%s, %d]<%d><active:%d><mac:%s><dst_ip:%s><vpnc_idx:%d>\n", __FUNCTION__, __LINE__,
			i, policy->active, policy->mac, policy->dst_ip, policy->vpnc_idx);
	}
	_dprintf("[%s, %d]End of dump!\n", __FUNCTION__, __LINE__);
}

static void
vpnc_dump_vpnc_profile(const VPNC_PROFILE *profile)
{
	if(!profile)
		return;
	_dprintf("[%s, %d]<active:%d><vpnc_idx:%d><proto:%d><server:%s><username:%s><password:%s>"
		, __FUNCTION__, __LINE__, profile->active, profile->vpnc_idx, profile->protocol, profile->basic.server, profile->basic.username, profile->basic.password);
	switch(profile->protocol)
	{
		case VPNC_PROTO_PPTP:
			_dprintf("<option:%d>\n", profile->config.pptp.option);
			break;
		case VPNC_PROTO_OVPN:
			_dprintf("<ovpn_idx:%d>\n", profile->config.ovpn.ovpn_idx);
			break;
		case VPNC_PROTO_IPSEC:
		default:
			_dprintf("\n");				
			break;
	}
}

/*******************************************************************
* NAME: vpnc_dump_vpnc_profile_list
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/12/06
* DESCRIPTION: dump VPNC_PROFILE list
* INPUT:  list: an array to store vpnc profile, list_size: size of list 
* OUTPUT:  
* RETURN:  
* NOTE:
*******************************************************************/
static void
vpnc_dump_vpnc_profile_list(const VPNC_PROFILE *list, const int list_size)
{
	int i;
	const VPNC_PROFILE *profile;
	
	if(!list || list_size <= 0)
		return;

	_dprintf("[%s, %d]Start to dump!\n", __FUNCTION__, __LINE__);

	for(i = 0, profile = list; i < list_size; ++i, profile = list + i)
		vpnc_dump_vpnc_profile(profile);

	_dprintf("[%s, %d]End of dump!\n", __FUNCTION__, __LINE__);
}

/*******************************************************************
* NAME: vpnc_set_basic_conf
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/12/07
* DESCRIPTION: set basic config dat structure
* INPUT:  server: string. server ip. username: string. password: string.
* OUTPUT:  basic_conf: a pointer of VPNC_BASIC_CONF
* RETURN:  0: success, -1: fialed
* NOTE:
*******************************************************************/
static int
vpnc_set_basic_conf(const char *server, const char *username, const char *passwd, VPNC_BASIC_CONF *basic_conf)
{
	if(!basic_conf)
		return -1;

	memset(basic_conf, 0, sizeof(VPNC_BASIC_CONF));

	if(server)
		snprintf(basic_conf->server, sizeof(basic_conf->server), "%s", server);
	if(username)
		snprintf(basic_conf->username, sizeof(basic_conf->username), "%s", username);
	if(passwd)
		snprintf(basic_conf->password, sizeof(basic_conf->password), "%s", passwd);

	return 0;
}

/*******************************************************************
* NAME: vpnc_load_profile
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/12/07
* DESCRIPTION: Parser the nvram setting and load the VPNC_PROFILE list
* INPUT:  list: an array to store vpnc profile, list_size: size of list 
* OUTPUT:  
* RETURN:  number of profiles, -1: fialed
* NOTE:
*******************************************************************/
int vpnc_load_profile(VPNC_PROFILE *list, const int list_size)
{
	char *nv = NULL, *nvp = NULL, *b = NULL;
	int cnt = 0, i = 0;
	char * desc, *proto, *server, *username, *passwd, *active, *vpnc_idx, *reserved1, *reserved2;

	if(!list || list_size <= 0)
		return -1;

	// load "vpnc_clientlist" to set username, password and server ip
	nv = nvp = strdup(nvram_safe_get("vpnc_clientlist"));

	cnt = 0;
	memset(list, 0, sizeof(VPNC_PROFILE)*list_size);
	while (nv && (b = strsep(&nvp, "<")) != NULL && cnt <= list_size) {
		if (vstrsep(b, ">", &desc, &proto, &server, &username, &passwd) < 3)
			continue;
			
		if(proto && server && username && passwd)
		{
			vpnc_set_basic_conf(server, username, passwd, &(list[cnt].basic));
			
			if(!strcmp(proto, PROTO_PPTP))
			{
				list[cnt].protocol = VPNC_PROTO_PPTP;
			}
			else if(!strcmp(proto, PROTO_L2TP))
			{
				list[cnt].protocol = VPNC_PROTO_L2TP;
			}
			else if(!strcmp(proto, PROTO_OVPN))
			{
				list[cnt].protocol = VPNC_PROTO_OVPN;
				list[cnt].config.ovpn.ovpn_idx = atoi(server);
			}
			++cnt;
		}
	}
	SAFE_FREE(nv);

	// load "vpnc_clientlist_ex" to set active and vpnc_idx
	nv = nvp = strdup(nvram_safe_get("vpnc_clientlist_ex"));
	i = 0;
	while (nv && (b = strsep(&nvp, "<")) != NULL && i <= cnt) {
		if (vstrsep(b, ">", &active, &vpnc_idx, &reserved1, &reserved2) < 2)
			continue;

		if(!active || !vpnc_idx)
			continue;
		
		list[i].active = atoi(active);
		list[i].vpnc_idx = atoi(vpnc_idx);		
		++i;	
	}
	SAFE_FREE(nv);
	if(i != cnt)
		_dprintf("[%s, %d]the numbers of vpnc_clientlist(%d) and vpnc_clientlist_ex(%d) are different!\n", __FUNCTION__, __LINE__, cnt, i);

	//load "vpnc_pptp_options_x_list" to set pptp option
	nv = nvp = strdup(nvram_safe_get("vpnc_pptp_options_x_list"));
	i = 0;
	while (nv && (b = strsep(&nvp, "<")) != NULL && i <= cnt) {

		if(i > 0 && VPNC_PROTO_PPTP == list[i - 1].protocol)
		{
			if(!strcmp(b, "auto"))
				list[i - 1].config.pptp.option = VPNC_PPTP_OPT_AUTO;
			else if(!strcmp(b, "-mppc"))
				list[i - 1].config.pptp.option = VPNC_PPTP_OPT_MPPC;
			else if(!strcmp(b, "+mppe-40"))
				list[i - 1].config.pptp.option = VPNC_PPTP_OPT_MPPE40;
			else if(!strcmp(b, "+mppe-56"))
				list[i - 1].config.pptp.option = VPNC_PPTP_OPT_MPPE56;
			else if(!strcmp(b, "+mppe-128"))
				list[i - 1].config.pptp.option = VPNC_PPTP_OPT_MPPE128;
			else
				list[i - 1].config.pptp.option = VPNC_PPTP_OPT_UNDEF;
		}
		++i;	
	}
	SAFE_FREE(nv);
	if(i != cnt + 1)
		_dprintf("[%s, %d]the numbers of vpnc_clientlist(%d) and vpnc_pptp_options_x_list(%d) are different!\n", __FUNCTION__, __LINE__, cnt, i);

	//vpnc_dump_vpnc_profile_list(list, cnt);
	return cnt;
}

/*******************************************************************
* NAME: vpnc_get_dev_policy_list
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/11/23
* DESCRIPTION: parser vpnc_dev_policy_list
* INPUT:  list_size: size of list 
* OUTPUT:  list: an array to store policy
* RETURN:  number of the list. 
* NOTE:
*******************************************************************/
static int
vpnc_get_dev_policy_list(VPNC_DEV_POLICY *list, const int list_size)
{
	int cnt;
	VPNC_DEV_POLICY *policy = list;

	char *nv, *nvp, *b;
	char *active, *mac, *dst_ip, *vpnc_idx;
	
	if(!list || list_size <= 0)
		return 0;

	memset(list, 0, sizeof(VPNC_DEV_POLICY) * list_size);
	
	/* Protection level per client */
	nv = nvp = strdup(nvram_safe_get("vpnc_dev_policy_list"));

	cnt = 0;
	while (nv && (b = strsep(&nvp, "<")) != NULL && cnt <= list_size) {
		
		if (vstrsep(b, ">", &active, &mac, &dst_ip, &vpnc_idx) < 3)
			continue;
			
		if(!active || !mac || !vpnc_idx)
			continue;

		policy->active = atoi(active);
		snprintf(policy->mac, sizeof(policy->mac), "%s", mac);
		if(dst_ip)
			snprintf(policy->dst_ip, sizeof(policy->dst_ip), "%s", dst_ip);
		policy->vpnc_idx = atoi(vpnc_idx);

		++cnt;
		++policy;
	}
	free(nv);

	//vpnc_dump_dev_policy_list(list, list_size);
	return cnt;
}

/*******************************************************************
* NAME: vpnc_find_index_by_ifname
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/11/25
* DESCRIPTION: Find vpnc index by interface name
* INPUT:  vpnc_ifname: interface name
* OUTPUT:  
* RETURN:  index of vpnc. -1 is not found.
* NOTE:
*******************************************************************/
static int
_vpnc_find_index_by_ifname(const char *vpnc_ifname)
{
	int unit;
	if(!vpnc_ifname)
		return -1;

	if(!strncmp(vpnc_ifname, "ppp", 3))	//pptp/l2tp
	{
		return vpnc_ppp_linkunit_by_ifname(vpnc_ifname);		
	}
	else if(!strncmp(vpnc_ifname, "tun", 3))	//openvpn
	{
		unit = atoi(vpnc_ifname + 3);
		return _find_vpnc_idx_by_ovpn_unit(unit);
	}
	return -1;
}


/*******************************************************************
* NAME: vpnc_set_policy_by_ifname
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/11/25
* DESCRIPTION: set device policy by vpnc interface name
* INPUT:  vpnc_ifname: interface name
*		action: 1: add rules for interface up. 0: remove rule for interface down.
* OUTPUT:  
* RETURN:  -1: failed, 0: success
* NOTE:
*******************************************************************/
int
vpnc_set_policy_by_ifname(const char *vpnc_ifname, const int action)
{
	int policy_cnt, vpnc_idx, i;
	VPNC_DEV_POLICY *policy;
	char *lan_ifname = nvram_safe_get("lan_ifname");
	VPNC_DEV_POLICY 	dev_policy[MAX_DEV_POLICY] = {0};

	policy_cnt =  vpnc_get_dev_policy_list(dev_policy, MAX_DEV_POLICY);	

	if(!vpnc_ifname || !policy_cnt || !lan_ifname || lan_ifname[0] == '\0')
		return -1;

	vpnc_idx = _vpnc_find_index_by_ifname(vpnc_ifname);

	for(i = 0, policy = dev_policy; i < policy_cnt; ++i, ++policy)
	{
		if(policy->active && vpnc_idx == policy->vpnc_idx)
		{
			if(!action)	//remove rule
			{
#ifdef USE_IPTABLE_ROUTE_TARGE				
				if(policy->dst_ip[0] != '\0')	//has destination ip
					eval("iptables", "-D", "PREROUTING", "-t", "mangle", "-i", lan_ifname, "-m", "mac", "--mac-source",
						policy->mac, "-d", policy->dst_ip,"-j", "ROUTE", "--oif", (char*)vpnc_ifname);
				else
					eval("iptables", "-D", "PREROUTING", "-t", "mangle", "-i", lan_ifname, "-m", "mac", "--mac-source",
						policy->mac, "-j", "ROUTE", "--oif", (char*)vpnc_ifname);
#endif	
#ifdef USE_MULTIPATH_ROUTE_TABLE
				//Can not support destination ip
				set_routing_rule_by_mac(VPNC_ROUTE_DEL, policy->mac, policy->vpnc_idx);
#endif
			}
			else		//add value
			{
#ifdef USE_IPTABLE_ROUTE_TARGE				
				if(policy->dst_ip[0] != '\0')	//has destination ip
					eval("iptables", "-A", "PREROUTING", "-t", "mangle", "-i", lan_ifname, "-m", "mac", "--mac-source",
						policy->mac, "-d", policy->dst_ip,"-j", "ROUTE", "--oif", (char*)vpnc_ifname);
				else
					eval("iptables", "-A", "PREROUTING", "-t", "mangle", "-i", lan_ifname, "-m", "mac", "--mac-source",
						policy->mac, "-j", "ROUTE", "--oif", (char*)vpnc_ifname);
#endif
#ifdef USE_MULTIPATH_ROUTE_TABLE
				//Can not support destination ip
				set_routing_rule_by_mac(VPNC_ROUTE_ADD, policy->mac, policy->vpnc_idx);
#endif
			}
		}
	}
	return 0;
}

/*******************************************************************
* NAME: vpnc_handle_policy_rule
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/1/25
* DESCRIPTION: add/del iptable rule
* INPUT:  action: 0:delete rule, 1: add rule.  lan_ifname: lan interface name. cliemt mac: client mac address.
* 		vpnc_ifname: vpn client interface name. target_ip(optional): target ip address. target_port(optional): target port. set -1 as unused.
* OUTPUT:  
* RETURN:  0:success, -1:failed.
* NOTE:
*******************************************************************/
#ifdef USE_IPTABLE_ROUTE_TARGE
int vpnc_handle_policy_rule(const int action, const char *lan_ifname, const char *client_mac, 
	const char *vpnc_ifname, const char *target_ip, const int target_port)
{
	char port[8];
	int vpnc_idx;

	if(!lan_ifname || !client_mac || !vpnc_ifname)
	{
		_dprintf("[%s, %d] parameters error!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	vpnc_idx = _vpnc_find_index_by_ifname(vpnc_ifname);
	
	snprintf(port, sizeof(port), "%d", target_port);
	
	if(!action)	//delete
	{
		if(target_ip && target_ip[0] != '\0' && target_port >= 0)	//has destination ip and port
			eval("iptables", "-D", "PREROUTING", "-t", "mangle", "-i", (char*)lan_ifname, "-m", "mac", "--mac-source",
				(char*)client_mac, "-d", (char*)target_ip, "--dport", port, "-j", "ROUTE", "--oif", (char*)vpnc_ifname);
		else if(target_ip && target_ip[0] != '\0')	//has target ip
			eval("iptables", "-D", "PREROUTING", "-t", "mangle", "-i", (char*)lan_ifname, "-m", "mac", "--mac-source",
				(char*)client_mac, "-d", (char*)target_ip, "-j", "ROUTE", "--oif", (char*)vpnc_ifname);
		else if(target_port >= 0)	//has target port			
			eval("iptables", "-D", "PREROUTING", "-t", "mangle", "-i", (char*)lan_ifname, "-m", "mac", "--mac-source",
				(char*)client_mac, "--dport", port, "-j", "ROUTE", "--oif", (char*)vpnc_ifname);
		else	//others
			eval("iptables", "-D", "PREROUTING", "-t", "mangle", "-i", (char*)lan_ifname, "-m", "mac", "--mac-source",
				(char*)client_mac, "-j", "ROUTE", "--oif", (char*)vpnc_ifname);
	}
	else	//add
	{
		if(target_ip && target_ip[0] != '\0' && target_port >= 0)	//has destination ip and port
			eval("iptables", "-A", "PREROUTING", "-t", "mangle", "-i", (char*)lan_ifname, "-m", "mac", "--mac-source",
				(char*)client_mac, "-d", (char*)target_ip, "--dport", port, "-j", "ROUTE", "--oif", (char*)vpnc_ifname);
		else if(target_ip && target_ip[0] != '\0')	//has target ip
			eval("iptables", "-A", "PREROUTING", "-t", "mangle", "-i", (char*)lan_ifname, "-m", "mac", "--mac-source",
				(char*)client_mac, "-d", (char*)target_ip, "-j", "ROUTE", "--oif", (char*)vpnc_ifname);
		else if(target_port >= 0)	//has target port			
			eval("iptables", "-A", "PREROUTING", "-t", "mangle", "-i", (char*)lan_ifname, "-m", "mac", "--mac-source",
				(char*)client_mac, "--dport", port, "-j", "ROUTE", "--oif", (char*)vpnc_ifname);
		else	//others
			eval("iptables", "-A", "PREROUTING", "-t", "mangle", "-i", (char*)lan_ifname, "-m", "mac", "--mac-source",
				(char*)client_mac, "-j", "ROUTE", "--oif", (char*)vpnc_ifname);
	}
	return 0;
}
#elif defined(USE_MULTIPATH_ROUTE_TABLE)
int vpnc_handle_policy_rule(const int action, const char *client_mac, const int vpnc_idx)
{
	if(!client_mac)
	{
		_dprintf("[%s, %d] parameters error!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	if(!action)	//delete
	{
		_dprintf("[%s, %d]remove rule. client_mac=%s, vpnc_idx=%d\n", __FUNCTION__, __LINE__, 
			client_mac, vpnc_idx);

		if(vpnc_idx != -1)
		{
			set_routing_rule_by_mac(VPNC_ROUTE_DEL, client_mac, vpnc_idx);
		}
	}
	else	//add
	{
		_dprintf("[%s, %d]add rule. client_mac=%s, vpnc_idx=%d\n", __FUNCTION__, __LINE__, 
			client_mac, vpnc_idx);
		
		if(vpnc_idx != -1)
		{
			set_routing_rule_by_mac(VPNC_ROUTE_ADD, client_mac, vpnc_idx);
		}
	}
	return 0;
}
#endif
/*******************************************************************
* NAME: vpnc_active_dev_policy
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/1/24
* DESCRIPTION: when web ui active/inactive one dev policy, this function will handle it
* INPUT:  policy_idx: index of vpnc_dev_policy_list. Start from 0.
* OUTPUT:  
* RETURN:  0:success, -1:failed.
* NOTE:
*******************************************************************/
int vpnc_active_dev_policy(const int policy_idx)
{
	int policy_cnt;
	VPNC_DEV_POLICY *policy;
	VPNC_DEV_POLICY 	dev_policy[MAX_DEV_POLICY] = {0};
#ifdef USE_IPTABLE_ROUTE_TARGE
	char *lan_ifname = nvram_safe_get("lan_ifname");
	char *vpnc_ifname = NULL, tmp[100];
#endif

	policy_cnt =  vpnc_get_dev_policy_list(dev_policy, MAX_DEV_POLICY);	

	_dprintf("[%s, %d]idx<%d> policy cnt<%d>\n", __FUNCTION__, __LINE__, policy_idx, policy_cnt);
	
	if(!policy_cnt || policy_idx >= policy_cnt || policy_idx < 0)
	{
		_dprintf("[%s, %d] policy_idx(%d) is not exist in the policy list(%d)\n", __FUNCTION__, __LINE__, policy_idx, policy_cnt);
		return -1;
	}

	policy = &(dev_policy[policy_idx]);

#ifdef USE_IPTABLE_ROUTE_TARGE
	if!lan_ifname || lan_ifname[0] == '\0'()
		return -1;
	snprintf(tmp, sizeof(tmp), "vpnc%d_ifname", policy->vpnc_idx);
	vpnc_ifname = nvram_safe_get(tmp);

	if(!vpnc_ifname || vpnc_ifname[0] =='\0')
	{
		_dprintf("[%s, %d] Can not find interface name by vpnc_idx(%d)\n", __FUNCTION__, __LINE__, policy_idx);
		return -1;
	}

	if(policy->active)
		vpnc_handle_policy_rule(1, lan_ifname, policy->mac, vpnc_ifname, policy->dst_ip, -1);
	else
		vpnc_handle_policy_rule(0, lan_ifname, policy->mac, vpnc_ifname, policy->dst_ip, -1);
#elif defined(USE_MULTIPATH_ROUTE_TABLE)
	if(policy->active)
		vpnc_handle_policy_rule(1, policy->mac, policy->vpnc_idx);
	else
		vpnc_handle_policy_rule(0, policy->mac, policy->vpnc_idx);
#endif
	return 0;
	
}

/*******************************************************************
* NAME: vpnc_active_dev_policy
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/1/24
* DESCRIPTION: when web ui active/inactive one dev policy, this function will handle it
* INPUT:  policy_idx: index of vpnc_dev_policy_list. Start from 0.
* OUTPUT:  
* RETURN:  0:success, -1:failed.
* NOTE:
*******************************************************************/
int vpnc_remove_tmp_policy_rule()
{
	char *nv;
	char *active, *mac, *dst_ip, *vpnc_idx;
	VPNC_DEV_POLICY policy;
#ifdef USE_IPTABLE_ROUTE_TARGE
	char *lan_ifname = nvram_safe_get("lan_ifname");
	char *vpnc_ifname = NULL, tmp[100];
#endif

	/* Protection level per client */
	nv = strdup(nvram_safe_get("vpnc_tmp_dev_policy"));
	memset(&policy, 0, sizeof(VPNC_DEV_POLICY));

	_dprintf("[%s, %d]remove vpnc_tmp_dev_policy=%s\n", __FUNCTION__, __LINE__, nv);
	
	if (vstrsep(nv, ">", &active, &mac, &dst_ip, &vpnc_idx) >= 3)
	{
		if(!active || !mac || !vpnc_idx)
		{
			free(nv);
			_dprintf("[%s, %d] Invalid value in vpnc_tmp_dev_policy\n", __FUNCTION__, __LINE__);
			return -1;
		}

		policy.active = atoi(active);
		snprintf(policy.mac, sizeof(policy.mac), "%s", mac);
		policy.vpnc_idx = atoi(vpnc_idx);

#ifdef USE_IPTABLE_ROUTE_TARGE
		if(dst_ip)
			snprintf(policy.dst_ip, sizeof(policy.dst_ip), "%s", dst_ip);
		snprintf(tmp, sizeof(tmp), "vpnc%d_ifname", policy.vpnc_idx);
		vpnc_ifname = nvram_safe_get(tmp);

		if(!vpnc_ifname || vpnc_ifname[0] =='\0')
		{
			_dprintf("[%s, %d] Can not find interface name by vpnc_idx(%d)\n", __FUNCTION__, __LINE__, policy.vpnc_idx);
			free(nv);
			return -1;			
		}

		if(policy.active)
			vpnc_handle_policy_rule(0, lan_ifname, policy.mac, vpnc_ifname, policy.dst_ip, -1);
#elif defined(USE_MULTIPATH_ROUTE_TABLE)
		vpnc_handle_policy_rule(0, policy.mac, policy.vpnc_idx);
#endif
		
	}

	free(nv);
	return 0;
}

/*******************************************************************
* NAME: vpnc_init
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/12/07
* DESCRIPTION: initialize variables
* INPUT:  
* OUTPUT:  
* RETURN:  
* NOTE:
*******************************************************************/
void
vpnc_init()
{
	//load profile
	vpnc_profile_num = vpnc_load_profile(vpnc_profile, MAX_VPNC_PROFILE);
	_dprintf("[%s, %d]vpnc_profile_num=%d\n", __FUNCTION__, __LINE__, vpnc_profile_num);
}

/*******************************************************************
* NAME: start_vpnc_by_unit
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/12/07
* DESCRIPTION: start to connect the vpnc by profile
* INPUT:  unit: index of vpnc client list. (NOT vpnc_idx)
* OUTPUT:  
* RETURN:  0: success, -1: failed
* NOTE:
*******************************************************************/
int
start_vpnc_by_unit(const int unit)
{
	FILE *fp;
	char options[80], l2tp_conf[128], l2tp_ctrl[128], l2tp_pid[128];
	char *pppd_argv[] = { "/usr/sbin/pppd", "file", options, NULL};
	char tmp[100], prefix[] = "vpnc_", vpnc_prefix[] = "vpncXXXXXXXXXX_", wan_prefix[] = "wanXXXXXXXXXX_";
	char buf[256];	/* although maximum length of pppoe_username/pppoe_passwd is 64. pppd accepts up to 256 characters. */
	mode_t mask;
	int ret = 0;
	VPNC_PROFILE *prof;
	
	if(unit >= vpnc_profile_num)
		return -1;

	_dprintf("[%s, %d]Start unit(%d)!\n", __FUNCTION__, __LINE__, unit);
	
	prof = vpnc_profile + unit;

	//vpnc_dump_vpnc_profile(prof);
	
	//stop if connection exist.
	stop_vpnc_by_unit(unit);

	//init prefix
	snprintf(wan_prefix, sizeof(wan_prefix), "wan%d_", wan_primary_ifunit());
	snprintf(vpnc_prefix, sizeof(vpnc_prefix), "vpnc%d_", prof->vpnc_idx);

	/* unset vpnc_dut_disc */
	nvram_unset(strcat_r(vpnc_prefix, "dut_disc", tmp));
	
	//init option path
	if(VPNC_PROTO_PPTP == prof->protocol)
		snprintf(options, sizeof(options), "/tmp/ppp/vpnc%d_options.pptp", prof->vpnc_idx);
	else if(VPNC_PROTO_L2TP == prof->protocol)
		snprintf(options, sizeof(options), "/tmp/ppp/vpnc%d_options.l2tp", prof->vpnc_idx);
	else if(VPNC_PROTO_OVPN == prof->protocol)
	{
		_dprintf("[%s, %d]Start to connect OpenVPN(%d).\n", __FUNCTION__, __LINE__, prof->config.ovpn.ovpn_idx);
		start_ovpn_client(prof->config.ovpn.ovpn_idx);
		return 0;
	}
	else if(VPNC_PROTO_IPSEC== prof->protocol)
	{
		//TODO:
		_dprintf("[%s, %d]Start to connect IPSec.\n", __FUNCTION__, __LINE__);
		return 0;
	}
	else
	{
		_dprintf("[%s, %d]Unknown protocol\n", __FUNCTION__, __LINE__);
		return -1;
	}

	//init vpncX_ state.
	update_vpnc_state(prof->vpnc_idx, WAN_STATE_INITIALIZING, 0);

	//Run PPTP/L2TP
	if (VPNC_PROTO_PPTP == prof->protocol || VPNC_PROTO_L2TP == prof->protocol) {
		mask = umask(0000);

		/* Generate options file */
		if (!(fp = fopen(options, "w"))) {
			perror(options);
			umask(mask);
			return -1;
		}

		umask(mask);

		/* route for pptp/l2tp's server */
		char *wan_ifname = nvram_safe_get(strcat_r(wan_prefix, "pppoe_ifname", tmp));
		route_add(wan_ifname, 0, nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0", "255.255.255.255");

		//generate ppp profile
		/* do not authenticate peer and do not use eap */
		fprintf(fp, "noauth\n");
		fprintf(fp, "refuse-eap\n");
		fprintf(fp, "user '%s'\n",
			ppp_safe_escape(prof->basic.username, buf, sizeof(buf)));
		fprintf(fp, "password '%s'\n",
			ppp_safe_escape(prof->basic.password, buf, sizeof(buf)));

		if (VPNC_PROTO_PPTP == prof->protocol) {
			fprintf(fp, "plugin pptp.so\n");
			fprintf(fp, "pptp_server '%s'\n", prof->basic.server);
			fprintf(fp, "vpnc 1\n");
			/* see KB Q189595 -- historyless & mtu */
			if (nvram_match(strcat_r(wan_prefix, "proto", tmp), "pptp") || nvram_match(strcat_r(wan_prefix, "proto", tmp), "l2tp"))
				fprintf(fp, "nomppe-stateful mtu 1300\n");
			else
				fprintf(fp, "nomppe-stateful mtu 1400\n");

			if (VPNC_PPTP_OPT_MPPC == prof->config.pptp.option) {
				fprintf(fp, "nomppe nomppc\n");
			} else
			if (VPNC_PPTP_OPT_MPPE40== prof->config.pptp.option) {
				fprintf(fp, "require-mppe\n"
					    "require-mppe-40\n");
			} else
			if (VPNC_PPTP_OPT_MPPE56== prof->config.pptp.option) {
				fprintf(fp, "nomppe-40\n"
					    "nomppe-128\n"
					    "require-mppe\n"
					    "require-mppe-56\n");
			} else
			if (VPNC_PPTP_OPT_MPPE128== prof->config.pptp.option) {
				fprintf(fp, "nomppe-40\n"
					    "nomppe-56\n"
					    "require-mppe\n"
					    "require-mppe-128\n");
			}
		} else {
			fprintf(fp, "nomppe nomppc\n");

			if (nvram_match(strcat_r(wan_prefix, "proto", tmp), "pptp") || nvram_match(strcat_r(wan_prefix, "proto", tmp), "l2tp"))
				fprintf(fp, "mtu 1300\n");
			else
				fprintf(fp, "mtu 1400\n");			
		}

		if (VPNC_PROTO_L2TP != prof->protocol) {
			ret = nvram_get_int(strcat_r(prefix, "pppoe_idletime", tmp));
			if (ret && nvram_get_int(strcat_r(prefix, "pppoe_demand", tmp))) {
				fprintf(fp, "idle %d ", ret);
				if (nvram_invmatch(strcat_r(prefix, "pppoe_txonly_x", tmp), "0"))
					fprintf(fp, "tx_only ");
				fprintf(fp, "demand\n");
			}
			fprintf(fp, "persist\n");
		}

		fprintf(fp, "holdoff %d\n", nvram_get_int(strcat_r(prefix, "pppoe_holdoff", tmp)) ? : 10);
		fprintf(fp, "maxfail %d\n", nvram_get_int(strcat_r(prefix, "pppoe_maxfail", tmp)));

		if (nvram_invmatch(strcat_r(prefix, "dnsenable_x", tmp), "0"))
			fprintf(fp, "usepeerdns\n");

		fprintf(fp, "ipcp-accept-remote ipcp-accept-local noipdefault\n");
		fprintf(fp, "ktune\n");

		/* pppoe set these options automatically */
		/* looks like pptp also likes them */
		fprintf(fp, "default-asyncmap nopcomp noaccomp\n");

		/* pppoe disables "vj bsdcomp deflate" automagically */
		/* ccp should still be enabled - mppe/mppc requires this */
		fprintf(fp, "novj nobsdcomp nodeflate\n");

		/* echo failures */
		fprintf(fp, "lcp-echo-interval 6\n");
		fprintf(fp, "lcp-echo-failure 10\n");

		/* pptp has Echo Request/Reply, l2tp has Hello packets */
		if (VPNC_PROTO_PPTP == prof->protocol || VPNC_PROTO_L2TP == prof->protocol)
			fprintf(fp, "lcp-echo-adaptive\n");

		fprintf(fp, "unit %d\n", prof->vpnc_idx);
		fprintf(fp, "linkname vpn%d\n", prof->vpnc_idx);
		fprintf(fp, "ip-up-script %s\n", "/tmp/ppp/vpnc-ip-up");
		fprintf(fp, "ip-down-script %s\n", "/tmp/ppp/vpnc-ip-down");
		fprintf(fp, "ip-pre-up-script %s\n", "/tmp/ppp/vpnc-ip-pre-up");
		fprintf(fp, "auth-fail-script %s\n", "/tmp/ppp/vpnc-auth-fail");

		/* user specific options */
		fprintf(fp, "%s\n",
			nvram_safe_get(strcat_r(prefix, "pppoe_options_x", tmp)));

		fclose(fp);

		if (VPNC_PROTO_L2TP == prof->protocol)
		{
			snprintf(l2tp_conf, sizeof(l2tp_conf), L2TP_VPNC_CONF, prof->vpnc_idx);
			snprintf(l2tp_ctrl, sizeof(l2tp_ctrl), L2TP_VPNC_CTRL, prof->vpnc_idx);
			snprintf(l2tp_pid, sizeof(l2tp_pid), L2TP_VPNC_PID, prof->vpnc_idx);

			//generate l2tp profile
			if (!(fp = fopen(l2tp_conf, "w"))) {
				perror(l2tp_conf);
				return -1;
			}

			fprintf(fp, "# automagically generated\n"
				"global\n\n"
				"load-handler \"sync-pppd.so\"\n"
				"load-handler \"cmd.so\"\n\n"
				"section sync-pppd\n\n"
				"lac-pppd-opts \"file %s\"\n\n"
				"section peer\n"
				"port 1701\n"
				"peername %s\n"
				"vpnc 1\n"
				"hostname %s\n"
				"lac-handler sync-pppd\n"
				"persist yes\n"
				"maxfail %d\n"
				"holdoff %d\n"
				"hide-avps no\n"
				"section cmd\n"
				"socket-path " "%s" "\n\n",
				options,
	                        prof->basic.server,
				nvram_invmatch(strcat_r(prefix, "hostname", tmp), "") ?
					nvram_safe_get(strcat_r(prefix, "hostname", tmp)) : "localhost",
				nvram_get_int(strcat_r(prefix, "pppoe_maxfail", tmp))  ? : 32767,
				nvram_get_int(strcat_r(prefix, "pppoe_holdoff", tmp)) ? : 10, l2tp_ctrl);

			fclose(fp);

			/* launch l2tp */
			eval("/usr/sbin/l2tpd", "-c", l2tp_conf, "-p", l2tp_pid);

			ret = 3;
			do {
				_dprintf("%s: wait l2tpd up at %d seconds...\n", __FUNCTION__, ret);
				usleep(1000*1000);
			} while (!pids("l2tpd") && ret--);

			/* start-session */
			ret = eval("/usr/sbin/l2tp-control", "-s", l2tp_ctrl, "start-session 0.0.0.0");

			/* pppd sync nodetach noaccomp nobsdcomp nodeflate */
			/* nopcomp novj novjccomp file /tmp/ppp/options.l2tp */

		} 
		else
		{
			ret = _eval(pppd_argv, NULL, 0, NULL);	//launch pppd
		}
	}
	update_vpnc_state(prof->vpnc_idx, WAN_STATE_CONNECTING, 0);

	return ret;
	
}

/*******************************************************************
* NAME: stop_vpnc_by_unit
* AUTHOR: Andy Chiu
* CREATE DATE: 2016/12/07
* DESCRIPTION: stop connecting the vpnc by profile
* INPUT:  unit: index of vpnc client list. (NOT vpnc_idx)
* OUTPUT:  
* RETURN:  0: success, -1: failed
* NOTE:
*******************************************************************/
int
stop_vpnc_by_unit(const int unit)
{
	VPNC_PROFILE *prof;
	char pidfile[sizeof("/var/run/ppp-vpnXXXXXXXXXX.pid")];
	char l2tp_pid[128];
	char tmp[100], vpnc_prefix[] = "vpncXXXXX_";
	
	if(unit >= vpnc_profile_num)
		return -1;

	prof = vpnc_profile + unit;

	snprintf(pidfile, sizeof(pidfile), "/var/run/ppp-vpn%d.pid", prof->vpnc_idx);

	snprintf(vpnc_prefix, sizeof(vpnc_prefix), "vpnc%d_", prof->vpnc_idx);

	/* Reset the state of vpnc_dut_disc */
	nvram_set_int(strcat_r(vpnc_prefix, "dut_disc", tmp), prof->vpnc_idx);

	if(VPNC_PROTO_PPTP == prof->protocol || VPNC_PROTO_L2TP == prof->protocol)
	{
		update_vpnc_state(prof->vpnc_idx, WAN_STATE_STOPPING, 0);
		if(VPNC_PROTO_L2TP == prof->protocol)
		{
			//stop l2tp
			snprintf(l2tp_pid, sizeof(l2tp_pid), L2TP_VPNC_PID, prof->vpnc_idx);
			if(check_if_file_exist(l2tp_pid))
			{
				kill_pidfile_tk(l2tp_pid);
				usleep(1000*10000);
			}
		}
		/* Stop pppd */
		if (kill_pidfile_s(pidfile, SIGHUP) == 0 &&
		    kill_pidfile_s(pidfile, SIGTERM) == 0) {
			usleep(3000*1000);
			kill_pidfile_tk(pidfile);
		}	
	}
	else if(VPNC_PROTO_OVPN == prof->protocol)
	{
		_dprintf("[%s, %d]Stop OpenVPN(%d).\n", __FUNCTION__, __LINE__, prof->config.ovpn.ovpn_idx);
		stop_ovpn_client(prof->config.ovpn.ovpn_idx);		
	}
	return 0;
}

#ifdef USE_MULTIPATH_ROUTE_TABLE
/*******************************************************************
* NAME: set_routing_table
* AUTHOR: Andy Chiu
* CREATE DATE: 2017/2/7
* DESCRIPTION: set multipath routing table 
* INPUT:  cmd: 1:add, 0:delete.  vpnc_id: index of vpnc profile
* OUTPUT:  
* RETURN:  0: success, -1: failed
* NOTE:
*******************************************************************/
int set_routing_table(const int cmd, const int vpnc_id)
{
	char tmp[100], prefix[] = "vpncXXXXX_", id_str[16], cmd_str[8];
	char *wan_gateway, *wan_ifname, *lan_ifname, *lan_ipaddr, *lan_netmask, lan_fulladdr[32];

	lan_ifname = nvram_safe_get("lan_ifname");
	lan_ipaddr = nvram_safe_get("lan_ipaddr");
	lan_netmask =  nvram_safe_get("lan_netmask");

	//_dprintf("[%s, %d]lan ip=%s, ifname=%s, mask=%s\n", __FUNCTION__, __LINE__, lan_ipaddr, lan_ifname, lan_netmask);

	if(get_network_addr_by_ip_prefix(lan_ipaddr, lan_netmask, lan_fulladdr, sizeof(lan_fulladdr)))
		return -1;

	snprintf(cmd_str, sizeof(cmd_str), "%s", (!cmd)? "del": "add");
	
	if(vpnc_id >= VPNC_UNIT_BASIC)	//VPNC
	{
		snprintf(id_str, sizeof(id_str), "%d", vpnc_id);
		snprintf(prefix, sizeof(prefix), "vpnc%d_", vpnc_id);
	}
	else	//internet
	{
		snprintf(id_str, sizeof(id_str), "%d", INTERNET_ROUTE_TABLE_ID);
		snprintf(prefix, sizeof(prefix), "wan%d_", wan_primary_ifunit());
	}
	
	wan_gateway = nvram_safe_get(strcat_r(prefix, "gateway", tmp));
	wan_ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));		
	
	eval("ip", "route", cmd_str, "default", "via", wan_gateway, "dev", wan_ifname, "table", id_str);
	eval("ip", "route", cmd_str, "127.0.0.0", "dev", "lo", "table", id_str);
	eval("ip", "route", cmd_str, lan_fulladdr, "dev", lan_ifname, "table", id_str);
	
	return 0;
}

/*******************************************************************
* NAME: set_routing_table
* AUTHOR: Andy Chiu
* CREATE DATE: 2017/2/7
* DESCRIPTION: set multipath routing table 
* INPUT:  cmd: VPNC_ROUTE_CMD.  vpnc_id: index of vpnc profile
* OUTPUT:  
* RETURN:  0: success, -1: failed
* NOTE:
*******************************************************************/
int set_routing_rule(const VPNC_ROUTE_CMD cmd, const char *source_ip, const int vpnc_id)
{
	char cmd_str[8], id_str[8];

	if(!source_ip)
		return -1;

	//_dprintf("[%s, %d]<%d><%s><%d>\n", __FUNCTION__, __LINE__, cmd, source_ip, vpnc_id);
	snprintf(cmd_str, sizeof(cmd_str), "%s", (cmd == VPNC_ROUTE_DEL)? "del": "add");

	snprintf(id_str, sizeof(id_str), "%d", (vpnc_id >= VPNC_UNIT_BASIC)? vpnc_id: INTERNET_ROUTE_TABLE_ID);

	eval("ip", "rule", cmd_str, "from", source_ip, "table", id_str);
	return 0;
}


/*******************************************************************
* NAME: set_routing_table
* AUTHOR: Andy Chiu
* CREATE DATE: 2017/2/7
* DESCRIPTION: set multipath routing table 
* INPUT:  cmd: 1:add, 0:delete.  vpnc_id: index of vpnc profile
* OUTPUT:  
* RETURN:  0: success, -1: failed
* NOTE:
*******************************************************************/
int set_routing_rule_by_mac(const VPNC_ROUTE_CMD cmd, const char *mac, const int vpnc_id)
{
	char *nv, *nvp, *cli_mac, *cli_ip, *b;
	char ip[20];
	int flag = 0;

	if(!mac)
		return -1;

	//finding out the ip address in dhcp static list.
	nv = nvp = strdup(nvram_safe_get("dhcp_staticlist"));
	while (nv && (b = strsep(&nvp, "<")) != NULL) {

		if (vstrsep(b, ">", &cli_mac, &cli_ip) < 2)
			continue;

		if(!cli_mac || !cli_ip)
			continue;

		if(!strcmp(cli_mac, mac))
		{
			flag = 1;
			snprintf(ip, sizeof(ip), "%s", cli_ip);
			break;
		}
	}
	SAFE_FREE(nv);	

	if(flag)
	{
		set_routing_rule(cmd, ip, vpnc_id);
	}
	
	return 0;
}

#endif

