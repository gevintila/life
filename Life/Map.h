//
//  Map.h
//  Life
//
//  Created by Georgel Vintila on 19/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <iostream>
#include <map>
#include <list>
#include "Creature.h"

using namespace std;

typedef pair<int, int> Coord;
typedef map<int,Creature *> MapInfo;

class Map {
    int width;
    int height;
    int** surface;
    MapInfo creatureInfo;
    int valueForCoord(Coord coord, int **surface, int it);
    Coord getEmptyNeighbour(int x, int y, int **checkSurface,int str);
    bool cancelSimulation;
    int fittest;
public:
    ~Map();
    Map(int width, int height);
    
    Creature * getItem(int x, int y);
    void setItem(Creature* item, int x, int y);
    
    int** getSurface();
    MapInfo mapInfo();
    
    void simulate();
    void startSimulation();
    void stopSimulation();
    
    void printSurface(int **);
    void printMap();
    void printMap(MapInfo info);
    
    int getWidth();
    int getHeight();
    Creature* getFittest();
};


#endif /* Map_h */
