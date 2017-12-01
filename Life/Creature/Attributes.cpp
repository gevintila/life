//
//  Attributes.cpp
//  Life
//
//  Created by Georgel Vintila on 30/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#include "Attributes.h"
#include "Constants.h"


Attributes::Attributes() {
    speed = minSpeed;
    strength = minStrength;
    energy = minEnergy;
    lifeForce = minLifeForce;
    maxForce = lifeForce;
}

Attributes::Attributes(Attributes &attrib){
    speed = attrib.speed;
    strength = attrib.strength;
    energy = attrib.energy;
    maxForce = attrib.maxForce;
    lifeForce = attrib.lifeForce;
}

Attributes::Attributes(int strength, int energy, int speed, int force = minLifeForce) {
    this->strength = strength;
    this->speed = speed;
    this->energy = energy;
    this->lifeForce = force;
    this->maxForce = force;
}

