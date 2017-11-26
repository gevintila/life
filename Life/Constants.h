//
//  Constants.h
//  Life
//
//  Created by Georgel Vintila on 26/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#ifndef Constants_h
#define Constants_h

#define MAX(X,Y) (X) > (Y) ? (X) : (Y)
#define MIN(X,Y) (X) < (Y) ? (X) : (Y)

#include <map>
#include <list>
#include <iostream>

using namespace std;

const int minStrength = 10;
const int minEnergy = 10;
const float mutationChance = 0.0001;
const int mutationThreshold = 2;
const int minLifeForce = 10;
const int minSpeed = 10;
const int mutationValue = 1000000;
const int descSize = 100;

const int freezeValue = 10;
const int heatValue = 10;

const double resourceDecayValue = 0.01;

typedef pair<double, double> NormalCoord;
typedef pair<int, int> MapCoord;

typedef int** MapSurface;


#endif /* Constants_h */
