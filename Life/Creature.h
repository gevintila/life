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

using namespace std;

extern int foodResources;
extern int wasteResources;

typedef pair<double, double> MapCoord;

typedef enum  {
    EatingSourceFood,
    EatingSourceWaste,
} EatingSource;

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
    int maxForce;
    char *description;
    int type;
    
    int *foodSource;
    int *wasteSource;
    
    void checkDead();
    void die();
    EatingSource source;
    MapCoord position;
    MapCoord absolutePosition;
    
public:
    Creature();
    Creature(int strength, int energy, int force);
    ~Creature();
    char* getDescription();
    int getID();
    bool isDead();
    bool isAdult();
    void feed();
    void live(bool *canMult);
    
    int getStr();
    int getForce();
    int getType();
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
