//
//  Resource.cpp
//  Life
//
//  Created by Georgel Vintila on 26/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#include "Resource.h"
#include "Constants.h"

map<ResourceType,Resource *> resMap = map<ResourceType,Resource*>();

int lightResources = 300000;
int plantResources = 0;
int meatResources = 0;
int wasteResources = 0;
int noResources = 0;

char *resDesc;

#pragma mark Static Functions

char* getResources() {
   if(!resDesc)
       resDesc = new char[200];
    sprintf(resDesc,"%d Light\n%d Plant\n%d Meat\n%d Waste",lightResources,plantResources,meatResources,wasteResources);
    return resDesc;
}

ResourceType mostWanted() {
    int max = lightResources;
    ResourceType mw = ResourceTypePlant;
    if(plantResources > max) {
        max = plantResources;
        mw = ResourceTypeHerb;
    }
    if(meatResources > max) {
        max = meatResources;
        mw = ResourceTypeCarn;
    }
    if(wasteResources > max) {
        max = wasteResources;
        mw = ResourceTypeDec;
    }
    return  mw;
}

void updateLight() {
    int pValue = (int)(plantResources * resourceDecayValue);
    plantResources -= pValue;
    
    int mValue = (int)(meatResources * resourceDecayValue);
    meatResources -= mValue;
    meatResources += pValue;
    
    int wValue = (int)(wasteResources * resourceDecayValue);
    wasteResources -= wValue;
    wasteResources += mValue;
    
    int lValue = (int)(lightResources * resourceDecayValue);
    lightResources -= lValue;
    lightResources += wValue;
    
    plantResources += lValue;
}


#pragma mark - Constructors

Resource::Resource(ResourceType type) {
    this->type = type;
    
    switch (type) {
        case ResourceTypePlant: {
            foodSource = &lightResources;
            wasteSource = &plantResources;
            break;
        }
        case ResourceTypeHerb: {
            foodSource = &plantResources;
            wasteSource = &meatResources;
            break;
        }
        case ResourceTypeCarn: {
            foodSource = &meatResources;
            wasteSource = &wasteResources;
            break;
        }
        case ResourceTypeDec: {
            foodSource = &wasteResources;
            wasteSource = &lightResources;
            break;
        }
        default: {
            foodSource = &noResources;
            wasteSource = &noResources;
            break;
        }
    }
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

int Resource::getFoodResource() {
    return (*foodSource);
}

int Resource::getWasteResource() {
    return (*wasteSource);
}

#pragma mark - Setters

void Resource::updateFoodResource(int value) {
    if(value < -10000)
        cout<<value<<endl;
    (*foodSource) += value;
}

void Resource::updateWasteResource(int value) {
    if(value < -10000)
        cout<<value<<endl;
    (*wasteSource) += value;
}

















