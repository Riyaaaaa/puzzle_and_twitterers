//
//  dbIO.cpp
//  puzzle_twitterer
//
//  Created by Riya.Liel on 2015/03/13.
//
//

#include "dbIO.h"

dbIO::dbIO(){
    auto filepath = "paTwitterer.sqlite3";
    
    auto status = sqlite3_open(filepath, &useDataBase);
    if (status != SQLITE_OK){
        CCLOG("opne failed : %s", errorMessage);
    }else{
        CCLOG("open sucessed");
    }
    
    //sqlite3_close(useDataBase);

}

static int callback(void *notUsed, int argc, char **argv, char **colNames) {
    int i;
    for (i = 0; i < argc; i++) {
        CCLOG("%s=%s\n", colNames[i], argv[i] ? argv[i] : "NULL");
    }
    CCLOG("\n");
    return 0;
}

int dbIO::getElement(int ID){
    switch (ID) {
        case 1:
            return 4;
            break;
        case 2:
            return 3;
            break;
        case 3:
            return 1;
            break;
        case 4:
            return 2;
            break;
        case 5:
            return 5;
            break;
        default:
            break;
    }
    return -1;
}

int dbIO::getHp(int ID){
    switch (ID) {
        case 1:
            return 2000;
            break;
        case 2:
            return 670;
            break;
        case 3:
            return 1200;
            break;
        case 4:
            return 1000;
            break;
        case 5:
            return 1300;
            break;
        default:
            break;
    }
    return -1;
}
int dbIO::getAttack(int ID){
    switch (ID) {
        case 1:
            return 100;
            break;
        case 2:
            return 250;
            break;
        case 3:
            return 170;
            break;
        case 4:
            return 120;
            break;
        case 5:
            return 200;
            break;
        default:
            break;
    }
    return -1;
}
int dbIO::getHeal(int ID){
    switch (ID) {
        case 1:
            return 100;
            break;
        case 2:
            return 80;
            break;
        case 3:
            return 120;
            break;
        case 4:
            return 310;
            break;
        case 5:
            return 200;
            break;
        default:
            break;
    }
    return -1;
}

std::string dbIO::getName(int ID){
    switch (ID) {
        case 1:
            return "symptte";
            break;
        case 2:
            return "hal";
            break;
        case 3:
            return "riya";
            break;
        case 4:
            return "gonzabu";
            break;
        case 5:
            return "soshik";
            break;
        default:
            break;
    }
    return "";
}

/*
int dbIO::getElement(int ID){
    sqlite3_stmt *pStmt = nullptr;
    int err = sqlite3_prepare_v2(useDataBase, "SELECT * FROM twitterer", 64, &pStmt, nullptr);
    
    if(err != SQLITE_OK){
        CCLOG("error");
    }else{
        // データの抽出
        while(SQLITE_ROW == (err = sqlite3_step(pStmt)) ){
            int id = sqlite3_column_int(pStmt, 0);
            const unsigned char *name = sqlite3_column_text(pStmt, 1);
            printf("id: %d, name: %s\n", id, name);
        }
        
        if(err != SQLITE_DONE){
             //TODO: エラー処理
        }
    }
    return 0;
}
*/