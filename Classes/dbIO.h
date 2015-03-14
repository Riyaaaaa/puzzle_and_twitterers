//
//  dbIO.h
//  puzzle_twitterer
//
//  Created by Riya.Liel on 2015/03/13.
//
//

#ifndef __puzzle_twitterer__dbIO__
#define __puzzle_twitterer__dbIO__

#include <string>
#include "sqlite3.h"

class dbIO{
public:
    std::string getName(int ID);
    int getElement(int ID);
    int getHp(int ID);
    int getAttack(int ID);
    int getHeal(int ID);
    
    static dbIO* getInstance(){static dbIO obj; return &obj;}
    
    dbIO();
private:
    sqlite3* useDataBase = NULL;
    char* errorMessage = NULL;
    
    static int getCallBack(void* aux,int size,char** data,char **ColName){
        int* value = reinterpret_cast<int*>(aux);
        *value = atoi(data[0]);
        CCLOG("get string = %s",data[0]);
        return 0;
    }
};

#endif /* defined(__puzzle_twitterer__dbIO__) */
