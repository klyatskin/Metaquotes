//
//  ViewController.m
//  Meta
//
//  Created by Konstantin Klyatskin on 2019-10-30.
//  Copyright © 2019 Konstantin Klyatskin. All rights reserved.
//

#import "ViewController.h"
#import "CLogReaderWrapper.h"

@interface ViewController () <CLogReaderProtocol> {
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [CLogReaderWrapper sharedReader].delegate = self;
    [[CLogReaderWrapper sharedReader] setFilter:"123"];
}


#pragma mark - CLogReaderProtocol

- (void)stringFound:(char *)str {
    NSLog(@"%@", [NSString stringWithUTF8String:str]);
}

@end
