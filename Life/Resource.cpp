//
//  Resource.cpp
//  Life
//
//  Created by Georgel Vintila on 26/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#include "Resource.h"
#include <iostream>
#include <map>
using namespace std;

map<ResourceType,Resource *> resMap = map<ResourceType,Resource*>();

int foodResources = 250000;
int wasteResources = 250000;
int noResources = 0;


#pragma mark - Static Functions

int getFoodValue() {
    return foodResources;
}

int getWasteValue() {
    return wasteResources;
}

#pragma mark - Constructors

Resource::Resource(ResourceType type) {
    this->type = type;
    
    switch (type) {
        case ResourceTypeFood: {
            foodSource = &foodResources;
            wasteSource = &wasteResources;
            break;
        }
        case ResourceTypeWaste: {
            foodSource = &wasteResources;
            wasteSource = &foodResources;
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

















