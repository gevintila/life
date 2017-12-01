//
//  Engine.cpp
//  Life
//
//  Created by Georgel Vintila on 24/10/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#include "Engine.h"
#include <math.h>
#include <iterator>
#include "Functions.h"


typedef map<double,MapCoord> CoordInfo;

int mapSeed = 0;

Engine::Engine() {
    map = NULL;
}


Engine::Engine(Map *map) {
    this->map = map;
//    int height = map->getHeight();
//    int width = map->getWidth();

    
    creatureInfo = CreatureInfo();
    FitList fList = FitList();

    fittest = 1;
    setupResources();
}


Engine::~Engine() {
    
    delete map;
}

#pragma mark - Getters

long Engine::populationSize() {
    return creatureInfo.size();
}

MapPair Engine::mapInfo() {
    return MapPair(map->copy(),creatureInfo);
}

Creature* Engine::getItem(int x, int y) {
    int objId = map->surface[y][x];
    if(objId)
        return creatureInfo.at(objId);
    return NULL;
}

Creature* Engine::getFittest() {
    if(!fittest)
        return NULL;
    return creatureInfo.at(fittest);
}

Map* Engine::getMap() {

    return map;
}


MapCoord Engine::getEmptyNeighbour(int x, int y, Map *srcMap) {
    
    MapCoord move = MapCoord(-1,-1);
    double minValue = 10000;
    
    double sum = 0;
    Creature *current = getItem(x, y);
    bool hadFight = false;
    CoordInfo coordInfo = CoordInfo();
    int it = 1;//ceil(current->getSpeed());
    for (int j = y - it; j <= y + it; j++){
        for(int i = x - it; i <= x + it; i++){
            if(i < 0 || i >= srcMap->getWidth() || j < 0 || j >= srcMap->getHeight())
                continue;
            if(!srcMap->surface[j][i]){
                MapCoord co = MapCoord(i,j);
                
                
                double distance = distanceFromPoint(MapCoord(x-i,y-j))*10;
                
                double yValue = (j/(double)srcMap->getHeight())*2-1;
                double freezeValue = freezeCurve(MapCoord(0,yValue),(mapSeed/1000)+1);
                int coordValue = srcMap->valueForCoord(co);
                double value = (coordValue * freezeValue * distance + 0.000001);
                sum += value;
                coordInfo.insert({sum,co});
                
                //
                if(minValue > value || !random(3)*(minValue == value) || !random(100)) {
                    minValue = value;
                    move = co;
                }
            } else if(!hadFight && current->isHungry()){
                Creature *next = getItem(i, j);
                
                if(shouldFight(current,next)) {
                    fight(current,next);
                    hadFight = true;
                }
            }
            
        }
    }
//    if(coordInfo.size()<1 && !random(3)){
//        current->die();
//    }
//        if(coordInfo.size())
//            move = (*coordInfo.begin()).second;
//        double val = randomDouble(100000)*sum;
//        for(pair<double,MapCoord> pair : coordInfo){
//            if(pair.first>=val)
//                break;
//            move = pair.second;
//        }
    
    return MapCoord(move);
}

void Engine::printMap() {
    for(pair<int, Creature*> c : creatureInfo) {
        cout<<c.second->getDescription()<<endl;
    }
    cout<<endl;
}
#pragma mark - Setters

void Engine::setItem(Creature* item, int x, int y) {
    map->surface[y][x] = item->getID();
    creatureInfo.insert({item->getID(),item});
    fitList.push_back(FitKey(item->fitness(),item->getID()));
    item->setPosition(MapCoord((double) x/map->getWidth(),(double)y/map->getHeight()));
    item->setAbsolutePosition(MapCoord((double) x,(double)y));
}

void Engine::printMap(CreatureInfo info) {
    for(pair<int, Creature*> c : info) {
        cout<<c.second->getDescription()<<endl;
    }
    cout<<endl;
}

#pragma mark - Actions

void Engine::simulate() {
    Map *newMap = map->copy();
    
    CreatureInfo crInfo = CreatureInfo();
    FitList fList = FitList();
    mapSeed ++;
    srand((unsigned)time(NULL));
    int newFit = 0;
    double maxFit = 0;
    
    for(FitKey key : fitList) {
        
        Creature *item = creatureInfo[key.second];
        MapCoord absPos = item->getAbsolutePosition();
        int x = absPos.first;
        int y = absPos.second;
        
        bool canMultiply = false;
        Creature *offspring = NULL;
        MapCoord pair = getEmptyNeighbour(x, y, newMap);
        item->live(&canMultiply);
        if(pair.first == -1) {
            canMultiply = false;
        }
        if(canMultiply){
            NormalCoord coord = NormalCoord((pair.first/(double)map->getWidth())*2 - 1,(pair.second/(double)map->getHeight())*2 - 1);
            item->multiply(&offspring,coord);
        }
        newMap->surface[y][x] = 0;
        map->surface[y][x] = 0;
        if(!item->isDead()){
            crInfo.insert({item->getID(),item});
            fList.push_back(FitKey({item->fitness(),item->getID()}));
            MapCoord fullCoord = pair;
            if(canMultiply || pair.first == -1)
                fullCoord = MapCoord(x,y);
            newMap->surface[fullCoord.second][fullCoord.first] = item->getID();
            NormalCoord coord = NormalCoord((fullCoord.first/(double)map->getWidth())*2 - 1,(fullCoord.second/(double)map->getHeight())*2 - 1);
            item->setPosition(coord);
            item->setAbsolutePosition(MapCoord((double) fullCoord.first,(double)fullCoord.second));
            double fit = item->fitness();
            if(fit > maxFit){
                newFit = item->getID();
                maxFit = fit;
            }
        } else {
            
            delete item;
        }
        if(canMultiply && offspring != NULL) {
            crInfo.insert({offspring->getID(),offspring});
            newMap->surface[pair.second][pair.first] = offspring->getID();
            fList.push_back(FitKey({offspring->fitness(),offspring->getID()}));
            offspring->setAbsolutePosition(MapCoord((double)pair.first,(double)pair.second));
            double fit = offspring->fitness();
            if(fit > maxFit){
                newFit = item->getID();
                maxFit = fit;
            }
        }
        
    }
    fittest = newFit;
    creatureInfo = crInfo;
    
    sort(fList.rbegin(), fList.rend());
    fitList = fList;
    //        printMap(crInfo);
    //
    //        printSurface(newSurface);
    
//    for(int i = 0; i< height; i++)
//        delete[] surface[i];
    
    delete map;
    map = newMap;
//
    
}

void Engine::startSimulation() {
    cancelSimulation = false;
    while (!cancelSimulation) {
        simulate();
    }
}
void Engine::stopSimulation() {
    cancelSimulation = true;
}
