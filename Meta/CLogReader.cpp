//
//  CLogReader.cpp
//  Meta
//
//  Created by Konstantin Klyatskin on 2019-10-30.
//  Copyright © 2019 Konstantin Klyatskin. All rights reserved.
//

#define MAX_STRING_LEN 65000

#include "CLogReader.hpp"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CLogReader::CLogReader()
: mFilter( NULL )
, mCallbackInstance( NULL)
, mOldStart( NULL )
{
//    printf("CLogReader created\n");
}

CLogReader::~CLogReader()  // never be called for singleton
{
    if (mFilter)
        free(mFilter);
    mFilter = NULL;
    mCallbackInstance = NULL;
//    printf("CLogReader deAllocated\n");
}

void CLogReader::SetCallbackInstance(CallbackInstance instance)
{
    mCallbackInstance = instance;
}



bool CLogReader::SetFilter( const char* filter )
{
    if (mFilter)
        free(mFilter);
    size_t len = strlen(filter);
    mFilter = (char*) malloc( len + 1 );
    if (!mFilter)
        return false;
    if (!strcpy(mFilter, filter))
        return false;
    // remove doubled *'s
    for (char *ps(mFilter), *pd(mFilter);;) {
        bool w(false);
        while (*ps == '*') {
            w = true;
            *pd = *ps;
            ++ps;
        }
        if (!w) {
            *pd = *ps;
            if (*ps == '\0')
                break;
            ++ps;
        }
        ++pd;
    }
    mOldStart = 0;
    return true;
}


// COPIED fromn https://github.com/gildor2/UEViewer/blob/master/Core/Core.cpp
bool CLogReader::WildTextCompare(
                            const char *pTameText,   // A string without wildcards
                            const char *pWildText    // A (potentially) corresponding string with wildcards
)
{
    // These two values are set when we observe a wildcard character.  They
    // represent the locations, in the two strings, from which we start once
    // we've observed it.
    const char *pTameBookmark = NULL;
    const char *pWildBookmark = NULL;
    
    if (*pTameText == 0 || *pWildText == 0 )    // empty strings
        return false;
    
    // Walk the text strings one character at a time.
    while (true)
    {
        // How do you match a unique text string?
        if (*pWildText == '*')
        {
            // Easy: unique up on it!
            while (*(++pWildText) == '*')
            {
            }                          // "xy" matches "x**y"
            
            if (!*pWildText)
            {
                return true;           // "x" matches "*"
            }
            
            if (*pWildText != '?')
            {
                // Fast-forward to next possible match.
                while (*pTameText != *pWildText)
                {
                    if (!(*(++pTameText)))
                        return false;  // "x" doesn't match "*y*"
                }
            }
            
            pWildBookmark = pWildText;
            pTameBookmark = pTameText;
        }
        else if (*pTameText != *pWildText && *pWildText != '?')
        {
            // Got a non-match.  If we've set our bookmarks, back up to one
            // or both of them and retry.
            //
            if (pWildBookmark)
            {
                if (pWildText != pWildBookmark)
                {
                    pWildText = pWildBookmark;
                    
                    if (*pTameText != *pWildText)
                    {
                        // Don't go this far back again.
                        pTameText = ++pTameBookmark;
                        continue;      // "xy" matches "*y"
                    }
                    else
                    {
                        pWildText++;
                    }
                }
                
                if (*pTameText)
                {
                    pTameText++;
                    continue;          // "mississippi" matches "*sip*"
                }
            }
            
            return false;              // "xy" doesn't match "x"
        }
        
        pTameText++;
        pWildText++;
        
        // How do you match a tame text string?
        if (!*pTameText)
        {
            // The tame way: unique up on it!
            while (*pWildText == '*')
            {
                pWildText++;           // "x" matches "x*"
            }
            
            if (!*pWildText)
            {
                return true;           // "x" matches "x"
            }
            
            return false;              // "x" doesn't match "xy"
        }
    }
}

bool CLogReader::AddSourceBlock(const char* block,const size_t block_size)
{
    static char buffer[MAX_STRING_LEN];

    int i = mOldStart;
    for (int j =0; j<= block_size; ) {
        buffer[i] = block[j];
        if (block[j] == '\n' || block[j] == '\r' ) {
            buffer[i] = 0;
            mOldStart = i = 0;
            if (WildTextCompare(buffer, mFilter))
                callStringFoundCallback(mCallbackInstance, buffer);
        } else {
            if (++i >= MAX_STRING_LEN) {
                mOldStart = 0;
                return false;
            }
        }
        j++;
    }
    if (i>=1)
        mOldStart = i-1;
    return true;
}

