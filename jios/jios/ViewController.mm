//
//  ViewController.m
//  jios
//
//  Created by ingun on 9/21/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//
#import <QuartzCore/QuartzCore.h>
#import "ViewController.hpp"
#import "jmetal.hpp"
@interface ViewController ()

@end

jmetal* gl;

@implementation ViewController

- (void)viewDidLoad {
	[super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
	
	gl->init(self.view.bounds.size.width, self.view.bounds.size.height);
}

-(void)viewWillAppear:(BOOL)animated
{
	gl->render(((CAMetalLayer*)self.view.layer).nextDrawable.texture);
}

- (void)didReceiveMemoryWarning {
	[super didReceiveMemoryWarning];
	// Dispose of any resources that can be recreated.
}
@end
