#include "block.h"
#include <stdlib.h>

c_block::c_block (unsigned long a_address, unsigned long a_netmask, string a_share, string a_username, string a_password, bool a_isnetwork)
{
	f_address = a_address;
	f_netmask = a_netmask;
	f_share = a_share;
	f_username = a_username;
	f_password = a_password;
	f_isnetwork = a_isnetwork;
}

c_block::c_block (string a_address, string a_netmask, string a_share, string a_username, string a_password, string a_isnetwork)
{
	f_address  = strtoul(a_address.c_str(), NULL, 10);
	f_netmask = strtoul(a_netmask.c_str(), NULL, 10);
	f_share = a_share;
	f_username = a_username;
	f_password = a_password;
	f_isnetwork = a_isnetwork=="1";
}


unsigned long c_block::address ()
{
	return f_address;
}

unsigned long c_block::address_from ()
{
	unsigned long result = f_netmask ?
		(f_address >> (netmask_max-f_netmask)) << (netmask_max-f_netmask)
		: address_min;
	return f_isnetwork ? result + 1 : result;
}

unsigned long c_block::address_till ()
{
	unsigned long result = f_netmask ? (address_from() |~
		((address_max >> (netmask_max-f_netmask)) << (netmask_max-f_netmask)))
		: address_max;
	return f_isnetwork ? result - 1 : result;
}

unsigned long c_block::netmask ()
{
	return f_netmask;
}

string c_block::share ()
{
	return f_share;
}

string c_block::username ()
{
	return f_username;
}

string c_block::password ()
{
	return f_password;
}

bool c_block::isnetwork ()
{
	return f_isnetwork;
}
