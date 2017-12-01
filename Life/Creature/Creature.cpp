//
//  Creature.cpp
//  Life
//
//  Created by Georgel Vintila on 24/10/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#include "Creature.h"
#include <string.h>
#include "Functions.h"
#include <stdlib.h>
#include <stdio.h>


int seed = 0;

#pragma mark Constructors

Creature:: Creature() {
//    srand(randSeed++);
    objId = ++seed;
    screen = 0;
    setType(0);
    attributes = Attributes();
    age = 0;
    resource->updateFoodResource(-attributes.energy,screen);
    description = new char[descSize];
    
    
}

Creature::Creature(Attributes attrib, int type = 0) {
    screen = -1;
    this->objId = ++seed;
    this->age = 0;
    this->attributes = attrib;
    setType(type);
    description = new char[descSize];

}

#pragma mark - Destructor

Creature::~Creature() {
    delete[] description;
}

#pragma mark - Getters

int Creature::getID() {
    return objId;
}

char* Creature::getDescription() {
    sprintf(description, "[C(%d): {%3.2f} <%d,%d, %d/%d, %d, %d>]",objId,fitness(),attributes.strength,attributes.speed,attributes.lifeForce,attributes.maxForce,attributes.energy,age);
    return description;
}

bool Creature::isDead() {
    return  dead;
}

bool Creature::isAdult() {
    return age >= 2.0*attributes.strength;
}

bool Creature::isHungry() {
    return hungry;
}

int Creature::getStr() {
    return attributes.strength;
}

int Creature::getForce() {
    return attributes.maxForce;
}

int Creature::getType() {
    return type;
}

double Creature::getSpeed() {
    return attributes.speed;
}

NormalCoord Creature::getAbsolutePosition(){
    return  absolutePosition;
}

int Creature::getFoodValue() {
    return  resource->getFoodResource(screen);
}

double Creature::fitness() {
    if(isDead())
        return 0.0;
    int str = attributes.strength;
    int frc = attributes.lifeForce;
    int spd = attributes.speed;
    int nrg = attributes.energy;
    return sqrt(((str * str) + (frc * frc)) * ageCurve(age, 2*(str+frc)));
}

#pragma mark - Setters

void Creature::setType(int type) {
    this->type = type;
    this->genType = type;
    this->resource = resource->resourceOfType((ResourceType)type);
}

void Creature::setPosition(NormalCoord pos) {
    int oldScreen = screen;
    position = pos;
    int xVal = MAX(ceil(pos.first),0);
    int yVal = MAX(ceil(pos.second),0);
    screen = xVal+yVal*2;
    resource->updateSpaceResource(oldScreen, screen);
}

void Creature::setAbsolutePosition(NormalCoord pos) {
    absolutePosition = pos;
}

#pragma mark - Actions

void Creature::multiply(Creature **cr,NormalCoord pos) {
    int nrg = attributes.energy/2;
    Attributes attrib = attributes;
    attrib.energy = nrg;
    *cr = new Creature(attrib,genType);
    (*cr)->attributes.lifeForce = MIN(attributes.lifeForce/2, attributes.maxForce);
    attributes.lifeForce = MIN(attributes.lifeForce/2, attributes.maxForce);
    (*cr)->setPosition(pos);
    mutate(*cr);
//    (*cr)->type = (*cr)->strength / 12;
    attributes.energy -= nrg;
    checkDead();
}

void Creature::live(bool *canMult) {
    feed();
    int freezeVal = MAX(freezeCurve(position,age)-heatCurve(position) - sqrt(attributes.strength),0);
    attributes.lifeForce -= freezeVal;
    age ++;
    checkDead();
    if(!dead)
        (*canMult) = selection(this);
}

void Creature::feed() {
    if(resource->getFoodResource(screen)*resourceCurve(position) <= 0) {
        
        attributes.lifeForce --;
        if(attributes.lifeForce<attributes.maxForce/2){
            hungry = true;
            //            if(resource->getWasteResource()>0)
            //                setType(1-type);
        }
        //        if((*foodSource) <= 0)
        return;
    }
    double fit = fitness()*attributes.speed;
    int value = MAX((int)(sqrt((log(fit)))),1);
    int foodAmount = MIN(value,resource->getFoodResource(screen)*resourceCurve(position));
    resource->updateFoodResource(-foodAmount,screen);
    attributes.energy += foodAmount;
    hungry = false;
    attributes.lifeForce = MIN(attributes.lifeForce + foodAmount, attributes.maxForce);
}

void Creature::checkDead() {
    double fit = fitness();
    if(attributes.strength <= 0 || attributes.lifeForce <= 0 || fit <= 0) {
        //        cout<<getDescription()<<"DEAD!"<<endl;
        die();
    }
}

void Creature::die() {
    if(!dead)
        resource->updateSpaceResource(screen, -1);
    dead = true;
    resource->updateWasteResource(attributes.energy,screen);
    attributes.energy = 0;
   
}

#pragma mark - Friends

bool selection(Creature *cr) {
    if(cr->resource->getSpaceResource(cr->screen)<=0)
        return false;
    int quota = 2.0*cr->attributes.strength/cr->attributes.speed;
    bool canMult = false;
    if(cr->isAdult() && quota && cr->attributes.energy >= (3 * quota) + 1)
        canMult = random(quota) < (cr->attributes.energy - (3 * quota));
    return canMult;
}

void mutate(Creature *cr) {
    cr->attributes.strength += handleMutation(seed);
    cr->attributes.maxForce += handleMutation(seed);
    cr->attributes.speed += handleMutation(seed);
    if(cr->resource->getFoodResource(cr->screen) < 1500 && random(mutationValue) < 10){
        cr->setType(((cr->type+1)%ResourceTypeALL));
    }
    cr->checkDead();
}



void fight(Creature *crA, Creature *crB) {
    double fitA = crA->fitness();
    double fitB = crB->fitness();
    if(fitA > fitB) {
        crA->attributes.lifeForce -= crB->attributes.lifeForce;
//        crA->strength++;
        crB->die();
        crA->feed();
    } else if(fitB > fitA) {
        crB->attributes.lifeForce -= crA->attributes.lifeForce;
//        crB->strength++;
        crA ->die();
        crB->feed();
    } else {
        crA->die();
        crB->die();
    }
}

bool shouldFight(Creature *crA, Creature *crB){
    if(!crB)
        return false;
    bool otherCanFight = crB && !crB->isDead() && crB->age > 2;
    bool currentCanFight =  crA->isHungry() && crA->age > 2;
    
    int typeA = crA->type;
    int typeB = crB->type;
    bool enemy = true;//(typeA == (typeB+1)%ResourceTypeALL) || (typeA == typeB && crA->isHungry()) ;
    
    
    bool fight = otherCanFight && currentCanFight && enemy;
    return fight;
}


bool operator< (Creature& lhs, Creature& rhs) {
    return lhs.fitness() < rhs.fitness();
}

