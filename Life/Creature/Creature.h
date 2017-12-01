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
#include "Attributes.h"
#include "Constants.h"


class Creature {

private:
    int objId = 0;
    
    int age;
    bool dead = false;
    bool hungry = false;
    
    char *description;
    int type;
    int genType;
    int screen;
    
    void checkDead();
    
    void setType(int type);
    Attributes attributes;
    Resource *resource;
    NormalCoord position;
    NormalCoord absolutePosition;
    
public:
    Creature();
    Creature(Attributes attrib, int type);
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
    double fitness();
    NormalCoord getAbsolutePosition();
    
    void setPosition(NormalCoord pos);
    void setAbsolutePosition(NormalCoord pos);
    
    void multiply(Creature **cr,NormalCoord pos);
    void friend mutate(Creature *cr);
    void friend crossover(Creature *crA, Creature *crB);
    
    bool friend selection(Creature *cr);
    void friend fight(Creature *crA, Creature *crB);
    bool friend shouldFight(Creature *crA, Creature *crB);
    bool friend operator<(Creature &lhs, Creature &rhs);
};


#endif
