#ifndef _FILESEARCH_UTILS_ENTRY_
#define _FILESEARCH_UTILS_ENTRY_

#include "functions.h"
#include <string>
#include <vector>
#include <sys/types.h>

using namespace std;

typedef vector<string> c_path;

class c_stat {
private:
	off_t  f_bytes;
	time_t f_ctime;
	time_t f_mtime;
	bool   f_container;
protected:
public:
	c_stat (off_t a_bytes, time_t a_ctime, time_t a_mtime, bool a_container);

	off_t  bytes ();
	time_t ctime ();
	time_t mtime ();
	bool container ();
};

//typedef vector<c_entry> c_entries;


class c_item
{
private:
	t_proto f_proto;
	t_ipport f_port;
	t_ipaddr f_addr;
	string f_username;
	string f_password;
	string f_hostname;
	string f_netbiosshare;
	string f_netbiosname;
	string f_netbiosgroup;
protected:
public:
	c_item () {}
	c_item (t_proto a_proto, t_ipport a_port, t_ipaddr a_addr, string a_username, string a_password, string a_hostname, string a_netbiosshare, string a_netbiosname, string a_netbiosgroup);

	t_proto proto ();
	t_ipport port ();
	t_ipaddr address ();
	string username ();
	string password ();
	string hostname ();
	string netbiosshare ();
	string netbiosname ();
	string netbiosgroup ();
};

#endif
