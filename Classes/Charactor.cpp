//
//  Charactor.cpp
//  puzzle_twitterer
//
//  Created by Riya.Liel on 2015/03/11.
//
//

#include "Charactor.h"

Charactor::Charactor(){
    
}

Twitterer::Twitterer(): _chain(0){
    damageLabel = cocos2d::Label::createWithTTF("", "fonts/Marker Felt.ttf", 100);
    damageLabel->setVisible(false);
    this->addChild(damageLabel);
}

void Twitterer::pushDamage(double damage){
    auto seq = cocos2d::Sequence::create(cocos2d::ScaleTo::create(0.3f,1.3f),cocos2d::ScaleTo::create(0.3f,1.0f),nullptr);
    
    _damage += static_cast<unsigned int>(damage*_attack);
    
    if(!damageLabel->isVisible()){
        damageLabel->setVisible(true);
        damageLabel->setPosition(cocos2d::Vec2(getContentSize().width/2,getContentSize().height+50));
    }
    
    damageLabel->setString(std::to_string(_damage));
    damageLabel->runAction(seq);
}

void Twitterer::pushChain(int chain){
    _chain += chain;
}

unsigned int Twitterer::getDamage(){
    auto moveby = cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(0,20.0f));
    auto moveFrom = cocos2d::MoveBy::create(0.5f, cocos2d::Vec2(0,-20.0f));
    this->runAction(cocos2d::Sequence::create(moveby,moveFrom,nullptr));
    return _damage*_attack;
}

void Twitterer::setExtraDamage(double extra){
    _damage *= extra;
}

Twitterer* Twitterer::create(std::string filename,Drop::Element element){
    Twitterer* pRet = new Twitterer;
    
    if(pRet && pRet->init(filename,element)){
        pRet->autorelease();
    }
    else{
        delete  pRet;
        pRet = nullptr;
    }
    
    return pRet;
}

bool Twitterer::init(std::string filename,Drop::Element element){
    
    std::string frame_path = "element_frame" + std::to_string(element) + ".png";
    
    if(!Sprite::initWithFile(filename))return false;
    
    this->cocos2d::Node::setScale(cocos2d::Director::getInstance()->getVisibleSize().width/5./400.);
    this->setAnchorPoint(cocos2d::Vec2(0.0f,0.5f));
    
    auto frame = cocos2d::Sprite::create(frame_path);
    frame->setAnchorPoint(cocos2d::Vec2::ZERO);
    frame->setScale(this->getContentSize().width/127.);
    this->addChild(frame);
    
    _element = element;
    
    return true;
}

void Twitterer::calcDamage(){
    auto seq = cocos2d::Sequence::create(cocos2d::ScaleTo::create(0.3f,1.3f),cocos2d::ScaleTo::create(0.3f,1.0f),nullptr);
    _damage *= (1.0 + 0.1*_chain);
    //Enhance
    
    damageLabel->setString(std::to_string(_damage));
    damageLabel->runAction(seq);
}

bool Enemy::init(std::string filename,Drop::Element element,enemy_pos pos){
    std::string frame_path = "element_frame" + std::to_string(element) + ".png";
    cocos2d::Size VisibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    if(!Sprite::initWithFile(filename))return false;
    
    this->cocos2d::Node::setScale(VisibleSize.width/3./400.);
    
    auto frame = cocos2d::Sprite::create(frame_path);
    frame->setAnchorPoint(cocos2d::Vec2::ZERO);
    frame->setScale(this->getContentSize().width/127.);
    this->addChild(frame);
    
    _hpBar = cocos2d::Sprite::create("hp_bar.png");
    
    auto hp_frame = cocos2d::Sprite::create("hp_frame.png");
    hp_frame->setScale(getContentSize().width /hp_frame->getContentSize().width);
    hp_frame->setAnchorPoint(cocos2d::Vec2(0.0f,1.0f));
    addChild(hp_frame);
    
    hpTimer = cocos2d::ProgressTimer::create(_hpBar);
    hpTimer->setType(cocos2d::ProgressTimer::Type::BAR);
    hpTimer->setBarChangeRate(cocos2d::Vec2(1, 0));
    hpTimer->setMidpoint(cocos2d::Vec2(0, 0));
    hpTimer->setScale(getContentSize().width /hpTimer->getContentSize().width);
    hpTimer->setPercentage(100);
    hpTimer->setAnchorPoint(cocos2d::Vec2(0.0f,1.0f));
    addChild(hpTimer);
    
    _element = element;
    _pos = pos;
    
    switch (_pos) {
        case enemy_pos::CENTER:
            setPosition(cocos2d::Vec2(VisibleSize.width/2,VisibleSize.height-200));
            break;
        case enemy_pos::RIGHT:
            setPosition(cocos2d::Vec2(VisibleSize.width/3*2,VisibleSize.height-200));
            break;
        case enemy_pos::LEFT:
            setPosition(cocos2d::Vec2(VisibleSize.width/3,VisibleSize.height-200));
            break;
        default:
            break;
    }
    
    return true;
}

Enemy* Enemy::create(std::string filename,Drop::Element element, enemy_pos pos){
    Enemy* pRet = new Enemy;
    
    if(pRet && pRet->init(filename,element,pos)){
        pRet->autorelease();
    }
    else{
        delete  pRet;
        pRet = nullptr;
    }
    
    return pRet;
}

Enemy::Enemy(){}

void Enemy::attackToEnemy(unsigned int damage){
    if(_hp<damage)_hp=0;
    else _hp-=damage;
}

void Enemy::attackToPlayer(){
    
}

void Enemy::refHpBar(){
    auto progress = cocos2d::ProgressTo::create(1.0f,_hp/static_cast<float>(_maxHp) * 100.0f);
    hpTimer->runAction(progress);
}

