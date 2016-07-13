//
//  testjlinear.m
//  testjlinear
//
//  Created by ingun on 7/12/16.
//  Copyright © 2016 Apple Inc. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "jlinear.hpp"
using namespace simd;
void testPlane(float p1x, float p1y, float p1z,
               float p2x, float p2y, float p2z,
               float p3x, float p3y, float p3z,
               float vx, float vy, float vz)
{
    float3 p1{p1x, p1y, p1z};
    float3 p2{p2x, p2y, p2z};
    float3 p3{p3x, p3y, p3z};
    matrix_float4x4 K;
    try
    {
        K = jlinear::makePlaneDistanceK(p1, p2, p3);
    }
    catch(const char* msg)
    {
        NSLog(@"%s", msg);
    }
    
    
    
    float4 v4{vx, vy, vz, 1};
    
    float dsqr = vector_dot( v4, (K * v4));
    NSLog(@"dist : %.2f", dsqr);
}
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

    testPlane(10, 0, 10,
              33, 0, 345,
              -566, 0, -3454,
              0, 10, 0);
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
