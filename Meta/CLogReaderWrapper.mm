//
//  CLogReaderWrapper.mm
//  Meta
//
//  Created by Konstantin Klyatskin on 2019-10-30.
//  Copyright © 2019 Konstantin Klyatskin. All rights reserved.
//

#import "CLogReaderWrapper.h"
#import "CLogReader.hpp"


@implementation CLogReaderWrapper

CLogReader _reader;

void callStringFoundCallback(CallbackInstance instance, char *str) {
    [(CLogReaderWrapper*) instance stringFound:str];
}


+ (CLogReaderWrapper *)sharedReader {
    static dispatch_once_t once;
    static CLogReaderWrapper* shared;
    dispatch_once(&once, ^{
        shared = [[self alloc] init];
        _reader.SetCallbackInstance(shared);
    });
    return shared;
}


- (bool)setFilter:(const char *)filter {
    return _reader.SetFilter(filter);
}

//- bool AddSourceBlock(const char* block,const size_t block_size); // добавление очередного блока текстового файла

#pragma mark - CLogReaderProtocol

- (void)stringFound:(char *)str {
    [self.delegate stringFound:str];
}

@end
