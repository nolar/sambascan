#include "functions.h"
#include <stdlib.h>
#include <iostream>

unsigned long utils::inet_aton (string address)
{
	//!!! add more checks for valid address here
	//!!! and throw e_address if can't convert
	string::size_type pos, pre = 0;
	string s[4];
	unsigned long a[4], result = 0;
	bool enough = false;
	for (int i = 0; i < 4 && !enough; i++, pre = pos + 1)
	{
		pos = address.find(".", pre);
		enough = pos == string::npos;
		s[i] = enough ? address.substr(pre) : address.substr(pre, pos-pre);
	}
	for (int i = 0; i < 4; i++)
	{
		a[i] = strtoul(s[i]);
		result = (result << 8) | (a[i] & 0xff);
	}
	return result;
}

string utils::inet_ntoa (unsigned long address)
{
	string result;
	result += ultostr((address >> 24) & 0xff) + ".";
	result += ultostr((address >> 16) & 0xff) + ".";
	result += ultostr((address >>  8) & 0xff) + ".";
	result += ultostr((address      ) & 0xff);
	return result;
}

string utils::iconv (iconv_t handle, string s)
{
//	printf("in0\n");
	string result;
	string::size_type len = s.length();
//	printf("in1: %d\n", len);
	size_t slength = len  +1;
	size_t dlength = len*2+1;
//	printf("in2: %d, %d\n", slength, dlength);
	char * sbuffer = new string::value_type [slength], *sponiter = sbuffer;
	char * dbuffer = new string::value_type [dlength], *dpointer = dbuffer;
//	printf("in3: %p, %p\n", sbuffer, dbuffer);
	const char * cpointer = const_cast<const char*>(sbuffer);
//	printf("in4:\n");
	strncpy(sbuffer, s.c_str(), slength);
	sbuffer[slength] = 0;
//	printf("in5: %s\n", sbuffer);

	size_t res = ::iconv(handle, &cpointer, &slength, &dpointer, &dlength);
//	printf("in6: %d (%d,%d)\n", res, slength, dlength);
	if (res != (size_t)-1)
	{
//		printf("in7: %s\n", dbuffer);
		result = dbuffer;
	}
//	printf("in8: %s\n", result.c_str());
	delete[] sbuffer;
	delete[] dbuffer;
//	printf("in9: %x, %x\n", sbuffer, dbuffer);
	return result;
}



e_address::e_address (string address)
{
	f_binary = false;
	f_string_data = address;
	f_binary_data = 0;
}

e_address::e_address (unsigned long address)
{
	f_binary = true;
	f_string_data = "";
	f_binary_data = address;
}

e_address::~e_address () throw ()
{
}

string e_address::message ()
{
	return f_binary ?
		(string() + "Can not convert address " + utils::ultostr(f_binary_data) + " to its textual form.") :
		(string() + "Can not convert address '" + f_string_data + "' to its binary form.");
}




string utils::ultostr (unsigned long value)
{
	string::value_type * buffer = (string::value_type*) calloc(1024, sizeof(string::value_type));
	sprintf(buffer, "%U", value);
	string result = buffer;
	free(buffer);
	return result;
}

unsigned long utils::strtoul (string value)
{
	return ::strtoul(value.c_str(), NULL, 10);
}
