#ifndef _FILESEARCH_UTILS_FUNCTIONS_
#define _FILESEARCH_UTILS_FUNCTIONS_

#include <string>
#include <vector>
#include <iconv.h>

using namespace std;

typedef unsigned int t_id;
typedef unsigned long t_ipaddr;
typedef unsigned int  t_ipport;
typedef enum{proto_unknown, proto_smb, proto_ftp} t_proto;

class utils
{
private:
protected:
public:
	static unsigned long inet_aton (string address);
	static string inet_ntoa (unsigned long address);
	static string ultostr (unsigned long value);
	static unsigned long strtoul (string value);
	static string iconv (iconv_t handle, string s);
};

class e_address : public exception
{
private:
	bool f_binary;
	string f_string_data;
	unsigned long f_binary_data;
protected:
public:
	e_address (unsigned long address);
	e_address (string address);
	virtual ~e_address () throw ();
	virtual string message ();
};

#endif
