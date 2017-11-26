//
//  Map.h
//  Life
//
//  Created by Georgel Vintila on 19/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include "Creature.h"

typedef map<int,Creature *> MapInfo;

class Map {
    int width;
    int height;
    bool cancelSimulation;
    int fittest;
    MapSurface surface;
    MapInfo creatureInfo;
    
    int valueForCoord(MapCoord coord, MapSurface surface);
    MapCoord getEmptyNeighbour(int x, int y, MapSurface checkSurface,int str);
    
public:
    ~Map();
    Map(int width, int height);
    
    Creature * getItem(int x, int y);
    void setItem(Creature* item, int x, int y);
    
    MapSurface getSurface();
    MapInfo mapInfo();
    
    void simulate();
    void startSimulation();
    void stopSimulation();
    
    void printSurface(MapSurface);
    void printMap();
    void printMap(MapInfo info);
    
    int getWidth();
    int getHeight();
    Creature* getFittest();
};


#endif /* Map_h */
