//
//  Map.cpp
//  Life
//
//  Created by Georgel Vintila on 19/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//


#include "Map.h"
#include <math.h>

int mapSeed = 0;

int Map::valueForCoord(Coord coord, int **checkSurface,int it) {
    int count = 0;
    int x = coord.first;
    int y = coord.second;
    for(int i = x - 1; i <= x + 1; i++){
        for (int j = y - 1; j <= y + 1; j++){
            if(i < 0 || i >= width || j < 0 || j >= height){
                count++;
            } else if(surface[j][i] || checkSurface[j][i]) {
                count++;
//                if(it)
//                    count +=valueForCoord(Coord(i,j), checkSurface, it-1);
            }

        }
    }
    return count;
}

double distanceFromPoint(Coord point) {
    return sqrt(pow(point.first, 2) + pow(point.second, 2));
}



Coord Map::getEmptyNeighbour(int x, int y, int **checkSurface,int str) {
    
    Coord move = Coord(-1,-1);
    double minValue = 10000;
    Creature *current = getItem(x, y);
    bool hadFight = false;
    list<Coord> pairList = list<Coord> ();
    for (int j = y - 1; j <= y + 1; j++){
        for(int i = x - 1; i <= x + 1; i++){
            if(i < 0 || i >= width || j < 0 || j >= height)
                continue;
            if(!surface[j][i] && !checkSurface[j][i]){
                Coord co = Coord(i,j);
                
                
                double distance = distanceFromPoint(Coord(x-i,y-j));
                
                double yValue = (j/(double)height)*2-1;
                double freezeValue = freezeCurve(MapCoord(0,yValue),(mapSeed/1000)+1);
//                double heatValue = heatCurve(MapCoord(0,yValue));
                double value = valueForCoord(co, checkSurface,2) * freezeValue * distance;
                if(minValue >= value) {
                    minValue = value;
                    move = co;
                }
            } else if(!hadFight){
                Creature *next = getItem(i, j);
                
                if(next && !next->isDead() && next->isAdult() && current->isAdult() && !foodResources) {
                    fight(current,next);
                    hadFight = true;
                }
            }

        }
    }
    
        
    return Coord(move);
}

Map::~Map() {
    width = 0;
    height = 0;
    for(int i = 0; i< height; i++)
        delete [] surface[i];
    delete [] surface;

}

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
}

Creature* Map::getItem(int x, int y) {
    int objId = surface[y][x];
    if(objId)
        return creatureInfo.at(objId);
    return NULL;
}

void Map::setItem(Creature* item, int x, int y) {
    surface[y][x] = item->getID();
    creatureInfo.insert({item->getID(),item});
    item->setAbsolutePosition(Coord((double) x,(double)y));
}

int** Map::getSurface() {
    return surface;
}

void Map::simulate() {
    int** newSurface = new int*[height];
    for(int i = 0; i< height; i++) {
        newSurface[i] = new int[width];
        memset(newSurface[i], 0, width * sizeof(int));
    }
    MapInfo crInfo = MapInfo();
    mapSeed ++;
    int newFit = 0;
    double maxFit = 0;
    for(pair<int, Creature *> cInfo : creatureInfo) {

        Creature *item = cInfo.second;
        MapCoord absPos = item->getAbsolutePosition();
        int x = absPos.first;
        int y = absPos.second;
        
        bool canMultiply = false;
        Creature *offspring = NULL;
        Coord pair = getEmptyNeighbour(x, y, newSurface, item->getStr());
        item->live(&canMultiply);
        if(pair.first == -1) {
            canMultiply = false;
        }
        if(canMultiply)
            item->multiply(&offspring);
        if(!item->isDead()){
            crInfo.insert({item->getID(),item});
            Coord fullCoord = pair;
            if(canMultiply || pair.first == -1)
                fullCoord = Coord(x,y);
            newSurface[fullCoord.second][fullCoord.first] = item->getID();
            MapCoord coord = MapCoord((fullCoord.first/(double)width)*2 - 1,(fullCoord.second/(double)height)*2 - 1);
            item->setPosition(coord);
            item->setAbsolutePosition(Coord((double) fullCoord.first,(double)fullCoord.second));
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
            
            MapCoord coord = MapCoord((pair.first/(double)width)*2 - 1,(pair.second/(double)height)*2 - 1);
            offspring->setPosition(coord);
            offspring->setAbsolutePosition(Coord((double)pair.first,(double)pair.second));
            double fit = fitness(offspring);
            if(fit > maxFit){
                newFit = item->getID();
                maxFit = fit;
            }
        }
        
    }
   fittest = newFit;
    creatureInfo = crInfo;
//    printMap(crInfo);
    
//    printSurface(newSurface);
    
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

MapInfo Map::mapInfo() {
    return  creatureInfo;
}

void Map::printSurface(int ** sur) {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            cout<<sur[y][x] <<" ";
        }
        cout<<endl;
    }
}

int Map::getWidth() {
    return width;
}

int Map::getHeight() {
    return height;
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

Creature* Map::getFittest() {
    if(!fittest)
        return NULL;
    return creatureInfo.at(fittest);
}
