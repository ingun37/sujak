//
//  testjlinear.m
//  testjlinear
//
//  Created by ingun on 7/12/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "jlinear.hpp"
@interface testjlinear : XCTestCase

@end

@implementation testjlinear

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testExample {
    jlinear::jlineartest(1, -2, 1, 0,
                         0, 2, -8, 8,
                         -4, 5, 9, -9);
    puts("");
    jlinear::jlineartest(2, 1, 3, 1,
                         2, 6, 8, 3,
                         6, 8, 18,5);
    puts("");
    jlinear::jlineartest(3, 1, -6, -10,
                         2, 1, -5, -8,
                         6, -3, 3, 0);
    puts("");
    
    jlinear::jlineartest(1, 0, 1, 1,
                         1, 1, 1, 2,
                         1, -1, 1, 1);
    puts("");
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
