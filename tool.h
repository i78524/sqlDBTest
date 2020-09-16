/******************************************************************************/
/* sql操作實作 基礎工具包                                                      */
/* Author:YiWen Chen                                                          */
/* Last update: 2020/07/31                                                    */
/*                                                                            */
/******************************************************************************/
#ifndef sqltool_V20200731_H
#define sqltool_V20200731_H

int fileisExist(char *path);
int creatSqlTable(char *filePath, char *sqliteTable);
int execSqlCmd(sqlite3 *db, char *cmd);
int checksqlDB_General(char *filePath, char *createTable);


#endif  // #ifndef _H

