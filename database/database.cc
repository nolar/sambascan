#include "database.h"

c_database::c_database () {
}

c_database::~c_database () {
}

string c_database::ultostr(unsigned long value)
{
	string::value_type * buffer = (string::value_type*) calloc(1024, sizeof(string::value_type));
	sprintf(buffer, "%U", value);
	string result = buffer;
	free(buffer);
	return result;
}




e_database::e_database (string message)
{
	f_message = message;
}

e_database::e_database (string message, string dberror)
{
	f_message = message + "Error: " + dberror;
}

e_database::~e_database () throw ()
{
}

string e_database::message () throw ()
{
	return f_message;
}
