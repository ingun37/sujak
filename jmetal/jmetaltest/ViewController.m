//
//  ViewController.m
//  jmetaltest
//
//  Created by ingun on 10/5/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import "ViewController.h"
#import "testview.hpp"
@interface ViewController ()

@end

@implementation ViewController
{
	CFTimeInterval _timeSinceLastDrawPreviousTime;
	CADisplayLink *_displayLink;
}
- (void)gameloop
{
	[(testview*)self.view update];
	[(testview*)self.view display];
}
- (void)dispatchGameLoop
{
	// create a game loop timer using a display link
	_displayLink = [[UIScreen mainScreen] displayLinkWithTarget:self
													   selector:@selector(gameloop)];
	_displayLink.preferredFramesPerSecond = 60;
	[_displayLink addToRunLoop:[NSRunLoop mainRunLoop]
					   forMode:NSDefaultRunLoopMode];
}


- (void)viewDidLoad {
	[super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
	
}

-(void)viewWillAppear:(BOOL)animated
{
	
	[self dispatchGameLoop];
}

- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning];
	// Dispose of any resources that can be recreated.
}

@end
