//
//  Creature.cpp
//  Life
//
//  Created by Georgel Vintila on 24/10/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#include "Creature.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>



int randSeed = 5457;
int seed = 0;
const int minStrength = 10;
const int minEnergy = 10;
const float mutationChance = 0.0001;
const int mutationThreshold = 2;
const int minLifeForce = 10;
const int mutationValue = 1000000;
const int descSize = 100;

const int freezeValue = 10;
const int heatValue = 10;



int handleMutation(int value) {
    double strMutation = (rand() % mutationValue) / (double)mutationValue;
    int strValue = 0;
    if(strMutation <= mutationChance){
        strValue = (rand() % (2 * mutationThreshold)) - mutationThreshold;
        
    }
    return  strValue;
}

double ageCurve(int age, double str) {
    return (-pow(age,2) + (str-2)*age + str*2);
}

double freezeCurve(MapCoord position,int gen){
    double lat = position.second;

    double frz = ((gen*pow(lat,6)-4*pow(lat, 2)+0.2)) * freezeValue;
    return frz;
}
double heatCurve(MapCoord position){
    double lat = position.second;
//    double frz = (-pow(lat,4) + 0.5) * heatValue;
    return 0;
}

void Creature::setType(int type) {
    this->type = type;
    this->resource = resource->resourceOfType((ResourceType)type);
    
}

Creature:: Creature() {
//    srand(randSeed++);
    objId = ++seed;
    setType(objId - 1);
    
    strength = minStrength;
    energy = minEnergy;
    lifeForce = minLifeForce;
    age = 0;
    resource->updateFoodResource(-energy);
    description = new char[descSize];
    maxForce = lifeForce;
    
}

Creature::Creature(int strength, int energy, int force = minLifeForce, int type = 0) {
    this->objId = ++seed;
    this->strength = strength;
    this->energy = energy;
    this->lifeForce = force;
    this->age = 0;
    this->maxForce = force;
    setType(type);
    description = new char[descSize];

}

Creature::~Creature() {
    delete[] description;
}

int Creature::getID() {
    return objId;
}
char* Creature::getDescription() {
    sprintf(description, "[C(%d): {%3.2f} <%d, %d/%d, %d, %d>]",objId,fitness(this),strength,lifeForce,maxForce,energy,age);
    return description;
}

bool Creature::isDead() {
    return  dead;
}

void Creature::multiply(Creature **cr) {
    int nrg = energy/2;
    *cr = new Creature(strength, nrg, maxForce,type);
    (*cr)->lifeForce = MIN(lifeForce/2, maxForce);
    
    (*cr)->resource = resource;
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

bool selection(Creature *cr) {
    int quota = cr->strength/2.0;
    bool canMult = false;
    if(cr->isAdult() && quota && cr->energy >= (3 * quota) + 1)
        canMult = (rand() % quota) < (cr->energy - (3 * quota));
    return canMult;
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
    double fit = fitness(this);
    int value = MAX((int)(sqrt(log(fit))),1);
    int foodAmount = MIN(value,resource->getFoodResource());
    resource->updateFoodResource(-foodAmount);
    energy += foodAmount;
    hungry = false;
    lifeForce = MIN(lifeForce + foodAmount, maxForce);
}

void mutate(Creature *cr) {
    cr->strength += handleMutation(seed);
    cr->maxForce += handleMutation(seed);
    cr->checkDead();
}



void Creature::checkDead() {
    double fit = fitness(this);
    if(strength <= 0 || lifeForce <= 0 || fit <= 0) {
//        cout<<getDescription()<<"DEAD!"<<endl;
        die();
    }
}

bool Creature::isAdult() {
    return age >= strength/3.0;
}

bool Creature::isHungry() {
    return hungry;
}

void Creature::die() {
    dead = true;
    resource->updateWasteResource(energy);
    energy = 0;
}

double fitness(Creature *cr) {
    if(cr->isDead())
        return 0.0;
    int str = cr->strength;
    int nrg = cr->maxForce;
    return sqrt(((str * str) + (nrg * nrg)) * ageCurve(cr->age, 2*(str+nrg)));
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
void Creature::setPosition(MapCoord pos) {
    position = pos;
}

void Creature::setAbsolutePosition(MapCoord pos) {
    absolutePosition = pos;
}

void fight(Creature *crA, Creature *crB) {
    double fitA = fitness(crA);
    double fitB = fitness(crB);
    if(fitA > fitB) {
        if(crA->type == crB->type)
            crA->setType(1-crA->type);
        crA->strength ++;
        crB->die();
        crA->feed();
        
    } else if(fitB > fitA) {
        if(crA->type == crB->type)
            crB->setType(1-crB->type);
        crB->strength ++;
        crA ->die();
        crB->feed();
        
    } else {
        crA->die();
        crB->die();
    }
}

MapCoord Creature::getAbsolutePosition(){
    return  absolutePosition;
}

int Creature::getFoodValue() {
    return  resource->getFoodResource();
}
