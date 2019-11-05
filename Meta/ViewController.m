//
//  ViewController.m
//  Meta
//
//  Created by Konstantin Klyatskin on 2019-10-30.
//  Copyright © 2019 Konstantin Klyatskin. All rights reserved.
//

#import "ViewController.h"
#import "CLogReaderWrapper.h"
#import "UIViewController+Message.h"

@interface ViewController () <CLogReaderProtocol, NSURLSessionDataDelegate> {
    int _lineCount;
}
@property(nonatomic, retain) NSFileHandle *fh;
@property(nonatomic, copy) NSString *filePath;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [CLogReaderWrapper sharedReader].delegate = self;
    self.textView.text = @"";
    self.textURL.text = self.textURL.placeholder;
    self.textMask.text = self.textMask.placeholder;
}

- (void)startLoading {
    NSURL *nsurl = [NSURL URLWithString:[self.textURL.text stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLQueryAllowedCharacterSet]]];
    NSURLSessionConfiguration *sessionConfiguration = [NSURLSessionConfiguration defaultSessionConfiguration];
    NSURLSession * session = [NSURLSession sessionWithConfiguration:sessionConfiguration
                                                           delegate:self
                                                      delegateQueue:nil];
    NSURLSessionDataTask *dataTask = [session dataTaskWithURL:nsurl];
    
    [[NSFileManager defaultManager] createFileAtPath:self.filePath contents:NULL attributes:NULL];
    self.filePath = [[NSHomeDirectory() stringByAppendingString:@"/Documents/"] stringByAppendingString:[nsurl lastPathComponent]];
    self.fh = [NSFileHandle fileHandleForWritingAtPath:self.filePath];
    [dataTask resume];
}


#pragma - NSURLSessionDataDelegate

-(void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask didReceiveResponse:(NSURLResponse *)response completionHandler:(void (^)(NSURLSessionResponseDisposition))completionHandler{
    NSHTTPURLResponse *resp = (NSHTTPURLResponse *)response;
    NSLog(@"URL %@ with status = %@", resp.URL, [NSHTTPURLResponse localizedStringForStatusCode:resp.statusCode]);
    completionHandler(NSURLSessionResponseAllow); // Allow this request to continue
}

- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask didReceiveData:(NSData *)data {
    @synchronized (self) {
        [data enumerateByteRangesUsingBlock:^(const void *bytes, NSRange byteRange, BOOL *stop) {
            [[CLogReaderWrapper sharedReader] addSourceBlock:bytes withSize:byteRange.length];
        }];
    }
}

-(void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error{
    dispatch_async(dispatch_get_main_queue(), ^{
        [self.fh closeFile];
        self.fh = nil;
        if (error) {
            [self showMessage:[error debugDescription]];
        } else {
            [self showMessage:[NSString stringWithFormat:@"\nSearch completed.\n\nResult saved at\n\n%@", self.filePath]];
        }
    });
    [session invalidateAndCancel];
}


#pragma mark - CLogReaderProtocol

- (void)stringFound:(char *)str {
    NSString *line = [NSString stringWithCString:str encoding:NSASCIIStringEncoding];
    dispatch_async(dispatch_get_main_queue(), ^{
        [self updateWithFoundString:line];
    });
}


#pragma mark - UIUpdate

- (void)updateWithFoundString:(NSString *)line {
    NSString * brLine = [NSString stringWithFormat:@"\n%d: ", _lineCount++];
    brLine = @"\n";
    NSString *str = [brLine stringByAppendingString:line];
    int maxLine = 30; // stop updating UI...
    if (_lineCount < maxLine) {
        self.textView.text = [self.textView.text stringByAppendingString: str];
    } else if (_lineCount == maxLine) {
        self.textView.text = [self.textView.text stringByAppendingString:[NSString stringWithFormat:@"\n\nToo many strings found. Please check log file at %@", self.filePath]];
    }
    @try {
        [self.fh writeData:[str dataUsingEncoding:NSUTF8StringEncoding]];
    }
    @catch (NSException * e) {
        [self showMessage:e.debugDescription];
    }
}

- (void)dealloc {
    [CLogReaderWrapper sharedReader].delegate = nil;
    [_textView release];
    [_textURL release];
    [_textMask release];
    [_textView release];
    [_fh release];
    [_filePath release];
    [super dealloc];
}

#pragma mark - Actions

- (IBAction)buttonTapped:(UIButton *)sender {
    if (self.textURL.text.length == 0) {
        [self showMessage:@"No URL provided"];
        return;
    }
    if (self.textMask.text.length == 0) {
        [self showMessage:@"No Mask provided"];
        return;
    }
    _textView.text = @"";
    _lineCount = 0;
    [[CLogReaderWrapper sharedReader] setFilter:[self.textMask.text UTF8String]];
    [self startLoading];
}


@end
