#include "apue.h"
#include "apue_db.h"
#include <fcntl.h>

void printfDB(DBHANDLE db) {
  db_rewind(db);
  char *ptr = NULL;
  char key[IDXLEN_MAX];
  while ((ptr = db_nextrec(db, key)) != NULL) {
    printf("key: %s \t value: %s \n", key, ptr);
  }
  printf("---------end------\n");
}

int main(void)
{
	DBHANDLE	db;

	if ((db = db_open("dbtest", O_RDWR | O_CREAT | O_TRUNC,
	  FILE_MODE)) == NULL)
		err_sys("db_open error");

	if (db_store(db, "Alpha", "data1", DB_INSERT) != 0)
		err_quit("db_store error for alpha");
	if (db_store(db, "beta", "Data for beta", DB_INSERT) != 0)
		err_quit("db_store error for beta");
	if (db_store(db, "gamma", "record3", DB_INSERT) != 0)
		err_quit("db_store error for gamma");
	printfDB(db);
	db_delete(db,"gamma");
	printfDB(db);
	if (db_store(db, "gamma", "record3", DB_INSERT) != 0)
		err_quit("db_store error for gamma");
	printfDB(db);
	db_close(db);
	exit(0);
}
