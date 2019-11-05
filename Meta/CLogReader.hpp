//
//  CLogReader.hpp
//  Meta
//
//  Created by Konstantin Klyatskin on 2019-10-30.
//  Copyright © 2019 Konstantin Klyatskin. All rights reserved.
//

#ifndef CLogReader_hpp
#define CLogReader_hpp

#include <stdio.h>
#include "CLogReaderInterface.h"


class CLogReader
{
public:
    CLogReader();
    ~CLogReader();
    bool SetFilter(const char *filter);   // установка фильтра строк, false - ошибка
    bool AddSourceBlock(const char* block,const size_t block_size); // добавление очередного блока текстового файла
    void SetCallbackInstance(CallbackInstance instance);

private:
    bool WildTextCompare(const char *pTameText,   // A string without wildcards
                         const char *pWildText    // A (potentially) corresponding string with wildcards
                         );
private:
    char *mFilter;
    void *mCallbackInstance;
    int mOldStart;
};


#endif /* CLogReader_hpp */
