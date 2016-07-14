#import <XCTest/XCTest.h>
#import <math.h>
#include "jlinear.hpp"

using namespace simd;

void testEchelon(float m11, float m12, float m13, float m14,
                 float m21, float m22, float m23, float m24,
                 float m31, float m32, float m33, float m34,
                 float r11, float r12, float r13, float r14,
                 float r21, float r22, float r23, float r24,
                 float r31, float r32, float r33, float r34)
{
    float r[3][4] = {
        {r11, r12, r13, r14},
        {r21, r22, r23, r24},
        {r31, r32, r33, r34},
    };
    float m[3][4];
    jlinear::getReducedEchelonMatrix34(m, float4{m11, m12, m13, m14}, float4{m21, m22, m23, m24}, float4{m31, m32, m33, m34});
    
    for(int i=0;i<3;i++)
    {
        
        for(int j=0;j<4;j++)
        {
            float diff = m[i][j] - r[i][j];
            if(::fabs(diff) > 0.0001f)
                throw "incorrect echelon";
        }
    }
}
void testPlane(float p1x, float p1y, float p1z,
               float p2x, float p2y, float p2z,
               float p3x, float p3y, float p3z,
               float vx, float vy, float vz, float dist)
{
    float3 p1{p1x, p1y, p1z};
    float3 p2{p2x, p2y, p2z};
    float3 p3{p3x, p3y, p3z};
    matrix_float4x4 K;
    
    K = jlinear::makePlaneDistanceK(p1, p2, p3);
    
    
    
    
    float4 v4{vx, vy, vz, 1};
    
    float dsqr = vector_dot( v4, (K * v4));
    
    if(::fabs(dsqr - (dist*dist)) > 0.0001)
        throw "incorrect";
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
    try
    {
        testEchelon(1, -2, 1, 0,
                    0, 2, -8, 8,
                    -4, 5, 9, -9,
                    1, 0, 0, 29,
                    0, 1, 0, 16,
                    0, 0, 1, 3);
        puts("");
        testEchelon(2, 1, 3, 1,
                    2, 6, 8, 3,
                    6, 8, 18,5,
                    1, 0, 0, 0.3,
                    0, 1, 0, 0.4,
                    0, 0, 1, 0);
        puts("");
        testEchelon(3, 1, -6, -10,
                    2, 1, -5, -8,
                    6, -3, 3, 0,
                    1, 0, -1, -2,
                    0, 1, -3, -4,
                    0, 0, 0, 0);
        puts("");
        
        testEchelon(1, 0, 1, 1,
                    1, 1, 1, 2,
                    1, -1, 1, 1,
                    1, 0, 1, 0,
                    0, 1, 0, 0,
                    0, 0, 0, 1);
        puts("");
        
        testPlane(10, 0, 10,
                  33, 0, 345,
                  -566, 0, -3454,
                  0, 10, 0, 10);
        
        testPlane(0, -33, 10,
                  0, -2, 345,
                  0, 0, -3454,
                  -30, 0, 0, 30);
        
        testPlane(10, -33, 0,
                  330, -2, 0,
                  1003.2f, 0, 0,
                  0, 0, -200, 200);
        
        
        testPlane(sqrtf(3), 0, 0,
                  0, 1, 0,
                  0, 1, 10,
                  5, 1 + (5*sqrtf(3)), 0, 10);
        
        testPlane(1, 2, -2,
                  3, -2, 1,
                  5, 1, -4,
                  8, 9, -30, 8.4804466608f);
        testPlane(-20, 54, 33,
                  24, 0, 23,
                  9, -200, 8,
                  -20, 0, -30, 57.803589084);
        
    }
    catch(const char* msg)
    {
        NSLog(@"%s", msg);
        throw;
    }
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
