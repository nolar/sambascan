#ifndef _FILESEARCH_UTILS_BLOCK_
#define _FILESEARCH_UTILS_BLOCK_

#include <string>
#include <vector>

using namespace std;

class c_block {
private:
	unsigned long f_address;
	unsigned long f_netmask;
	string f_share;
	string f_username;
	string f_password;
	bool f_isnetwork;
protected:
	static const unsigned long address_max = (unsigned long) -1;
	static const unsigned long address_min = (unsigned long) 0;
	static const unsigned long netmask_min = (unsigned long) 0;
	static const unsigned long netmask_max = (unsigned long) 32;
public:
	c_block (unsigned long a_address, unsigned long a_netmask, string a_share, string a_username, string a_password, bool a_isnetwork);
	c_block (string a_address, string a_netmask, string a_share, string a_username, string a_password, string a_isnetwork);

	unsigned long address ();
	unsigned long netmask ();
	unsigned long address_from ();
	unsigned long address_till ();
	string share ();
	string username ();
	string password ();
	bool isnetwork();
};

typedef vector<c_block> c_blocks;

#endif
