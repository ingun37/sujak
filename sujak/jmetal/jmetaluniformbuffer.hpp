//
//  jmetaluniformbuffer.h
//  jmetal
//
//  Created by ingun on 9/27/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>

class jmtlBuffer;
@interface jmetaluniformbuffer : NSObject
{
	jmtlBuffer* buffer;
}
-(id)initWithDevice:(id<MTLDevice>)device;
@end
