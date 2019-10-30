//
//  CLogReader.cpp
//  Meta
//
//  Created by Konstantin Klyatskin on 2019-10-30.
//  Copyright © 2019 Konstantin Klyatskin. All rights reserved.
//

#include <string.h>
#include <stdlib.h>

#include "CLogReader.hpp"


CLogReader::CLogReader()
: mFilter( NULL )
, mCallbackInstance( NULL)
{
    printf("CLogReader created\n");
}

CLogReader::~CLogReader()  // never be called for singleton
{
    if (mFilter)
        free(mFilter);
    mFilter = NULL;
    mCallbackInstance = NULL;
    printf("CLogReader deAllocated\n");
}

void CLogReader::SetCallbackInstance(CallbackInstance instance)
{
    mCallbackInstance = instance;
}



bool CLogReader::SetFilter( const char* filter )
{
//    if (mFilter)
//        free(mFilter);

    printf("SetFilter\n");
    
    char testStr[] = "test from callback. Remove it later. \n";
    callStringFoundCallback(mCallbackInstance, testStr);
    return true;
}


bool AddSourceBlock(const char* block,const size_t block_size)
{
    return true;
}

