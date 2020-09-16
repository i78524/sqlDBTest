/******************************************************************************/
/* sql操作實作                                                                 */
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

#include "dbTest.h"



/* 列印boxDATAALL */
int printfboxDATAALL(boxDATAALL* dataAll)
{
  printf("dataAll->totalNumUsed = %d \n", dataAll->totalNumUsed);
  int i = 0;
  for(i = 0; i <dataAll->totalNumUsed; i++) {
    printf("dataAll->data[%d].name   = %s\n", i, dataAll->data[i].name);
    printf("dataAll->data[%d].enable = %d\n", i, dataAll->data[i].enable);
    printf("dataAll->data[%d].syear  = %d\n", i, dataAll->data[i].syear);
    printf("dataAll->data[%d].smonth = %d\n", i, dataAll->data[i].smonth);
    printf("dataAll->data[%d].sday   = %d\n", i, dataAll->data[i].sday);
  }
}



/* 執行完指令後 如果有顯示結果 可以用這樣形式的函式來承接 */
/*    para 為 要承接結果的指標 */
/*    n_column 為 執行結果有幾行 */
/*    column_value 為 執行結果的每行的內容 */
/*    column_name  為 執行結果的每行的名稱 */
/*    注意 函式一定要是int                */
int callback4GetALLDataArray_boxDATAALL(void *para, int n_column, char** column_value, char** column_name) {

    //printf("[%s] n_column = %d\n", __func__, n_column);
    //printf("[%s] column_name[0] = %s\n", __func__, column_name[0] );
    //printf("[%s] column_name[1] = %s\n", __func__, column_name[1] );
    //printf("[%s] column_name[2] = %s\n", __func__, column_name[2] );
    //printf("[%s] column_name[3] = %s\n", __func__, column_name[3] );
    //printf("[%s] column_name[4] = %s\n", __func__, column_name[4] );

    boxDATAALL* dataAll = (boxDATAALL*)para;

    int i = dataAll->totalNumUsed; // 確認總量
    if(i < 200) { //我只吃 0~199個 共200個
      // 複製內容
      strcpy((dataAll->data[i]).name, column_value[0]);
      (dataAll->data[i]).enable = atoi(column_value[1]);
      (dataAll->data[i]).syear  = atoi(column_value[2]);
      (dataAll->data[i]).smonth = atoi(column_value[3]);
      (dataAll->data[i]).sday   = atoi(column_value[4]);

      // 更新總量
      dataAll->totalNumUsed = (dataAll->totalNumUsed) + 1;
    }
    return 0;
}





/* 取得boxDB資料庫中所有資料 */
/*  注意：指標sdAll 用來承接執行結果 */
/*    成功回傳 0    */
/*    失敗回傳 -1   */
int GetALLdata_boxDB(sqlite3 *db, boxDATAALL* sdAll) {

    memset(sdAll, 0, sizeof(boxDATAALL));

    char *sqlCmd = NULL;
    char *errMsg = NULL;
    int rc;
    sqlCmd = sqlite3_mprintf("SELECT * FROM box");

    if((rc = sqlite3_exec(db, sqlCmd, callback4GetALLDataArray_boxDATAALL, (void*)sdAll, &errMsg)) !=SQLITE_OK){
      printf("[%s] %s \n", __func__, errMsg);
      sqlite3_free(errMsg);
      sqlite3_free(sqlCmd);
      return -1;
    }

    sqlite3_free(sqlCmd);
    return 0;
}





/* 對boxDB資料庫插入資料 */
/*    成功回傳 0    */
/*    失敗回傳 -1   */
int insert_boxDB(sqlite3 *db, boxDATA data)
{
    // 建立指令字串 插入DB
    char tmpStr[250];
    memset(tmpStr, 0, sizeof(tmpStr)); 
    sprintf(tmpStr, "INSERT INTO box VALUES ('%s', %d, %d, %d, %d)", data.name, data.enable, data.syear, data.smonth, data.sday);

    return execSqlCmd(db, tmpStr);
}




/* 對boxDB資料庫修改資料 */
/*    成功回傳 0    */
/*    失敗回傳 -1   */
int update_boxDB(sqlite3 *db, boxDATA data)
{
    // 建立指令字串 插入DB
    char tmpStr[250];
    memset(tmpStr, 0, sizeof(tmpStr)); 
    sprintf(tmpStr, "UPDATE box SET enable=%d, Syear=%d  WHERE name='%s'", data.enable, data.syear, data.name);

    return execSqlCmd(db, tmpStr);
}




int main(int argc,char**argv) {

    boxDATAALL dataAll;
    sqlite3 *sqlboxPtr;

    // 檢查資料庫boxDB是否存在，如果不存在則會建立一份
    if(checksqlDB_General(SQL_boxDB_PATH, SQL_boxDB) != 0) {
      printf("[%s] fail part01\n", __func__);            
      return -1;
    }

    // 將指標sqlboxPtr指向資料庫boxDB
    if(sqlite3_open_v2(SQL_boxDB_PATH, &sqlboxPtr, SQLITE_OPEN_READWRITE, NULL)) {
      printf("[%s] Open %s failed!\n", __func__, SQL_boxDB_PATH);
      printf("[%s] fail part02\n", __func__);            
      return -1;
    }



    // 輸入一筆空白資料
    boxDATA data;
    memset(&data, 0, sizeof(data));
    strcpy(data.name, "ABC");
    if(insert_boxDB(sqlboxPtr, data) != 0) {
      printf("[%s] fail part03\n", __func__);            
      return -1;
    }

    // 從資料庫讀出所有資料
    if(GetALLdata_boxDB(sqlboxPtr, &dataAll) != 0) {
      printf("[%s] fail part04\n", __func__);            
      return -1;
    }
    printfboxDATAALL(&dataAll);// 列印出來


    // 修改一筆資料
    data.enable = 123;
    data.syear  = 123;
    if(update_boxDB(sqlboxPtr, data) != 0) {
      printf("[%s] fail part05\n", __func__);            
      return -1;
    }
    

    // 從資料庫讀出所有資料
    if(GetALLdata_boxDB(sqlboxPtr, &dataAll) != 0) {
      printf("[%s] fail part06\n", __func__);            
      return -1;
    }
    printfboxDATAALL(&dataAll);// 列印出來


    return 0;
}
