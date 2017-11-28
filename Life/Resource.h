//
//  Resource.h
//  Life
//
//  Created by Georgel Vintila on 26/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#ifndef Resource_h
#define Resource_h

typedef enum : int {
    ResourceTypePlant = 0,
    ResourceTypeHerb,
    ResourceTypeCarn,
    ResourceTypeDec,
    ResourceTypeALL
    
} ResourceType;

char *getResources();
void setupResources();
void updateLight();
ResourceType mostWanted();

class Resource {
    int *foodSource;
    int *wasteSource;
    
    ResourceType type;
    Resource(ResourceType type);
    
public:
    static Resource* resourceOfType(ResourceType type);
    
    int getFoodResource(int screen);
    int getWasteResource(int screen);
    ResourceType getType();

    void updateFoodResource(int value,int screen);
    void updateWasteResource(int value,int screen);
};


#endif /* Resource_h */
