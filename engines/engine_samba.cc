#include "engine_samba.h"
#include "../utils/functions.h"
#include "../utils/entry.h"
#include <iostream>
#include <iomanip>
#include <libsmbclient.h>
#include <iconv.h>
#include <sys/stat.h>
#include <map>
#include <unistd.h>


c_database * temp_database = NULL;
c_item temp_itemdata;
t_id temp_itemid;

bool c_engine_samba_smbctx_ready = false;
SMBCCTX * c_engine_samba_smbctx;
bool c_engine_samba_iconv_ready = false;
iconv_t c_engine_samba_iconv;


map<c_samba_cache_id,SMBCSRV*> samba_cache;

//c_samba_cache_compare::operator () (const c_samba_cache_id &x, const c_samba_cache_id &y)
//	        {
//		                return x.f_server < y.f_server;
//				        }
//					}



c_samba_cache_id::c_samba_cache_id (string a_server, string a_share, string a_workgroup, string a_username)
{
	f_server    = a_server   ;
	f_share     = a_share    ;
	f_workgroup = a_workgroup;
	f_username  = a_username ;
}

c_samba_cache_id::c_samba_cache_id (const c_samba_cache_id &right)
{
	f_server    = right.f_server   ;
	f_share     = right.f_share    ;
	f_workgroup = right.f_workgroup;
	f_username  = right.f_username ;
}

bool c_samba_cache_id::operator == (c_samba_cache_id &right)
{
	return	(f_server    == right.f_server   ) &&
//		(f_share     == right.f_share    ) &&
		(f_workgroup == right.f_workgroup);
//		(f_username  == right.f_username );
}

bool c_samba_cache_id::operator != (c_samba_cache_id &right)
{
	return	(f_server    != right.f_server   ) ||
		(f_share     != right.f_share    ) ||
		(f_workgroup != right.f_workgroup) ||
		(f_username  != right.f_username );
}

bool c_samba_cache_id::operator < (const c_samba_cache_id &right) const
{
	return	(f_server    < right.f_server   ) ||
		(f_share     < right.f_share    ) ||
		(f_workgroup < right.f_workgroup) ||
		(f_username  < right.f_username );
}



c_engine_samba::c_engine_samba ()
{
}

c_engine_samba::~c_engine_samba ()
{
}



void _samba_auth_fn(const char * server, const char * share,
	char * workgroup, int wgmaxlen,
	char * username , int unmaxlen,
	char * password , int pwmaxlen)
{
	strncpy(workgroup, "KRASLAN", wgmaxlen-1);
	strncpy(username , "guest"  , unmaxlen-1);
	strncpy(password , ""       , pwmaxlen-1);
}


int samba_add_cached_srv (SMBCCTX * c, SMBCSRV *srv, const char * server, const char * share, const char * workgroup, const char * username)
{
	c_samba_cache_id id(server, share, workgroup, username);
	samba_cache[id] = srv;
	return 0;
}

SMBCSRV * samba_get_cached_srv (SMBCCTX * c, const char * server, const char * share, const char * workgroup, const char * username)
{
	c_samba_cache_id id(server, share, workgroup, username);
	map<c_samba_cache_id,SMBCSRV*>::iterator i = samba_cache.find(id);
	if (i != samba_cache.end())
		return i->second;
	else	return NULL;
}

int samba_remove_cached_srv (SMBCCTX * c, SMBCSRV *srv)
{
	map<c_samba_cache_id,SMBCSRV*>::iterator i;
	for (i = samba_cache.begin(); i != samba_cache.end(); i++)
	{
		if (i->second == srv)
		{
			samba_cache.erase(i->first);
		}
	}
	return 0;
}

int samba_purge_cached (SMBCCTX * c)
{
//	map<c_samba_cache_id,SMBCSRV*>::iterator i;
	while (samba_cache.begin() != samba_cache.end()) samba_cache.erase(samba_cache.begin());
//	for (i = samba_cache.begin(); i != samba_cache.end(); i++)
//	{
//		samba_cache.erase(i);
//	}
	return 0;
}



void c_engine_samba::init ()
{
	// smbclient context
	c_engine_samba_smbctx = smbc_new_context();
	if (!c_engine_samba_smbctx)
	{
		throw e_samba("Can not allocate context for smbclient.");
	}
	c_engine_samba_smbctx->debug = 0;
	c_engine_samba_smbctx->timeout = 3000; //!!! вынести в опции
	c_engine_samba_smbctx->netbios_name = strdup("SCANNER"); //!!! вынести в опции
	c_engine_samba_smbctx->workgroup = strdup("KRASLAN"); //!!! вынести в опции
	c_engine_samba_smbctx->callbacks.auth_fn = _samba_auth_fn;
	c_engine_samba_smbctx->callbacks.add_cached_srv_fn = samba_add_cached_srv;
	c_engine_samba_smbctx->callbacks.get_cached_srv_fn = samba_get_cached_srv;
	c_engine_samba_smbctx->callbacks.remove_cached_srv_fn = samba_remove_cached_srv;
	c_engine_samba_smbctx->callbacks.purge_cached_fn = samba_purge_cached;
	if (!smbc_init_context(c_engine_samba_smbctx))
	{
		smbc_free_context(c_engine_samba_smbctx, 1);
		throw e_samba("Can not initialize smbclient.", strerror(errno));
	}
	c_engine_samba_smbctx_ready = true;
	// iconv
	c_engine_samba_iconv = iconv_open("KOI8-R//IGNORE", "UTF-8");
	if (c_engine_samba_iconv == (iconv_t)-1)
	{
		smbc_free_context(c_engine_samba_smbctx, 1);
		c_engine_samba_smbctx_ready = false;
		throw e_samba("Can not create iconv handle.", strerror(errno));
	}
	c_engine_samba_iconv_ready = true;
}

void c_engine_samba::free ()
{
	if (c_engine_samba_smbctx_ready) smbc_free_context(c_engine_samba_smbctx, false);
	c_engine_samba_smbctx_ready = false;
	if (c_engine_samba_iconv_ready ) iconv_close(c_engine_samba_iconv);
	c_engine_samba_iconv_ready = false;
}


void c_engine_samba::recursive (string prefix, c_path root, unsigned int level, unsigned int maxlevel, bool isroot)
{
	if (level > maxlevel) return;

//	if (!c_engine_samba_smbctx_ready) init();

	// preparing database
	c_database * database = temp_database;
	if (!database) throw e_samba("Trying to recurse with null database");

	// creating paths
	string rootstr = prefix; for (c_path::iterator i = root.begin(); i != root.end(); i++) rootstr += "/" + *i;
	cerr << ":::" << rootstr << endl;

	// reading dir content and closing it
	map<string,unsigned int> dirents;
	SMBCCTX * ctx = c_engine_samba_smbctx;
	SMBCFILE * dir = ctx->opendir(ctx, rootstr.c_str());
	if (!dir)
	{
		cerr << "    " << "Could not open [" << utils::iconv(c_engine_samba_iconv, rootstr) << "] (" << setw(0) << dec << errno << "):" << strerror(errno) << endl;
	} else {
		smbc_dirent * dirent;
		while ((dirent = ctx->readdir(ctx, dir)) != NULL)
		{
			cerr << "###" << dirent->name << endl;
			dirents[dirent->name] = dirent->smbc_type;
		}
		ctx->closedir(ctx, dir);
	}

	// making item in database
	if (isroot)
	{
		temp_itemid = database->found_item(temp_itemdata);
	}

	// iterating through directory entries
	for (map<string,unsigned int>::iterator dirent = dirents.begin(); dirent != dirents.end(); dirent++)
	{
		string uname = dirent->first;
		c_path upath = root; upath.push_back(uname);
		string ustr = prefix; for (c_path::iterator i = upath.begin(); i != upath.end(); i++) ustr += "/" + *i;
		string pname = utils::iconv(c_engine_samba_iconv, uname);
//		string ppath = utils::iconv(c_engine_samba_iconv, upath);
		if ((uname != "..") && (uname != "."))
		{
			bool container = (dirent->second == SMBC_DIR       ) ||
					 (dirent->second == SMBC_FILE_SHARE) ||
					 (dirent->second == SMBC_SERVER    ) ||
					 (dirent->second == SMBC_WORKGROUP );
			struct stat st;
			if (ctx->stat(ctx, ustr.c_str(), &st) != -1)
			{
				c_stat stat(st.st_size, st.st_ctime, st.st_mtime, container);
				database->found_file(temp_itemid, upath, stat);
			}
			if (container)
			{
				recursive(prefix, upath, level+1, maxlevel, false);
			}
			if ((dirent->second == SMBC_FILE_SHARE) || (dirent->second == SMBC_SERVER    ) || (dirent->second == SMBC_WORKGROUP ))
			{
				sleep(5);
//				free();
			}
		}
	}
}

t_scan_result c_engine_samba::scan (unsigned long address, string share, string username, string password, c_database * database)
{
	cerr << "scanning "
		<< hex << setw(8) << utils::inet_ntoa(address)
		<< string(share.empty()?"":"/")+share
		<< string(username.empty()?"":"/")+username
		<< "..." << endl;


	// сканинг пути
	temp_database = NULL;
	temp_database = database;
	temp_itemdata = c_item(proto_smb, 0, address, username, password, "", share, "", "");

	string fullpath = string("smb://") + utils::inet_ntoa(address) + (share.empty()?"":"/") + share;
	c_path emptypath;
	recursive(fullpath, emptypath, 1, 2, true);
	temp_database = NULL;

	return scan_ok;
}
