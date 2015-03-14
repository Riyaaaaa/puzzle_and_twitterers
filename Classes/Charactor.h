//
//  Charactor.h
//  puzzle_twitterer
//
//  Created by Riya.Liel on 2015/03/11.
//
//

#ifndef __puzzle_twitterer__Charactor__
#define __puzzle_twitterer__Charactor__

#include "Drop.h"
#include "dbIO.h"

class Charactor : public cocos2d::Sprite{
public:
    CC_SYNTHESIZE(int, _hp, HP);
    CC_SYNTHESIZE(int, _maxHp, MaxHp);
    CC_SYNTHESIZE(int, _attack, Attack);
    CC_SYNTHESIZE(Drop::Element, _element, Element);
    CREATE_FUNC(Charactor);
    Charactor();
    
    bool isLiving(){return _hp > 0;}

protected:
    


};


class Enemy : public Charactor{
public:
    enum class enemy_pos{
        CENTER=0,
        RIGHT,
        LEFT
    };
    virtual bool init(std::string filename,Drop::Element element,enemy_pos pos);
    static Enemy* create(std::string filename,Drop::Element element,enemy_pos pos);
    
    Enemy();
    void attackToEnemy(unsigned int damage);
    void attackToPlayer();
    
    void countTurn();
    bool isAttackTurn(){return true;}; //debug
    
    void refHpBar();
    
    CC_SYNTHESIZE(int, _deffence, Deffence);
    //CC_SYNTHESIZE(int, _frequency, AttackFrequency);
private:
    
    cocos2d::Sprite* _hpBar;
    
    int _remainTurn;
    
    enemy_pos _pos;
    
    cocos2d::ProgressTimer* hpTimer;
};

class Twitterer :public Charactor{
public:
    virtual bool init(int ID);
    static Twitterer* create(int ID);
    Twitterer();

    void pushChain(int Chain);
    void pushDamage(double damage);
    unsigned int getDamage();
    void setExtraDamage(double extra);
    
    void calcDamage();
    void initDamage();
    
    CC_SYNTHESIZE(int, _heal, HealPower);
    CC_SYNTHESIZE_READONLY(int, _chain, Chain);
private:
    cocos2d::Label* damageLabel=nullptr;
    unsigned int _damage=0;
};

#endif /* defined(__puzzle_twitterer__Charactor__) */
