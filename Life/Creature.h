//
//  Engine.hpp
//  Life
//
//  Created by Georgel Vintila on 24/10/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#ifndef Creature_h
#define Creature_h

#include "Resource.h"
#include "Constants.h"


class Creature {

private:
    int objId = 0;
    int strength;
    int speed;
    int lifeForce;
    int energy;
    int age;
    bool dead = false;
    bool hungry = false;
    int maxForce;
    char *description;
    int type;
    int genType;
    
    void checkDead();
    
    void setType(int type);
    Resource *resource;
    NormalCoord position;
    NormalCoord absolutePosition;
    
public:
    Creature();
    Creature(int strength, int energy,int speed, int force, int type);
    ~Creature();
    char* getDescription();
    int getID();
    bool isDead();
    bool isAdult();
    bool isHungry();
    void feed();
    void live(bool *canMult);
    
    int getStr();
    double getSpeed();
    int getForce();
    int getType();
    int getFoodValue();
    void die();

    NormalCoord getAbsolutePosition();
    
    void setPosition(NormalCoord pos);
    void setAbsolutePosition(NormalCoord pos);
    
    void multiply(Creature **cr);
    void friend mutate(Creature *cr);
    void friend crossover(Creature *crA, Creature *crB);
    double friend fitness(Creature *cr);
    bool friend selection(Creature *cr);
    void friend fight(Creature *crA, Creature *crB);
    bool friend shouldFight(Creature *crA, Creature *crB);
    
};


#endif
