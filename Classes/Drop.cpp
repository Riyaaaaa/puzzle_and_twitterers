//
//  Drop.cpp
//  puzzle_twitterer
//
//  Created by Riya.Liel on 2015/03/08.
//
//

#include "Drop.h"

Drop* Drop::create(Drop::Element type){
    Drop* pDrop = new Drop;
    
    if(pDrop && pDrop->init(type)){
        pDrop->autorelease();
    }
    else{
        delete  pDrop;
        pDrop = nullptr;
    }
    
    return pDrop;
}


bool Drop::init(Element type){
    std::string filePath("ball");
    
    filePath += std::to_string(type) += ".png";
    
    if(!Sprite::initWithFile(filePath))return false;
    
    _Element = type;
    
    return true;
}

void Drop::setFiledWithPosIndex(posIndex pos){
    posIndex index = posIndex((int)pos.x,(int)pos.y);
    setPosition(index*DROP_SIZE + getContentSize()/2);
    setPosIndex(index);
    setTag(Drop::generateTag(index));
}

void Drop::setFiledWithPosIndexAndDelay(posIndex pos,int delay){
    posIndex index = posIndex((int)pos.x,(int)pos.y);
    auto move = cocos2d::MoveTo::create(delay,index*DROP_SIZE + getContentSize()/2);
    this->runAction(move);
    setPosIndex(index);
    setTag(Drop::generateTag(index));
}