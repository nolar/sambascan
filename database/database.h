#ifndef _FILESEARCH_DATABASE_
#define _FILESEARCH_DATABASE_

#include "../utils/block.h"
#include "../utils/entry.h"
#include "../utils/functions.h"
#include <exception>

using namespace std;

class e_database : public exception
{
private:
	string f_message;
protected:
public:
	e_database (string message);
	e_database (string message, string dberror);
	virtual ~e_database () throw();
	string message () throw ();
//	virtual const char* what() const throw();
};

class c_database {
private:
protected:
	string ultostr(unsigned long value);
public:
	c_database ();
	virtual ~c_database ();
	// Functions to work with address blocks.
	virtual c_blocks fetch_blocks () = 0;
	// Functions to work with temporary per-process host/share status.
	virtual bool status_check (unsigned int address, string share, string username) = 0;
	virtual void status_renew (unsigned int address, string share, string username) = 0;
	virtual void status_clean () = 0;
	//
	virtual t_id found_item (c_item item) = 0;
	virtual t_id found_file (t_id item, c_path path, c_stat stat) = 0;
//	virtual void found_bundle   (unsigned long address, string share, string username, string password, string path, c_entries entries) = 0;
	//
};

#endif
