//
//  Engine.h
//  Life
//
//  Created by Georgel Vintila on 01/12/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#ifndef Engine_h
#define Engine_h

#include "Map.h"
#include "Creature.h"
#include <vector>

typedef pair<double,int> FitKey;
typedef vector<FitKey> FitList;
typedef map<int,Creature *> CreatureInfo;
typedef pair<Map*,CreatureInfo> MapPair;

class Engine {
    Map *map;
    bool cancelSimulation;
    int fittest;
    CreatureInfo creatureInfo;
    FitList fitList;
   
    MapCoord getEmptyNeighbour(int x, int y, Map *map);
public:
    
    Engine();
    Engine(Map *map);
    ~Engine();
    
    Creature * getItem(int x, int y);
    void setItem(Creature* item, int x, int y);
    
    void simulate();
    void startSimulation();
    void stopSimulation();
    Map* getMap();
    MapPair mapInfo();
    long populationSize();
    void printMap();
    void printMap(CreatureInfo info);
    Creature* getFittest();
};


#endif /* Engine_h */
