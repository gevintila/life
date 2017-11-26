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
    setType(objId - 1);
    speed = minSpeed;
    strength = minStrength;
    energy = minEnergy;
    lifeForce = minLifeForce;
    age = 0;
    resource->updateFoodResource(-energy);
    description = new char[descSize];
    maxForce = lifeForce;
    
}

Creature::Creature(int strength, int energy, int speed, int force = minLifeForce, int type = 0) {
    this->objId = ++seed;
    this->strength = strength;
    this->speed = speed;
    this->energy = energy;
    this->lifeForce = force;
    this->age = 0;
    this->maxForce = force;
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
    sprintf(description, "[C(%d): {%3.2f} <%d,%d, %d/%d, %d, %d>]",objId,fitness(this),strength,speed,lifeForce,maxForce,energy,age);
    return description;
}

bool Creature::isDead() {
    return  dead;
}

bool Creature::isAdult() {
    return age >= strength/(2.0*speed);
}

bool Creature::isHungry() {
    return hungry;
}

int Creature::getStr() {
    return strength;
}

int Creature::getForce() {
    return maxForce;
}

int Creature::getType() {
    return type;
}

double Creature::getSpeed() {
    return speed;
}

NormalCoord Creature::getAbsolutePosition(){
    return  absolutePosition;
}

int Creature::getFoodValue() {
    return  resource->getFoodResource();
}

#pragma mark - Setters

void Creature::setType(int type) {
    this->type = type;
    this->genType = type;
    this->resource = resource->resourceOfType((ResourceType)type);
}

void Creature::setPosition(NormalCoord pos) {
    position = pos;
}

void Creature::setAbsolutePosition(NormalCoord pos) {
    absolutePosition = pos;
}

#pragma mark - Actions

void Creature::multiply(Creature **cr) {
    int nrg = energy/2;
    *cr = new Creature(strength, nrg,speed, maxForce,genType);
    (*cr)->lifeForce = MIN(lifeForce/2, maxForce);
    
//    (*cr)->resource = resource;
    mutate(*cr);
//    (*cr)->type = (*cr)->strength / 12;
    energy -= nrg;
    checkDead();
}

void Creature::live(bool *canMult) {
    feed();
    int freezeVal = MAX(freezeCurve(position,age)-heatCurve(position) - sqrt(strength),0);
    lifeForce -= freezeVal;
    age ++;
    checkDead();
    if(!dead)
        (*canMult) = selection(this);
}

void Creature::feed() {
    if(resource->getFoodResource() <= 0) {
        
        lifeForce --;
        if(lifeForce<maxForce/2){
            hungry = true;
            //            if(resource->getWasteResource()>0)
            //                setType(1-type);
        }
        //        if((*foodSource) <= 0)
        return;
    }
    double fit = fitness(this)/speed;
    int value = MAX((int)(sqrt(log(fit))),1);
    int foodAmount = MIN(value,resource->getFoodResource());
    resource->updateFoodResource(-foodAmount);
    energy += foodAmount;
    hungry = false;
    lifeForce = MIN(lifeForce + foodAmount, maxForce);
}

void Creature::checkDead() {
    double fit = fitness(this);
    if(strength <= 0 || lifeForce <= 0 || fit <= 0) {
        //        cout<<getDescription()<<"DEAD!"<<endl;
        die();
    }
}

void Creature::die() {
    dead = true;
    resource->updateWasteResource(energy);
    energy = 0;
}

#pragma mark - Friends

bool selection(Creature *cr) {
    int quota = cr->strength/2.0;
    bool canMult = false;
    if(cr->isAdult() && quota && cr->energy >= (3 * quota) + 1)
        canMult = random(quota) < (cr->energy - (3 * quota));
    return canMult;
}

void mutate(Creature *cr) {
    cr->strength += handleMutation(seed);
    cr->maxForce += handleMutation(seed);
    cr->speed += handleMutation(seed);
    if(cr->resource->getFoodResource() < 1500 && random(mutationValue) < 10){
        cr->setType(mostWanted());
    }
    cr->checkDead();
}

double fitness(Creature *cr) {
    if(cr->isDead())
        return 0.0;
    int str = cr->strength;
    int nrg = cr->lifeForce;
    int spd = cr->speed;
    return sqrt(((str * str) + (nrg * nrg) + (spd * spd)) * ageCurve(cr->age, 2*(str+nrg)));
}

void fight(Creature *crA, Creature *crB) {
    double fitA = fitness(crA);
    double fitB = fitness(crB);
    if(fitA > fitB) {
        crA->lifeForce -= crB->lifeForce;
//        crA->strength++;
        crB->die();
        crA->feed();
    } else if(fitB > fitA) {
        crB->lifeForce -= crA->lifeForce;
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
    bool otherCanFight = crB && !crB->isDead() && crB->isAdult();
    bool currentCanFight = crA->isAdult() && crA->isHungry();
    
    int typeA = crA->type;
    int typeB = crB->type;
    bool enemy = (typeA == (typeB+1)%ResourceTypeALL) || (typeA == typeB && crA->isHungry()) ;
    
    
    bool fight = otherCanFight && currentCanFight && enemy;
    return fight;
}




