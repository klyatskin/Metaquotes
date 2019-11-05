//
//  UIViewController+UIViewController_Message.m
//  Meta
//
//  Created by Konstantin Klyatskin on 2019-11-04.
//  Copyright © 2019 Konstantin Klyatskin. All rights reserved.
//

#import "UIViewController+Message.h"

@implementation UIViewController (Message)


- (void)showMessage:(NSString *)msg {
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Attention"
                                                                   message:msg
                                                            preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction* okButton = [UIAlertAction actionWithTitle:@"OK"
                                                       style:UIAlertActionStyleDefault
                                                     handler:nil ];
    [alert addAction:okButton];
    [self presentViewController:alert animated:YES completion:nil];
}



@end
