//
//  CLogReaderInterface.h
//  Meta
//
//  Created by Konstantin Klyatskin on 2019-10-30.
//  Copyright © 2019 Konstantin Klyatskin. All rights reserved.
//

#ifndef CLogReaderInterface_h
#define CLogReaderInterface_h

typedef void *CallbackInstance;
void callStringFoundCallback(CallbackInstance instance, char *str);

#endif /* CLogReaderInterface_h */
