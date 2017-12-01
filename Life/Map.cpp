//
//  Map.cpp
//  Life
//
//  Created by Georgel Vintila on 19/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//


#include "Map.h"




#pragma mark Constructors

Map::Map(){
    width = 0;
    height = 0;
    surface = NULL;
}

Map::Map(int width, int height) {
    this->width = width;
    this->height = height;
    surface = new int*[height];
    for(int i = 0; i< height; i++) {
        surface[i] = new int[width];
        memset(surface[i], 0, width * sizeof(int));
    }
    
}

Map::Map(MapCoord coord) {
    width = coord.first;
    height = coord.second;
    surface = new int*[height];
    for(int i = 0; i< height; i++) {
        surface[i] = new int[width];
        memset(surface[i], 0, width * sizeof(int));
    }
}


Map::Map(MapCoord coord,MapSurface surface) {
    width = coord.first;
    height = coord.second;
    this->surface = surface;
}

#pragma mark - Destructor

Map::~Map() {
    
    for(int i = 0; i< height; i++)
        delete [] surface[i];
    delete [] surface;
    width = 0;
    height = 0;
}

#pragma mark - Getters

Map* Map::copy() {
    Map *map = new Map();
    map->height = height;
    map->width = width;
    map->surface = new int*[height];
    for(int i = 0; i< height; i++) {
        map->surface[i] = new int[width];
        for(int j = 0; j< width; j++)
            map->surface[i][j] = surface[i][j];
    }
    return map;
}

int Map::getWidth() {
    return width;
}

int Map::getHeight() {
    return height;
}

int Map::valueForCoord(MapCoord coord) {
    int count = 0;
    int x = coord.first;
    int y = coord.second;
    for(int i = x - 1; i <= x + 1; i++){
        for (int j = y - 1; j <= y + 1; j++){
            if(i < 0 || i >= width || j < 0 || j >= height){
                count++;
            } else if(surface[j][i]) {
                count++;
            }
        }
    }
    NormalCoord c = NormalCoord((x/(double)width)*2 - 1,(y/(double)height)*2 - 1);
    return count;// resourceCurve(c);
}

#pragma mark - Setters


void Map::printSurface(MapSurface sur) {
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            cout<<sur[y][x] <<" ";
        }
        cout<<endl;
    }
}





#pragma mark - Friends

