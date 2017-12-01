//
//  ViewController.m
//  LifeUI
//
//  Created by Georgel Vintila on 19/11/2017.
//  Copyright © 2017 Georgel Vintila. All rights reserved.
//

#import "ViewController.h"
#import <Quartz/Quartz.h>
#include "Life.h"


static const int RED = 3;
static const int GREEN = 2;
static const int BLUE = 1;
static const int ALPHA = 0;

static void mark(uint8_t *pixel,uint8_t str, uint8_t frc, uint8_t val)
{
    pixel[RED] = str;
    pixel[BLUE] = frc;
    pixel[GREEN] = val;
    pixel[ALPHA] = 255;
}

@interface ViewController()
{
    Engine *engine;
    
}
@property (nonatomic,weak) IBOutlet NSImageView *imageView;
@property (nonatomic,weak) IBOutlet NSTextField *popLabel;
@property (nonatomic,weak) IBOutlet NSTextField *fitLabel;
@property (nonatomic,weak) IBOutlet NSTextField *resourcesLabel;
@property (atomic,assign) BOOL stopSim;

@end

@implementation ViewController



- (void)viewDidLoad {
    [super viewDidLoad];
    MapCoord size = MapCoord(self.imageView.frame.size.width,self.imageView.frame.size.height);
    Map *map = new Map(size);
    engine = new Engine(map);
    
    self.stopSim = YES;
}

-(IBAction)simulate:(NSButton *)sender {
    
    if(!engine->mapInfo().second.size())
    {
        for(int i=0;i<1;i++){
            Creature *creature = new Creature();
            engine->setItem(creature, (i+1)*350, (i+1)*250);
        }
        self.stopSim = NO;
    }
    __block NSImage *image;
    __block bool lock = true;
//    dispatch_queue_t que = dispatch_queue_create("que", NULL);
    dispatch_async(dispatch_queue_create("queue", NULL), ^{
        while (engine->populationSize() && !self.stopSim) {
            engine->simulate();
            usleep(20000);
//            dispatch_async(dispatch_queue_create("que", NULL), ^{
//                usleep(40000);
//                lock = true;
//            });
//            if(lock){
                image = [self generateImageFromMap:engine];
//                lock = false;
//            }
            dispatch_sync(dispatch_get_main_queue(), ^{
                if(image)
                    self.imageView.image = image;
                Creature *c = engine->getFittest();
                if(!c) {
                    self.stopSim = YES;
                    [self.resourcesLabel setStringValue:[NSString stringWithFormat:@"%s",getResources()]];
                    return;
                }
                [self.fitLabel setStringValue:[NSString stringWithFormat:@"FIT: %s",c->getDescription()]];
                [self.popLabel setStringValue:[NSString stringWithFormat:@"POP: %d",(int)engine->populationSize()]];
                [self.resourcesLabel setStringValue:[NSString stringWithFormat:@"%s",getResources()]];
                updateLight();
            });

        }
        self.stopSim = NO;
    });
}
-(IBAction)stopSimulate:(NSButton *)sender{
    self.stopSim = YES;
}


-(NSImage *)generateImageFromMap:(Engine*) engine {
    NSImage *img = nil;
    
    MapPair mapPair = engine->mapInfo();
    Map *map = mapPair.first;
    CGSize size = CGSizeMake(map->getWidth(), map->getHeight());
    
    uint32_t *pixels = (uint32_t *)malloc(size.width * size.height * sizeof(uint32_t));
    
    memset(pixels, 0, size.width * size.height * sizeof(uint32_t));

    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    
    CGContextRef context = CGBitmapContextCreate(pixels, size.width, size.height, 8, size.width * sizeof(uint32_t), colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedLast);
    
    
    CreatureInfo crInfo = mapPair.second;
    int** ptr = map->surface;
    for(int i = 0; i < size.width * size.height; i++) {
        uint8_t *rgbaPixel = (uint8_t*)&pixels[i];
        int y = i/(int)size.width;
        int x = i%(int)size.width;
        int val = ptr[y][x];
        if(val){
            Creature *cr = crInfo.at(val);
//            if(cr->getStr() > cr->getForce()) {
                mark(rgbaPixel, 255 * (cr->getType()&1), 255 * (cr->getType()&2), (cr->getType() &4) * 230 );
//            }
//            else{
//                mark(rgbaPixel, 0, 255 * cr->getType(), 0);
//            }
        }
        else
            mark(rgbaPixel,255,255,255);
        
   }
    delete map;
    CGImageRef image = CGBitmapContextCreateImage(context);
    
    
    
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
    free(pixels);
    
    img = [[NSImage alloc] initWithCGImage:image size:size];
    CGImageRelease(image);
    
    return img;
}


@end
