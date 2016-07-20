//
//  jmetalstreambuffer.h
//  jmetal
//
//  Created by ingun on 9/27/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "jmetalindexbuffer.hpp"
#import "jmetalvertexbuffer.hpp"
namespace sujak
{
    class jrenderobject;
}
@interface jmetalstreambuffer : NSObject
{
	jmetalindexbuffer* index;
	jmetalvertexbuffer* vertex;
}
-(id)initWithDevcie:(id<MTLDevice>)device;
-(void)loadObject:(sujak::jrenderobject*)obj;
@end
