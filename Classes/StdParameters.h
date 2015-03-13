//
//  StdParameters.h
//  puzzle_twitterer
//
//  Created by Riya.Liel on 2015/03/08.
//
//

#ifndef puzzle_twitterer_StdParameters_h
#define puzzle_twitterer_StdParameters_h

const int WIDTH_DROP_NUM = 6;
const int HEIGHT_DROP_NUM = 5;

const int DROP_SIZE = 106;

enum ZOrder{
    BACKGROUND=0,
    Z_FIELD,
    DROP,
};

    enum objectID{
    ID_FIELD=101,
};

#define CREATE_FUNC_WITH_ARUGMENT(__TYPE__,...) \
static __TYPE__* create(__VA_ARGS__) \
{ \
__TYPE__ *pRet = new __TYPE__(); \
if (pRet && pRet->init()) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

#endif
