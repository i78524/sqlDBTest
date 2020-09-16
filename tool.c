/******************************************************************************/
/* sql操作實作 基礎工具包                                                      */
/* Author:YiWen Chen                                                          */
/* Last update: 2020/07/31                                                    */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <sqlite3.h>
#include <stdint.h>

/***檢查檔案是否存在****/
/*    存在回傳 1    */
/*    不存在回傳 0   */
int fileisExist(char *path) {

  struct stat sts;

  if ((stat (path, &sts)) == -1){
    printf("The file %s doesn't exist...\n", path);
    return 0;
  } else {
    printf("The file %s has exist...\n", path);
    return 1;
  }
}





/* 建立sql資料庫 */
/*    成功回傳 0    */
/*    失敗回傳 -1   */
int creatSqlTable(char *filePath, char *sqliteTable) {

    sqlite3 *db;

    // 使用open來製造出檔案，並且讓變數db可以
    if (sqlite3_open_v2(filePath, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)){
        printf("[%s] Open %s failed! \n", __func__, filePath);
        return -1;
    }

    // 使用exec來建立sql資料庫的基本格式，指令來自sqliteTable
    int rc;
    char *errMsg = NULL;
    if((rc = sqlite3_exec(db, sqliteTable, 0, 0, &errMsg))!=SQLITE_OK){
       printf("[%s] %s \n", __func__, errMsg);
       sqlite3_free(errMsg);
       sqlite3_close(db);
       return -1;
    }

    sqlite3_close(db);
    return 0;
}



/* 執行sql指令，通常用於不需要承接執行結果的指令 */
/*    成功回傳 0    */
/*    失敗回傳 -1   */
int execSqlCmd(sqlite3 *db, char *cmd) {
    char *errMsg = NULL;
    int rc;
    if((rc = sqlite3_exec(db, cmd, 0, 0, &errMsg))!=SQLITE_OK){
       printf("[%s] %s \n", __func__, errMsg);
       sqlite3_free(errMsg);
       return -1;
    }
    return 0;
}



/* 前處理通用函式 */
/*    如果資料庫不存在 則創造資料庫並且對資料庫格式做初始化 */
int checksqlDB_General(char *filePath, char *createTable) {

  if (!fileisExist(filePath)) {
    if(creatSqlTable(filePath,createTable) != 0) {
       printf("[%s] fail\n", __func__);
       return -1;
    }
  }
  return 0;
}
