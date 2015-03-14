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
    GameLayer();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameLayer);
    
    void createDrop(posIndex _pos);
    void initField();
    
private:
    const int REMOVE_DELAY=1.0f;
    
    std::mt19937 _engine;
    std::discrete_distribution<int> _distribution;
    
    cocos2d::Sprite* field;
    
    bool is_moved=false;
    bool is_removed=false;
    
    Drop* _activeDrop;
    Drop* getDropByTouched(cocos2d::Vec2 pos);
    Drop* getDropByPosIndex(posIndex pos);
    
    std::vector<Twitterer*> twitterers;
    
    int _hp;
    int _maxHp;
    int _healPower;
    
    int countKillSymptte=-1;
    
    void puzzleSeqence();
    void removeDropWithAnimation(vector2<Drop*>);
    void removeDropWithAnimation(std::vector<Drop*>,int,int);
    void fallDropWithAnimation();
    void replaceDropWithAnimation();
    
    void calcDamage(std::vector<Drop*>);
    void calcDamage(Drop::Element,int);
    
    void attack();
    
    vector2<Drop*> searchDropConnected();
    void progressTurn();
    void changeDrop(Drop*,Drop*);
    
    virtual bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    virtual void onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* unused_event);
    
    void makeCharactors(); //debug
    void initPlayerStatus();
    
    void refHpBar();
    
    void nextEnemy();
    
    Enemy* targetEnemy;
    
    cocos2d::ProgressTimer* hpTimer;
    cocos2d::Label* countLabel;
};

#endif // __HELLOWORLD_SCENE_H__
