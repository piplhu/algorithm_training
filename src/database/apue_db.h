#ifndef _H_APUR_DB_H_
#define _H_APUR_DB_H_

/* 数据库的有效应用句柄 */
typedef void *DBHANDLE;

/**
 * @brief 打开一个数据库
 *
 * @param path 数据库的路径
 * @param int 文件打开的方式读写
 * @param ... 文件权限
 *
 * @return 数据库句柄
 */
DBHANDLE db_open(const char *path, int, ...);

/**
 * @brief 关闭数据库
 *
 * @param DBHANDLE
 */
void db_close(DBHANDLE);
char *db_fetch(DBHANDLE, const char *);

/**
 * @brief 数据库操作
 *
 * @param DBHANDLE
 * @param
 * @param
 * @param int DB_FLAG
 *
 * @return
 */
int db_store(DBHANDLE, const char *, const char *, int);
int db_delete(DBHANDLE, const char *);
void db_rewind(DBHANDLE);
char *db_nextrec(DBHANDLE, char *);

/*DB_FLAG*/
#define DB_INSERT 1
#define DB_REPLACE 2
#define DB_STORE 3

#define IDXLEN_MIN                                                             \
  6 // 1字节键 1字节分隔符 1字节起始偏移量 1字节分隔符 1字节长度 1字节换行符
#define IDXLEN_MAX 1024

#define DATALEN_MIN 2
#define DATALEN_MAX 1024

#endif //_H_APUR_DB_H_
