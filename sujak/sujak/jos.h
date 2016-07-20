//
//  jos.h
//  sujak
//
//  Created by ingun on 7/20/16.
//  Copyright © 2016 loadcomplete. All rights reserved.
//

#ifndef jos_h
#define jos_h

namespace sujak
{
    class jos
    {
        public:
        virtual void getFile(const char* szFileName, const char* szExt, char* &file, unsigned long& size)=0;
        virtual void getObjInfo(const char* jobjname, char* nameskel, char* namemesh, size_t namebufflen)=0;
    };
}

#endif /* jos_h */
