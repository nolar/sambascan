/*
 * Сканер ресурсов обслуживаемых компьютеров.
 *
 * Формат входных данных (stdin):
 *
 * Формат выходных данных (stdout):
 *
 * (a) Sergei Vasilyev aka nolar 2005
 */

#include "utils/block.h"
#include "utils/functions.h"
#include "database/database_mysql.h"
#include "engines/engine_samba.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>

c_database * database = NULL;
	
//#include <libsmbclient.h>

/*
void auth_fn(const char *server, const char *share,
             char *workgroup, int wgmaxlen, char *username, int unmaxlen,
	                  char *password, int pwmaxlen)
{
	strncpy(workgroup, "KRASLAN", wgmaxlen-1);
	strncpy(username , "guest", unmaxlen-1);
	strncpy(password , "", pwmaxlen-1);
}			  



*/


//
//
//
void log (char * format, ...) {
}

unsigned long long COUNTER;//!!!

inline void scan_address (unsigned int address, string share, string username, string password)
{
	COUNTER++;//!!!
	// проверяем, не была ли такая шара с такого адреса уже найдена
	bool already = database->status_check(address, share, username);
	// если таковой еще нет, то сканируем эту шару на этом компьютере
	if (!already)
	{
		t_scan_result result;
		// сканируем компьютер или конкретную шару
//!!!		switch (protocol)
//!!!		{
//!!!			case netbios:
				result = c_engine_samba::scan(address, share, username, password, database);
//!!!				break;
//!!!			default:
//!!!				result = scan_protocol_unknown;
//!!!		}

		// в случае успешного сканирования добавляем хост/шару в список
		switch (result)
		{
			// в случае успеха - вносим в список просканированных хостов/шар
			case scan_ok:
				database->status_renew(address, share, username);
				break;
			// в случае, если возвращенный код нам неизвестен, сообщаем об ошибке
			default:
				cerr << "error!!! unknown result code from scanner" << endl;
		}
	}
}

int main(int argc, char ** argv, char ** env) {
	int exitcode = 0;
	try
	{
	// получение параметров вызова программы и занесение их в переменные
//!!!
	// initializing engines libraries
	c_engine_samba::init();
	// connecting to database
	database = new c_database_mysql("", "filesearch", "filesearch", "filesearch", 0, "", 0);
	database->status_clean();
	// retrieving list of address blocks
	c_blocks blocks = database->fetch_blocks();
	c_blocks::iterator block;
	for (block = blocks.begin(); block != blocks.end(); block++)
	{
		COUNTER=0;
		//!!! debug output. delete it later.
//		cerr << "CHECKING: ";
		cerr << "Address=" << dec << setw(10) << block->address() << "(" << hex << setw(8) << block->address() << ")";
		cerr << " ";
		cerr << "Netmask=" << dec << setw( 2) << block->netmask();
		cerr << " => ";
		cerr << dec << setw(10) << block->address_from() << "(" << hex << setw(8) << block->address_from() << ")";
		cerr << "-";
		cerr << dec << setw(10) << block->address_till() << "(" << hex << setw(8) << block->address_till() << ")";
//		cerr << " // Share=" << block->share();
//		cerr << " // Accrd=" << block->username() << ":" << block->password();
		cerr << endl;

		// пробежка по всем адресам проверяемого блока
		unsigned long address_from = block->address_from();
		unsigned long address_till = block->address_till();
		for (unsigned long address = address_from; address < address_till; address++)
		{scan_address(address     , block->share(), block->username(), block->password());}
		if (address_from <= address_till)
		{scan_address(address_till, block->share(), block->username(), block->password());}
	}
	// freeing engines resources
	c_engine_samba::free();
	}
	catch (e_address &e)
	{
		cerr << "Address convertion error: " <<  e.message() << endl;
		exitcode = -4;
	}
	catch (e_database &e)
	{
		cerr << "Database error: " <<  e.message() << endl;
		exitcode = -3;
	}
	catch (exception &e)
	{
		cerr << "Unknown exception: " << e.what() << endl;
		exitcode = -2;
	}
	catch (...)
	{
		cerr << "Unknown exception without message" << endl;
		exitcode = -1;
	}
	if (database)
	{
		try { database->status_clean(); }
		catch (e_database) {}
		delete database; database = NULL;
	}
	return exitcode;
}
