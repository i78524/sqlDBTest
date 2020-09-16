/******************************************************************************/
/* sql操作實作                                                                 */
/* Author:YiWen Chen                                                          */
/* Last update: 2020/07/31                                                    */
/*                                                                            */
/******************************************************************************/
#ifndef sqlexe_V20200731_H
#define sqlexe_V20200731_H

#include "tool.h"


#define SQL_boxDB_PATH   "./box.db"
#define SQL_boxDB        "CREATE TABLE box (name char(20), enable INT2, Syear INT2, Smonth INT2 , Sday INT2)"


// 與QL_boxDB的內容一一對應著
typedef struct _boxDATA
{
    char     name[21];
    uint8_t  enable;
    
	  uint16_t syear;
    uint8_t  smonth;
    uint8_t  sday;
}boxDATA;


typedef struct _boxDATAALL
{
  int totalNumUsed;     // 目前共用多少個
  boxDATA data[200];    // 0~199 共200個
}boxDATAALL;


#endif  // #ifndef _H
