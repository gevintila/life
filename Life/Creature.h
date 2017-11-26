//
//  Engine.hpp
//  Life
//
//  Created by Georgel Vintila on 24/10/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#ifndef Creature_h
#define Creature_h

#define MAX(X,Y) (X) > (Y) ? (X) : (Y)
#define MIN(X,Y) (X) < (Y) ? (X) : (Y)

#include <iostream>
#include "Resource.h"

using namespace std;


typedef pair<double, double> MapCoord;


double freezeCurve(MapCoord position,int gen);
double heatCurve(MapCoord position);

class Creature {

private:
    int objId = 0;
    int strength;
    int lifeForce;
    int energy;
    int age;
    bool dead = false;
    bool hungry = false;
    int maxForce;
    char *description;
    int type;
    
    void checkDead();
    void die();
    void setType(int type);
    Resource *resource;
    MapCoord position;
    MapCoord absolutePosition;
    
public:
    Creature();
    Creature(int strength, int energy, int force, int type);
    ~Creature();
    char* getDescription();
    int getID();
    bool isDead();
    bool isAdult();
    bool isHungry();
    void feed();
    void live(bool *canMult);
    
    int getStr();
    int getForce();
    int getType();
    int getFoodValue();

    MapCoord getAbsolutePosition();
    
    void setPosition(MapCoord pos);
    void setAbsolutePosition(MapCoord pos);
    
    void multiply(Creature **cr);
    void friend mutate(Creature *cr);
    void friend crossover(Creature *crA, Creature *crB);
    double friend fitness(Creature *cr);
    bool friend selection(Creature *cr);
    void friend fight(Creature *crA, Creature *crB);
    
};


#endif
