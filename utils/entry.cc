#include "entry.h"
#include <stdlib.h>

c_stat::c_stat (off_t a_bytes, time_t a_ctime, time_t a_mtime, bool a_container)
{
	f_bytes = a_bytes;
	f_ctime = a_ctime;
	f_mtime = a_mtime;
	f_container = a_container;
}

off_t c_stat::bytes ()
{
	return f_bytes;
}

time_t c_stat::ctime ()
{
	return f_ctime;
}

time_t c_stat::mtime ()
{
	return f_mtime;
}

bool c_stat::container ()
{
	return f_container;
}













c_item::c_item (t_proto a_proto, t_ipport a_port, t_ipaddr a_addr, string a_username, string a_password, string a_hostname, string a_netbiosshare, string a_netbiosname, string a_netbiosgroup)
{
	f_proto = a_proto;
	f_port  = a_port ;
	f_addr  = a_addr;
	f_username = a_username;
	f_password = a_password;
	f_hostname = a_hostname;
	f_netbiosshare = a_netbiosshare;
	f_netbiosname  = a_netbiosname ;
	f_netbiosgroup = a_netbiosgroup;
}

t_proto  c_item::proto () { return f_proto; }
t_ipport c_item::port  () { return f_port ; }
t_ipaddr c_item::address () { return f_addr; }
string c_item::username () { return f_username; }
string c_item::password () { return f_password; }
string c_item::hostname () { return f_hostname; }
string c_item::netbiosshare () { return f_netbiosshare; }
string c_item::netbiosname () { return f_netbiosname; }
string c_item::netbiosgroup () { return f_netbiosgroup; }
