//
//  Curves.h
//  Life
//
//  Created by Georgel Vintila on 26/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#ifndef Curves_h
#define Curves_h

#include "Constants.h"
#include <math.h>

double freezeCurve(NormalCoord position,int gen);
double heatCurve(NormalCoord position);
double ageCurve(int age, double str);

int random(int);
double randomDouble(int);

double distanceFromPoint(MapCoord point);
int handleMutation(int value);

#endif /* Curves_h */
