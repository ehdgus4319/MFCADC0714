#pragma once
#include <mysql.h>
#define DB_HOST "127.0.0.1"//서버 아이피
#define DB_USER "root"//DB 접속 계정
#define DB_PASS "1234"//DB 계정 암호
#define DB_NAME "adc_db"//DB 이름
class CMysqlController
{
public:
	CMysqlController();
	~CMysqlController();
	bool SelectQuery(char* sql, CString& result);
	bool InsertQuery(char* sql);
};