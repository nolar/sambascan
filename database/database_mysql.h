#ifndef _FILESEARCH_DATABASE_MYSQL_
#define _FILESEARCH_DATABASE_MYSQL_

#include "database.h"
#include "mysql.h"

class c_database_mysql : public c_database {
private:
	MYSQL handle;
	bool _select_host (t_id &id, t_ipaddr address);
	bool _insert_host (t_id &id, t_ipaddr address);
	void _update_host (t_id id);
	bool _select_item (t_id &id, t_id host, t_proto proto, t_ipport port, string username, string password, string hostname, string netbiosshare, string netbiosname, string netbiosgroup);
	bool _insert_item (t_id &id, t_id host, t_proto proto, t_ipport port, string username, string password, string hostname, string netbiosshare, string netbiosname, string netbiosgroup);
	void _update_item (t_id id);
	bool _select_file (t_id &id, t_id item, string path, string name);
	bool _insert_file (t_id &id, t_id item, string path, string name);
	void _update_file (t_id id, size_t size, time_t time);
protected:
	string escape (string s);
public:
	c_database_mysql (string host, string user, string pass, string db, unsigned int port, string socket, unsigned long flags);
	virtual ~c_database_mysql ();
	// Functions to work with address blocks.
	virtual c_blocks fetch_blocks ();
	// Functions to work with temporary per-process host/share status.
	virtual bool status_check (unsigned int address, string share, string username);
	virtual void status_renew (unsigned int address, string share, string username);
	virtual void status_clean ();
	//
	virtual t_id found_item (c_item item);
	virtual t_id found_file (t_id item, c_path path, c_stat stat);
};

#endif
