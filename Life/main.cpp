//
//  main.cpp
//  Life
//
//  Created by Georgel Vintila on 24/10/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#include <iostream>
#include <map>
#include "Creature.h"
#include "Map.h"

#include <stdio.h>
#include <thread>

using namespace std;

char esc = 0;
int year = 0;
pair<int, long> maxPop;
void runLoop() {
    esc = getchar();
}

void printList(map<int,Creature *> info) {
    cout <<endl<<"Resoureces: "<<foodResources<<endl;
    cout <<"Waste: "<<wasteResources<<endl;
//    cout <<"Population: "<<maxPop.second<<" Year: "<<maxPop.first<<endl;
    for(pair<int, Creature*> c : info) {
        cout<<c.second->getDescription()<<endl;
    }
}

map<int,Creature *> simulate(map<int,Creature *> info) {
    if(year % 100 == 0){
        cout<<"Year: "<<year<<endl;
//            foodResources += 1000;
//            wasteResources -= 200;
 
    }
    map<int,Creature *> crInfo = map<int,Creature*>();
    for(pair<int, Creature*> cInfo : info) {
        Creature *c = cInfo.second;
        bool canMultiply = false;
        c->live(&canMultiply);
        Creature *cr = NULL;
        if(canMultiply/*&& (crInfo.size()+info.size()) < 2000*/)
            c->multiply(&cr);
        
        if(!c->isDead())
            crInfo.insert({c->getID(),c});
        else
            delete c;
        
        if(canMultiply && cr != NULL)
            crInfo.insert({cr->getID(),cr});
    }
    year ++;
    long pop = crInfo.size();
    if(maxPop.second < pop)
        maxPop = pair<int, long>(year, pop);
    if(year == 20 || crInfo.size() == 0){
        esc = 1;
        cout<<"DONE!:"<<year;
    }
//    printList(crInfo);
    return crInfo;
}



int main(int argc, const char * argv[]) {
    // insert code here...
    

    cout<<freezeCurve(MapCoord(0,(299/599.0) *2 -1))<<endl;
    cout<<freezeCurve(MapCoord(0,(300/599.0) *2 -1))<<endl;
    
    
//    Creature *c = new Creature();
//    Map crMap = Map(30,30);
//    crMap.setItem(c,14,14);
    
//    crMap.startSimulation();
    
//    thread loop = thread(runLoop);
    
//    while (!esc) {
//        crMap.simulate();
//        printList(crMap.mapInfo());
//        if(!crMap.mapInfo().size())
//            esc = 1;
//    }
//    crMap.stopSimulation();
//    loop.join();
    
    
    return 0;
}
