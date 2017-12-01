//
//  Attributes.h
//  Life
//
//  Created by Georgel Vintila on 30/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#ifndef Attributes_h
#define Attributes_h

#include <stdio.h>

class Attributes {
    
public:
    int strength;
    int speed;
    int lifeForce;
    int energy;
    int maxForce;

    Attributes();
    Attributes(Attributes &attrib);
    Attributes(int strength, int energy, int speed, int force);
};

#endif /* Attributes_h */
