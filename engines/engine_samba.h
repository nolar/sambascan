#ifndef _FILESEARCH_ENGINE_SAMBA_
#define _FILESEARCH_ENGINE_SAMBA_

#include "engine.h"
#include "../database/database.h"
#include <string>
#include <vector>

using namespace std;

class c_samba_cache_id
{
private:
	string f_share;
	string f_workgroup;
	string f_username;
protected:
public:
	string f_server;
//	c_samba_cache_id () {}
	c_samba_cache_id (const c_samba_cache_id &right);
	c_samba_cache_id (string a_server, string a_share, string a_workgroup, string a_username);
	bool operator == (c_samba_cache_id &right);
	bool operator != (c_samba_cache_id &right);
	bool operator <  (const c_samba_cache_id &right) const;
};

//struct c_samba_cache_compare
//{
//public:
//	operator () (const c_samba_cache_id &x, const c_samba_cache_id &y);
//};

class e_samba : public e_database
{
public:
	e_samba (string message) : e_database (message) {}
	e_samba (string message, string m2) : e_database (message, m2) {}
};

class c_engine_samba : public c_engine
{
private:
protected:
public:
	//!!! do we need this constructor & destructor?
	c_engine_samba ();
	virtual ~c_engine_samba ();
	//
	static void init ();
	static void free ();
	//
	static void guarantie_smbctx ();
	static void guarantie_iconv  ();
	//
	static void recursive (string prefix, c_path root, unsigned int level, unsigned int maxlevel, bool isroot);
	static t_scan_result scan (unsigned long address, string share, string username, string password, c_database * database);
};

#endif
