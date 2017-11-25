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

int foodResources = 250000;
int wasteResources = 100;//1000000;

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

Creature:: Creature() {
//    srand(randSeed++);
    objId = ++seed;
    
//    if(objId % 2){
//        source = EatingSourceWaste;
//        foodSource = &wasteResources;
//        wasteSource = &foodResources;
//    }
//    else {
        source = EatingSourceFood;
        foodSource = &foodResources;
        wasteSource = &foodResources;
//    }
    strength = minStrength;
    energy = minEnergy;
    lifeForce = minLifeForce;
    age = 0;
    (*foodSource) -= energy;
    description = new char[descSize];
    maxForce = lifeForce;
    type = objId - 1;
}

Creature::Creature(int strength, int energy, int force = minLifeForce) {
    this->objId = ++seed;
    this->strength = strength;
    this->energy = energy;
    this->lifeForce = force;
    this->age = 0;
    this->maxForce = force;
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
    *cr = new Creature(strength, nrg, maxForce);
    (*cr)->lifeForce = MIN(lifeForce/2, maxForce);
    
    (*cr)->foodSource = foodSource;
    (*cr)->wasteSource = wasteSource;
    (*cr)->source = source;
    mutate(*cr);
    (*cr)->type = (*cr)->strength / 12;
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
    if((*foodSource) <= 0) {
        lifeForce --;
        return;
    }
    int foodAmount = MIN((int)(sqrt(log(fitness(this)))),(*foodSource));
    (*foodSource) -= foodAmount;
    energy += foodAmount;
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

void Creature::die() {
    dead = true;
    (*wasteSource) += energy;
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
        crA->strength ++;
        crB->die();
        crA->feed();
        
    } else if(fitB > fitA) {
        crB->strength ++;
        crA ->die();
        crB->feed();
        
    } else {
        crA ->die();
        crB->die();
    }
}

MapCoord Creature::getAbsolutePosition(){
    return  absolutePosition;
}

