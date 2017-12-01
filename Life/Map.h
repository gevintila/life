//
//  Map.h
//  Life
//
//  Created by Georgel Vintila on 19/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include "Constants.h"

class Map {
    int width;
    int height;
    Map();
public:
    MapSurface surface;
    
    ~Map();
    Map(MapCoord size);
    Map(int width, int height);
    Map(MapCoord coord,MapSurface surface);
    
    Map* copy();
   
    int valueForCoord(MapCoord coord);
   
    void printSurface(MapSurface);

    int getWidth();
    int getHeight();
    
};


#endif /* Map_h */
