#include "GameLayer.h"
#include <random>

USING_NS_CC;

GameLayer::GameLayer(){
    std::random_device rnd;
    _engine =std::mt19937(rnd());
    _distribution=std::discrete_distribution<int>{20,20,20,20,20,10};
    
}

Scene* GameLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    
    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameLayer::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    field = Sprite::create("field.png");
    field->setAnchorPoint(Vec2::ZERO);
    field->setPosition(Vec2::ZERO);
    // add the sprite as a child to this layer
    this->addChild(field, ZOrder::Z_FIELD,objectID::ID_FIELD);
    
    initField();
    
    auto bar_frame = cocos2d::Sprite::create("hp_frame.png");
    auto hp_bar = cocos2d::Sprite::create("hp_bar.png");
    
    hpTimer = cocos2d::ProgressTimer::create(hp_bar);
    hpTimer->setType(cocos2d::ProgressTimer::Type::BAR);
    hpTimer->setBarChangeRate(cocos2d::Vec2(1, 0));
    hpTimer->setMidpoint(cocos2d::Vec2(0, 0));
    hpTimer->setPercentage(100);
    hpTimer->setAnchorPoint(cocos2d::Vec2::ZERO);
    addChild(hpTimer);
    
    bar_frame->setAnchorPoint(Vec2::ZERO);
    
    bar_frame->setPosition(Vec2(0,field->getContentSize().height+5));
    hpTimer->setPosition(Vec2(0,field->getContentSize().height+5));
    
    this->addChild(bar_frame);
    this->addChild(hp_bar);
    
    countLabel = cocos2d::Label::createWithTTF("killed symptte: 0", "fonts/Marker Felt.ttf", 20);;
    countLabel->setAnchorPoint(Vec2(0.0f,1.0f));
    countLabel->setPosition(cocos2d::Size(0,visibleSize.height));
    this->addChild(countLabel);
    
    nextEnemy();
    
    auto touchListner = EventListenerTouchOneByOne::create();
    touchListner->setSwallowTouches(true);
    
    touchListner->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan,this);
    touchListner->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved,this);
    touchListner->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded,this);
    touchListner->onTouchCancelled = CC_CALLBACK_2(GameLayer::onTouchCancelled,this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListner, this);
    
    makeCharactors();
    initPlayerStatus();
    
    return true;
}


void GameLayer::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameLayer::createDrop(posIndex _pos){
    int type = _distribution(_engine)+1;
    
    auto drop = Drop::create(static_cast<Drop::Element>(type));
    drop->setFiledWithPosIndex(_pos);
    addChild(drop,ZOrder::DROP,Drop::generateTag(_pos));
    
}

void GameLayer::initField(){
    
    for(int i=0;i<HEIGHT_DROP_NUM;i++){
        for(int j=0;j<WIDTH_DROP_NUM;j++){
            createDrop(posIndex(j,i));
        }
    }
}

bool GameLayer::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    _activeDrop = getDropByTouched(touch->getLocation());
    
    if(_activeDrop!=nullptr){
        _activeDrop->setZOrder(ZOrder::DROP+1);
        return true;
    }
    else return false;
    
}
void GameLayer::onTouchMoved(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    auto newDrop = getDropByTouched(touch->getLocation());
    if(newDrop != nullptr){
        
        if(newDrop != _activeDrop){
            
            changeDrop(_activeDrop, newDrop);
            
            is_moved = true;
        }
        _activeDrop->setPosition(touch->getLocation());
    }
    else {
        EventTouch cancellevent; //dispatch touch cancell event
        cancellevent.setEventCode(EventTouch::EventCode::CANCELLED);
        cancellevent.setTouches(std::vector<Touch*>{touch});
        _eventDispatcher->dispatchEvent(dynamic_cast<Event*>(&cancellevent));
    }
    
}

void GameLayer::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    _activeDrop->setZOrder(ZOrder::DROP);
    _activeDrop->setFiledWithPosIndex(_activeDrop->getPosIndex()); //reset position
    _activeDrop=nullptr;
    
    if(is_moved){
        _eventDispatcher->setEnabled(false);
        puzzleSeqence();
        is_moved=false;
    }
    
}

void GameLayer::onTouchCancelled(cocos2d::Touch* touch,cocos2d::Event* unused_event){
    onTouchEnded(touch,unused_event);
}

Drop* GameLayer::getDropByTouched(Vec2 pos){
    posIndex index;
    
    Rect rect = field->boundingBox();
    
    if(rect.containsPoint(pos)){
    }
    else return nullptr;
    
    index = pos / this->getChildByTag(0)->getContentSize().width;
    
    return getDropByPosIndex(index);
    
}

Drop* GameLayer::getDropByPosIndex(posIndex index){
    int tag = Drop::generateTag(index);
    Node* ptr = this->getChildByTag(tag);
    return dynamic_cast<Drop*>(ptr);
}

void GameLayer::puzzleSeqence(){
    vector2<Drop*> removeList;
    
    removeList = searchDropConnected();
    if(removeList.empty()){
        _eventDispatcher->setEnabled(true);
        attack();
        progressTurn();
        return;
    }
    auto fall = CallFunc::create(CC_CALLBACK_0(GameLayer::fallDropWithAnimation, this));
    auto replace = CallFunc::create(CC_CALLBACK_0(GameLayer::replaceDropWithAnimation, this));
    auto recursive = CallFunc::create(CC_CALLBACK_0(GameLayer::puzzleSeqence, this));
    auto delay1 = DelayTime::create(removeList.size()*ACTION_DELAY);
    auto delay2 = DelayTime::create(ACTION_DELAY);
    
    removeDropWithAnimation(removeList);
    this->runAction(Sequence::create(delay1, fall, delay2, replace, delay2->clone(), recursive,  nullptr));
}

vector2<Drop*> GameLayer::searchDropConnected(){
    Drop::Element subject,target;
    vector2<int> removeMap;
    vector2<Drop*> removeList;
    
    int count,removeNo=0;
    
    removeMap.resize(HEIGHT_DROP_NUM);
    for(auto &v: removeMap){
        v.resize(WIDTH_DROP_NUM);
        std::fill(v.begin(),v.end(),-1);
    }
    
    
    for(int i=0;i<HEIGHT_DROP_NUM;i++){
        count =0;
        for(int j=0;j<WIDTH_DROP_NUM;j++){
            subject = getDropByPosIndex(posIndex(j,i))->getElement();
            if(j != WIDTH_DROP_NUM-1)target = getDropByPosIndex(posIndex(j+1,i))->getElement();
            else target = Drop::NONE;
            if(subject==target){
                count++;
                continue;
            }
            if(count > 1){
                int tmp=removeNo;
                for(int k=j-count;k<=j;k++){
                    if(tmp==removeNo && removeMap[i][k] != -1){
                        tmp = removeMap[i][k];
                        k=j-count-1;
                        continue;
                    }
                    removeMap[i][k] = tmp;
                }
                if(tmp == removeNo)removeNo++;
            }
            count =0;
        }
    }
    
    for(int j=0;j<WIDTH_DROP_NUM;j++){
        count =0;
        for(int i=0;i<HEIGHT_DROP_NUM;i++){
            subject = getDropByPosIndex(posIndex(j,i))->getElement();
            if(i != HEIGHT_DROP_NUM-1)target = getDropByPosIndex(posIndex(j,i+1))->getElement();
            else target = Drop::NONE;
            if(subject==target){
                count++;
                continue;
            }
            if(count > 1){
                int tmp=removeNo;
                for(int k=i-count;k<=i;k++){
                    if(tmp==removeNo && removeMap[k][j] != -1){
                        tmp = removeMap[k][j];
                        k=i-count-1;
                        continue;
                    }
                    removeMap[k][j] = tmp;
                }
                if(tmp==removeNo)removeNo++;
            }
            count =0;
        }
    }
    
    removeList.resize(removeNo);
    
    bool flag=false;
    for(int i=0;i<HEIGHT_DROP_NUM;i++){
        for(int j=0;j<WIDTH_DROP_NUM;j++){
            if(removeMap[i][j] != -1){
                flag=true;
                removeList[removeMap[i][j]].push_back(getDropByPosIndex(posIndex(j,i)));
            }
        }
    }
    
    if(!flag)removeList.clear();
    
    return removeList;
}

void GameLayer::removeDropWithAnimation(vector2<Drop*> removeList){
    for(int i=0;i<removeList.size();i++){
        removeDropWithAnimation(removeList[i],i,removeList.size()-1);
    }
}

void GameLayer::removeDropWithAnimation(std::vector<Drop*> targetDrops,int removeNo,int maxNo){
    auto delay1 = DelayTime::create(ACTION_DELAY*removeNo);
    auto fade = FadeTo::create(ACTION_DELAY, 0);
    auto delay2 = DelayTime::create(ACTION_DELAY*(maxNo-removeNo));
    auto removeSelf = RemoveSelf::create(true);
    
    Drop::Element element = targetDrops[0]->getElement();

    int size = targetDrops.size();
    
    auto calcDM = CallFuncN::create([element,size,this](Ref* obj){
        this->calcDamage(element,size);
    });
    
    this->runAction(Sequence::create(delay1, DelayTime::create(ACTION_DELAY), calcDM, nullptr));
    
    for(auto ptr: targetDrops){
        ptr->kill();
        ptr->runAction(Sequence::create(delay1->clone(),fade->clone(),delay2->clone(),removeSelf->clone(), nullptr));
    }
}

void GameLayer::fallDropWithAnimation(){
    Drop* targetDrop;
    int targetY;
    for(int j=0;j<WIDTH_DROP_NUM;j++){
        targetY = 0;
        for(int i=0;i<HEIGHT_DROP_NUM;i++){
            targetDrop = getDropByPosIndex(posIndex(j,i));
            if(targetDrop!=nullptr && targetDrop->is_valid()){
                targetDrop->setFallToTargetY(targetY);
                targetDrop->setFiledWithPosIndexAndDelay(posIndex(j,targetY), ACTION_DELAY);
                targetY++;
            }
        }
    }
}

void GameLayer::replaceDropWithAnimation(){
    Drop* targetDrop,*fallDrop;
    int targetY;
    for(int j=0;j<WIDTH_DROP_NUM;j++){
        targetY = 0;
        for(int i=0;i<HEIGHT_DROP_NUM;i++){
            targetDrop = getDropByPosIndex(posIndex(j,i));
            if(targetDrop==nullptr || !targetDrop->is_valid()){
                for(int k=i;k<HEIGHT_DROP_NUM;k++){
                    createDrop(posIndex(j,k+5));
                    fallDrop = getDropByPosIndex(posIndex(j,k+5));
                    fallDrop->setFiledWithPosIndexAndDelay(posIndex(j,k),1.0f);
                }
                break;
            }
        }
    }
}

void GameLayer::progressTurn(){
    if(targetEnemy->isLiving()){
        if(targetEnemy->isAttackTurn()){
            
        _hp -= targetEnemy->getAttack();
        if(_hp<0)_hp=0;
            
            if(_hp == 0){
                std::string str = "LOSE";
                
                auto bonusLabel = cocos2d::Label::createWithTTF(str, "fonts/Marker Felt.ttf", 150);
                bonusLabel->setScale(0.02f);
                bonusLabel->setColor(cocos2d::Color3B::RED);
                bonusLabel->setPosition(cocos2d::Director::getInstance()->getVisibleSize()/2);
                this->addChild(bonusLabel);
                bonusLabel->runAction(Sequence::create(DelayTime::create(2.0f),ScaleTo::create(1.0f, 1.0f),DelayTime::create(0.5f),nullptr));
                
                _eventDispatcher->setEnabled(false);
            }
            
        }
    }
    
    else {
        std::string str = "next stage";
        
        auto bonusLabel = cocos2d::Label::createWithTTF(str, "fonts/Marker Felt.ttf", 120);
        bonusLabel->setScale(0.02f);
        bonusLabel->setColor(cocos2d::Color3B::WHITE);
        bonusLabel->setPosition(cocos2d::Director::getInstance()->getVisibleSize()/2+cocos2d::Size(0,200));
        this->addChild(bonusLabel);
        bonusLabel->runAction(Sequence::create(DelayTime::create(2.0f),ScaleTo::create(1.0f, 1.0f),DelayTime::create(0.5f),FadeOut::create(0.5f),RemoveSelf::create(),nullptr));
        
        targetEnemy->runAction(Sequence::create(FadeTo::create(3.0f, 0),RemoveSelf::create(),CallFunc::create(CC_CALLBACK_0(GameLayer::nextEnemy,this)), nullptr));
    }
    
    auto progress = cocos2d::ProgressTo::create(1.0f,_hp/static_cast<float>(_maxHp) * 100.0f);
    hpTimer->runAction(progress);
}

void GameLayer::calcDamage(std::vector<Drop*> drops){
    double damage;
    
    damage = drops.size()/3.0;
    for(auto &member: twitterers){
        if(drops[0]->getElement()==member->getElement()){
            member->pushDamage(damage);
        }
    }
    
    for(auto &member: twitterers){
        member->pushChain(1);
    }
    
}

void GameLayer::calcDamage(Drop::Element element,int size){
    double damage;
    
    damage = size/3.0;
    
    if(element == Drop::Element::HEAL){
        _hp += damage*_healPower;
        if(_hp>_maxHp)_hp = _maxHp;
    }
    
    for(auto &member: twitterers){
        if(element==member->getElement()){
            member->pushDamage(damage);
        }
    }
    
    for(auto &member: twitterers){
        member->pushChain(1);
    }
    
}

void GameLayer::changeDrop(Drop* subject,Drop* target){
    posIndex newindex = target->getPosIndex();
    
    target->setFiledWithPosIndexAndDelay(subject->getPosIndex(),0.5f);
    
    subject->setPosIndex(newindex);
    subject->setTag(Drop::generateTag(newindex));
}


void GameLayer::makeCharactors(){
    for(int i=0;i<5;i++){
        twitterers.push_back(Twitterer::create(i+1));
        twitterers[i]->setPosition(Director::getInstance()->getVisibleSize().width/5*i,field->getContentSize().height+105);
        this->addChild(twitterers[i]);
    }
}

void GameLayer::attack(){
    for(auto &member: twitterers){
        member->calcDamage();
        targetEnemy->attackToEnemy(member->getDamage());
    }
    
    targetEnemy->refHpBar();
    
    int chain = twitterers[0]->getChain();
    std::string str = std::to_string(chain) + " Chain!";
    
    auto bonusLabel = cocos2d::Label::createWithTTF(str, "fonts/Marker Felt.ttf", 120);
    bonusLabel->setScale(0.02f);
    bonusLabel->setColor(cocos2d::Color3B::RED);
    bonusLabel->setPosition(cocos2d::Director::getInstance()->getVisibleSize()/2);
    this->addChild(bonusLabel);
    bonusLabel->runAction(Sequence::create(ScaleTo::create(1.0f, 1.0f),DelayTime::create(0.5f),FadeOut::create(0.5f),RemoveSelf::create(),nullptr));
    
}

void GameLayer::initPlayerStatus(){
    _maxHp=0;
    _healPower=0;
    
    for(auto &member: twitterers){
        _maxHp += member->getHP();
        _healPower += member->getHealPower();
    }
    
    _hp = _maxHp;
}

void GameLayer::nextEnemy(){
    countKillSymptte++;
    countLabel->setString("killed symptte: "+std::to_string(countKillSymptte));
    
    targetEnemy = Enemy::create("symptte.png",Drop::Element::DARK,Enemy::enemy_pos::CENTER);
    targetEnemy->setMaxHp(100000);
    targetEnemy->setHP(100000);
    targetEnemy->setAttack(1000 * countKillSymptte + 1000);
    targetEnemy->setDeffence(1000);
    
    this->addChild(targetEnemy);
}

