#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Drop.h"
#include "Charactor.h"
#include "StdParameters.h"
#include "dbIO.h"

template<typename T>
using vector2 = std::vector<std::vector<T>>;

class GameLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender); //end game
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameLayer);
    
private:
    GameLayer(); //Constructor
    
    void createDrop(posIndex _pos);
    void initField();
    
    Drop* getDropByTouched(cocos2d::Vec2 pos); //Touch Cordinate To PosIndex
    Drop* getDropByPosIndex(posIndex pos);
    
    /* CallBack Touch Event*/
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    
    void progressTurn();
    void changeDrop(Drop*,Drop*); //exchange posindex and tag
    
    void makeCharactors(); //debug
    void initPlayerStatus(); //init Player HP, HealPower
    
    void refHpBar(); //Refresh HP ProgressBar
    
    void nextEnemy(); //set Next Symptte
    
    void puzzleSeqence(); //search, remove, replace Sequence
    
    /*puzzle sequence method*/
    void removeDropWithAnimation(vector2<Drop*>);
    void removeDropWithAnimation(std::vector<Drop*>,int,int);
    void fallDropWithAnimation();
    void replaceDropWithAnimation();
    
    void calcDamage(std::vector<Drop*>);
    void calcDamage(Drop::Element,int);
    
    void attack();
    
    const int ACTION_DELAY=1.0f;
    
    std::mt19937 _engine;
    std::discrete_distribution<int> _distribution;
    
    cocos2d::Sprite* field;
    
    bool is_moved=false;
    bool is_removed=false;
    
    Drop* _activeDrop;
    
    std::vector<Twitterer*> twitterers;
    
    int _hp;
    int _maxHp;
    int _healPower;
    int countKillSymptte=-1;
    
    vector2<Drop*> searchDropConnected();
    
    Enemy* targetEnemy;
    
    cocos2d::ProgressTimer* hpTimer;
    cocos2d::Label* countLabel;
};

#endif // __HELLOWORLD_SCENE_H__
