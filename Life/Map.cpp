//
//  Map.cpp
//  Life
//
//  Created by Georgel Vintila on 19/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//


#include "Map.h"
#include <math.h>
#include <iterator>
#include "Functions.h"

int mapSeed = 0;

#pragma mark Constructors

Map::Map(int width, int height) {
    this->width = width;
    this->height = height;
    
    this->surface = new int*[height];
    for(int i = 0; i< height; i++) {
        this->surface[i] = new int[width];
        memset(surface[i], 0, width * sizeof(int));
    }
    creatureInfo = MapInfo();
    fittest = 1;
    setupResources();
}

#pragma mark - Destructor

Map::~Map() {
    width = 0;
    height = 0;
    for(int i = 0; i< height; i++)
        delete [] surface[i];
    delete [] surface;

}

#pragma mark - Getters

Creature* Map::getItem(int x, int y) {
    int objId = surface[y][x];
    if(objId)
        return creatureInfo.at(objId);
    return NULL;
}

MapSurface Map::getSurface() {
    return surface;
}

MapInfo Map::mapInfo() {
    return  creatureInfo;
}

int Map::getWidth() {
    return width;
}

int Map::getHeight() {
    return height;
}

Creature* Map::getFittest() {
    if(!fittest)
        return NULL;
    return creatureInfo.at(fittest);
}

int Map::valueForCoord(MapCoord coord, MapSurface checkSurface) {
    int count = 0;
    int x = coord.first;
    int y = coord.second;
    for(int i = x - 1; i <= x + 1; i++){
        for (int j = y - 1; j <= y + 1; j++){
            if(i < 0 || i >= width || j < 0 || j >= height){
                count++;
            } else if(surface[j][i] || checkSurface[j][i]) {
                count++;
            }
            
        }
    }
    return count;
}

MapCoord Map::getEmptyNeighbour(int x, int y, MapSurface checkSurface,int str) {
    
    MapCoord move = MapCoord(-1,-1);
    double minValue = 10000;
    
    double sum = 0;
    Creature *current = getItem(x, y);
    bool hadFight = false;
    map<double,MapCoord> pairList = map<double,MapCoord> ();
    int it = 1;//ceil(current->getSpeed());
    for (int j = y - it; j <= y + it; j++){
        for(int i = x - it; i <= x + it; i++){
            if(i < 0 || i >= width || j < 0 || j >= height)
                continue;
            if(!surface[j][i] && !checkSurface[j][i]){
                MapCoord co = MapCoord(i,j);
                
                
                double distance = distanceFromPoint(MapCoord(x-i,y-j))*10;
                
                double yValue = (j/(double)height)*2-1;
                double freezeValue = freezeCurve(MapCoord(0,yValue),(mapSeed/1000)+1);

                double value = ((valueForCoord(co, checkSurface) * freezeValue * distance)+0.000001);
                sum += value;
                pairList.insert({sum,co});
                
//
                if(minValue > value || !random(3)*(minValue == value)) {
                    minValue = value;
                    move = co;
                }
            } else if(current->isHungry() && !hadFight){
                Creature *next = getItem(i, j);

                if(shouldFight(current,next)) {
                    fight(current,next);
                    hadFight = true;
                }
            }
            
        }
    }
    if(pairList.size()<1){
        current->die();
    }
//    if(pairList.size())
//        move = (*pairList.begin()).second;
//    double val = randomDouble(100000)*sum;
//    for(pair<double,MapCoord> pair : pairList){
//        if(pair.first>=val)
//            break;
//        move = pair.second;
//    }
//
    return MapCoord(move);
}

#pragma mark - Setters

void Map::setItem(Creature* item, int x, int y) {
    surface[y][x] = item->getID();
    creatureInfo.insert({item->getID(),item});
    item->setPosition(MapCoord((double) x/width,(double)y/height));
    item->setAbsolutePosition(MapCoord((double) x,(double)y));
}

#pragma mark - Actions

void Map::simulate() {
    MapSurface newSurface = new int*[height];
    for(int i = 0; i< height; i++) {
        newSurface[i] = new int[width];
        memset(newSurface[i], 0, width * sizeof(int));
    }
    MapInfo crInfo = MapInfo();
    mapSeed ++;
    srand((unsigned)time(NULL));
    int newFit = 0;
    double maxFit = 0;
    for(pair<int, Creature *> cInfo : creatureInfo) {
        
        Creature *item = cInfo.second;
        MapCoord absPos = item->getAbsolutePosition();
        int x = absPos.first;
        int y = absPos.second;
        
        bool canMultiply = false;
        Creature *offspring = NULL;
        MapCoord pair = getEmptyNeighbour(x, y, newSurface, item->getStr());
        item->live(&canMultiply);
        if(pair.first == -1) {
            canMultiply = false;
        }
        if(canMultiply){
            NormalCoord coord = NormalCoord((pair.first/(double)width)*2 - 1,(pair.second/(double)height)*2 - 1);
            item->multiply(&offspring,coord);
        }
        if(!item->isDead()){
            crInfo.insert({item->getID(),item});
            MapCoord fullCoord = pair;
            if(canMultiply || pair.first == -1)
                fullCoord = MapCoord(x,y);
            newSurface[fullCoord.second][fullCoord.first] = item->getID();
            NormalCoord coord = NormalCoord((fullCoord.first/(double)width)*2 - 1,(fullCoord.second/(double)height)*2 - 1);
            item->setPosition(coord);
            item->setAbsolutePosition(MapCoord((double) fullCoord.first,(double)fullCoord.second));
            double fit = fitness(item);
            if(fit > maxFit){
                newFit = item->getID();
                maxFit = fit;
            }
        } else {
            surface[y][x] = 0;
            delete item;
        }
        if(canMultiply && offspring != NULL) {
            crInfo.insert({offspring->getID(),offspring});
            newSurface[pair.second][pair.first] = offspring->getID();

            offspring->setAbsolutePosition(MapCoord((double)pair.first,(double)pair.second));
            double fit = fitness(offspring);
            if(fit > maxFit){
                newFit = item->getID();
                maxFit = fit;
            }
        }
        
    }
    fittest = newFit;
    creatureInfo = crInfo;
//        printMap(crInfo);
//
//        printSurface(newSurface);
    
    for(int i = 0; i< height; i++)
        delete[] surface[i];
    delete [] surface;
    
    surface = newSurface;
}

void Map::startSimulation() {
    cancelSimulation = false;
    while (!cancelSimulation) {
        simulate();
    }
}
void Map::stopSimulation() {
    cancelSimulation = true;
}

void Map::printSurface(MapSurface sur) {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            cout<<sur[y][x] <<" ";
        }
        cout<<endl;
    }
}

void Map::printMap() {
    for(pair<int, Creature*> c : creatureInfo) {
        cout<<c.second->getDescription()<<endl;
    }
    cout<<endl;
}

void Map::printMap(MapInfo info) {
    for(pair<int, Creature*> c : info) {
        cout<<c.second->getDescription()<<endl;
    }
    cout<<endl;
}

#pragma mark - Friends

