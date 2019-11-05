//
//  CLogReaderWrapper.h
//  Meta
//
//  Created by Konstantin Klyatskin on 2019-10-30.
//  Copyright © 2019 Konstantin Klyatskin. All rights reserved.
//

#ifndef CLogReaderWrapper_h
#define CLogReaderWrapper_h

#import <Foundation/Foundation.h>
#include "CLogReaderInterface.h"


@protocol CLogReaderProtocol

- (void)stringFound:(char *)str;

@end



@interface CLogReaderWrapper: NSObject <CLogReaderProtocol>

+ (CLogReaderWrapper *)sharedReader;
- (bool)setFilter:(const char *)filter;
- (bool)addSourceBlock:(const char *)block withSize:(size_t)block_size;


@property (nonatomic, assign) id <CLogReaderProtocol> delegate;

@end

#endif /* CLogReaderWrapper_h */

