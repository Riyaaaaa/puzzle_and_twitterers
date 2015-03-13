//
//  Drop.h
//  puzzle_twitterer
//
//  Created by Riya.Liel on 2015/03/08.
//
//

#ifndef __puzzle_twitterer__Drop__
#define __puzzle_twitterer__Drop__

#include"cocos2d.h"
#include"StdParameters.h"

using posIndex = cocos2d::Vec2;

class Drop : public cocos2d::Sprite{
    
public:
    enum Element{
        FIRE=1,
        WATER,
        THUNDER,
        DARK,
        TREE,
        HEAL,
        NONE,
    };
    
    CC_SYNTHESIZE_READONLY(Element, _Element, Element);
    CC_SYNTHESIZE(posIndex, _pos, PosIndex);
    CC_SYNTHESIZE(int, removeNo, RemoveNo);
    CC_SYNTHESIZE(int, _targetY, FallToTargetY);
    
    static Drop* create(Element type);
    virtual bool init(Element type);
    
    void setFiledWithPosIndex(posIndex pos);
    void setFiledWithPosIndexAndDelay(posIndex pos,int delay);
    static int generateTag(posIndex pos){return (int)pos.y*WIDTH_DROP_NUM+(int)pos.x;}
    
    void kill(){valid=false;};
    bool is_valid(){return valid;}
    
    
private:
    bool valid=true;
};

#endif /* defined(__puzzle_twitterer__Drop__) */
