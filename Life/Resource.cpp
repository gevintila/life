//
//  Resource.cpp
//  Life
//
//  Created by Georgel Vintila on 26/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#include "Resource.h"
#include "Constants.h"


typedef map<ResourceType,int *> SourceInfo;
map<ResourceType,Resource *> resMap = map<ResourceType,Resource*>();
SourceInfo foodSourceInfo = SourceInfo();
SourceInfo wasteSourceInfo = SourceInfo();

int lightResources[16] = {80000,80000,80000,80000,0,0,0,0,0,0,0,0,0,0,0,0};

int noResources = 0;

char *resDesc;

#pragma mark Static Functions

void setupResources() {
    
    foodSourceInfo.insert({ResourceTypePlant,&lightResources[0]});
    wasteSourceInfo.insert({ResourceTypeDec,&lightResources[0]});
    
    for(int res = 1; res < ResourceTypeALL; res++) {

        foodSourceInfo.insert({(ResourceType)res,&lightResources[res*4]});
        wasteSourceInfo.insert({(ResourceType)(res-1),&lightResources[res*4]});
    }
}

char* getResources() {
   if(!resDesc)
       resDesc = new char[200];
    sprintf(resDesc,"%d Light\n%d Plant\n%d Meat\n%d Waste",lightResources[0],lightResources[4],lightResources[8],lightResources[12]);
    
//    for(int i=0;i <16;i++) {
//        cout<<lightResources[i]<<" ";
//    }
//    cout<<endl;
    return resDesc;
}

ResourceType mostWanted() {
    int max = 0;
    ResourceType mw = ResourceTypePlant;
    
    for(pair<ResourceType,int *> pair : foodSourceInfo) {
        if((*pair.second) > max) {
            max = *pair.second;
            mw = pair.first;
        }
    }
    return  mw;
}

void updateLight() {
    for(int scr = 0; scr < 4; scr ++) {
        pair<ResourceType,int *> lastPair = (*foodSourceInfo.rbegin());
        int transValue = (int)((*(lastPair.second+scr)) * resourceDecayValue);
        for(pair<ResourceType,int *> pair : foodSourceInfo) {
            int *res = pair.second+scr;
            int value = (int)((*res) * resourceDecayValue);
            *res += transValue - value;
            transValue = value;
        }
    }
}

#pragma mark - Constructors

Resource::Resource(ResourceType type) {
    this->type = type;
    
    foodSource = foodSourceInfo.at(type);
    wasteSource = wasteSourceInfo.at(type);
    
}

Resource* Resource::resourceOfType(ResourceType type) {
    Resource *res;
    if(resMap.find(type) != resMap.end()) {
        res = resMap.at(type);
    } else {
        res = new Resource(type);
        resMap.insert({type, res});
    }
    return res;
}

#pragma mark - Getters

ResourceType Resource::getType() {
    return type;
}

int Resource::getFoodResource(int screen) {
    return *(foodSource + screen);
}

int Resource::getWasteResource(int screen) {
    return *(wasteSource + screen);
}

#pragma mark - Setters

void Resource::updateFoodResource(int value,int screen) {
//    if(screen != 0)
//        cout<<value<<endl;
    int * val = (foodSource + screen);
    *val += value;
}

void Resource::updateWasteResource(int value,int screen) {
//    if(screen != 0)
//        cout<<value<<endl;
    int * val = (wasteSource + screen);
    *val += value;
}

















