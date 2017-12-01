//
//  Curves.cpp
//  Life
//
//  Created by Georgel Vintila on 26/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#include "Functions.h"

double ageCurve(int age, double str) {
    return (-pow(age,2) + (str-2)*age + str*2);
}

double freezeCurve(NormalCoord position,int gen){
    double lat = position.second;
    
    double frz = ((gen*pow(lat,6)-4*pow(lat, 2)+0.2)) * freezeValue;
    return frz;
}
double heatCurve(NormalCoord position){
    double lat = position.second;
    //    double frz = (-pow(lat,4) + 0.5) * heatValue;
    return 0;
}

double sign(double a){
    if(a){
        return a/abs(a);
    }
    return 1;
}

double resourceCurve(NormalCoord pos) {
    NormalCoord center = NormalCoord(sign(pos.first)*0.5,sign(pos.second)*0.5);
    return 1/sqrt(pow(center.first, 2) + pow(center.second, 2));
}

int random(int value) {
    return rand() % value;
}

double randomDouble(int value){
    return rand() % value / (double)value;
}

double distanceFromPoint(MapCoord point) {
    
    return sqrt(pow(point.first, 2) + pow(point.second, 2));
}

int handleMutation(int value) {
    double strMutation = randomDouble(mutationValue);
    int strValue = 0;
    if(strMutation <= mutationChance){
        strValue = (random(2 * mutationThreshold)) - mutationThreshold;
        
    }
    return  strValue;
}
