/* таблица настроек: какие блоки адресов или конкретные адреса сканировать */
create table t_filesearch_block (
	f_filesearch_block	integer unsigned not null auto_increment,
	f_address		integer unsigned not null default 0,
	f_netmask		integer unsigned not null default 32,
	f_period		integer unsigned,
	f_share			varchar(250),
	f_username		varchar(250),
	f_password		varchar(250),
	f_isnetwork		bit default 0,
	f_active		bit not null default 0,
	index idx_address (f_address),
	index idx_netmask (f_netmask),
	primary key (f_filesearch_block));

/* таблица состояния просканированности - временные данные для сканера */
create table t_filesearch_status (
	f_startup		datetime not null,
	f_process		integer unsigned not null,
	f_address		integer unsigned not null,
	f_share			varchar(250) null,
	f_username		varchar(250) null);

/* таблица обнаруженных хостов - управляется сканером автоматически */
create table t_filesearch_host (
	f_filesearch_host	integer unsigned	not null	auto_increment,
	f_address		integer unsigned	not null	,
	f_stamp_found		datetime		null		default null,
	f_stamp_seen		datetime		null		default null,
	unique (f_address),
	primary key (f_filesearch_host));

create table t_filesearch_item (
	f_filesearch_item	integer unsigned	not null	auto_increment,
	f_filesearch_host	integer unsigned	not null	references t_filesearch_host,
	f_stamp_found		datetime		null		default null,
	f_stamp_seen		datetime		null		default null,
	f_proto			enum ('', 'smb', 'ftp')	not null	default '',
	f_port			integer unsigned	null		default null,
	f_username		varchar(250)		null		default null,
	f_password		varchar(250)		null		default null,
	f_hostname		varchar(250)		null		default null,
	f_netbiosshare		varchar(250)		null		default null,
	f_netbiosname		varchar(250)		null		default null,
	f_netbiosgroup		varchar(250)		null		default null,
	unique (f_filesearch_host, f_proto, f_netbiosshare, f_username),
	primary key (f_filesearch_item));



/* таблица обнаруженных ресурсов (share) - управляется сканером автоматически */
/*!!! create table t_filesearch_resource (
	f_filesearch_resource	integer unsigned not null auto_increment,
	f_filesearch_host	integer unsigned not null references t_filesearch_host on delete cascade on update cascade,
	f_stamp_found		datetime null default null,
	f_stamp_seen		datetime null default null,
	f_share			varchar(250) not null,
	f_username		varchar(250) default null,
	f_password		varchar(250) default null,
	unique (f_filesearch_host, f_share),
	primary key (f_filesearch_resource));*/

/* таблица обнаруженных файлов - управляется сканером автоматически */
create table t_filesearch_file (
	f_filesearch_file	integer unsigned	not null	auto_increment,
	f_filesearch_item	integer unsigned	not null	references t_filesearch_item,
	f_stamp_found		datetime		null		default null,
	f_stamp_seen		datetime		null		default null,
	f_container		bit			not null	,
/*	f_path			text			not null	,
	f_name			text			not null	,*/
	f_bytes			bigint unsigned		null/*not!!!*/	,
	f_mtime			datetime		null/*not!!!*/	,
	f_ctime			datetime		null/*not!!!*/	,
/*	unique (f_filesearch_item, f_path(800), f_name(150)),*/
	primary key (f_filesearch_file));

create table t_filesearch_name (
	f_filesearch_file	integer unsigned	not null	references f_filesearch_file,
	f_level			integer unsigned	not null	,
	f_name			text			not null	,
	primary key (f_filesearch_file, f_level));

/* начальные данные */
insert into t_filesearch_block (f_address, f_netmask, f_share)
	values (inet_aton('80.253.235.59'), 32, "public");
insert into t_filesearch_block (f_address, f_netmask, f_share)
	values (inet_aton('10.10.7.0'    ), 24, NULL);



