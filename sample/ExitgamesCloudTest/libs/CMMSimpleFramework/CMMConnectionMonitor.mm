//  Created by JGroup(kimbobv22@gmail.com)

#import "CMMConnectionMonitor.h"

#import <sys/socket.h>
#import <netinet/in.h>
#import <netinet6/in6.h>
#import <arpa/inet.h>
#import <ifaddrs.h>
#import <netdb.h>

static void cmmFuncCMMConnectionMonitor_ReachabilityCallback(SCNetworkReachabilityRef target, SCNetworkReachabilityFlags flags, void* info){
	NSCAssert(info != NULL, @"info was NULL in cmmFuncCMMConnectionMonitor_ReachabilityCallback");
	NSCAssert([(NSObject*) info isKindOfClass: [CMMConnectionMonitor class]], @"info was wrong class in cmmFuncCMMConnectionMonitor_ReachabilityCallback");
	
	NSAutoreleasePool* pool_ = [[NSAutoreleasePool alloc] init];
	
	CMMConnectionMonitor *noteObject_ = (CMMConnectionMonitor *) info;
	[[NSNotificationCenter defaultCenter] postNotificationName:cmmVarCMMConnectionMonitor_reachabilityChangedNotification object:noteObject_];
	
	[pool_ release];
}

static CMMConnectionMonitor *_sharedCMMConnectionMonitor_ = nil;

@implementation CMMConnectionMonitor
@synthesize connectionStatus,IPAddress;

+(CMMConnectionMonitor *)sharedMonitor{
	if(!_sharedCMMConnectionMonitor_){
		_sharedCMMConnectionMonitor_ = [[CMMConnectionMonitor alloc] init];
	}
	
	return _sharedCMMConnectionMonitor_;
}

-(id)init{
	if(!(self = [super init])) return self;
	
	struct sockaddr_in zeroAddress;
	bzero(&zeroAddress, sizeof(zeroAddress));
	zeroAddress.sin_len = sizeof(zeroAddress);
	zeroAddress.sin_family = AF_INET;
	
	_reachability =  SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault, (const struct sockaddr*)&zeroAddress);
	
	if(_reachability == NULL){
		[NSException raise:@"CMMConnectionMonitor" format:@"failed to initialize"];
		[self release];
		return nil;
	}
	
	SCNetworkReachabilityContext context_ = {0, self, NULL, NULL, NULL};
	if(SCNetworkReachabilitySetCallback(_reachability, cmmFuncCMMConnectionMonitor_ReachabilityCallback, &context_)){
		if(!SCNetworkReachabilityScheduleWithRunLoop(_reachability, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode)){
			[NSException raise:@"CMMConnectionMonitor" format:@"failed to initialize"];
			[self release];
			return nil;
		}
	}
	
	return self;
}

-(CMMConnectionStatus)connectionStatus{
	SCNetworkReachabilityFlags flags_;
	if(SCNetworkReachabilityGetFlags(_reachability, &flags_)){
		if((flags_ & kSCNetworkReachabilityFlagsReachable) == 0){
			return CMMConnectionStatus_noConnection;
		}
		
		CMMConnectionStatus connectionStatus_ = CMMConnectionStatus_noConnection;
		
		if((flags_ & kSCNetworkReachabilityFlagsConnectionRequired) == 0){
			connectionStatus_ = CMMConnectionStatus_WiFi;
		}
		
		if((((flags_ & kSCNetworkReachabilityFlagsConnectionOnDemand ) != 0) ||
			 (flags_ & kSCNetworkReachabilityFlagsConnectionOnTraffic) != 0)){
			
			if((flags_ & kSCNetworkReachabilityFlagsInterventionRequired) == 0){
				connectionStatus_ = CMMConnectionStatus_WiFi;
			}
		}
		
		if((flags_ & kSCNetworkReachabilityFlagsIsWWAN) == kSCNetworkReachabilityFlagsIsWWAN){
			connectionStatus_ = CMMConnectionStatus_WWAN;
		}
		
		return connectionStatus_;
	}
	
	return CMMConnectionStatus_noConnection;
}

-(NSString *)IPAddressOfTargetStatus:(CMMConnectionStatus)targetStatus_{
	if(targetStatus_ == CMMConnectionStatus_noConnection) return nil;
	
	NSString *address_ = nil;
    struct ifaddrs *interfaces_ = NULL;
    struct ifaddrs *tempAddress_ = NULL;
    int success_ = getifaddrs(&interfaces_);
	
	if(success_ == 0){
		tempAddress_ = interfaces_;
		while(tempAddress_ != NULL){
			if(tempAddress_->ifa_addr->sa_family == AF_INET){
				
				BOOL isCorrectInterface_ = NO;
				switch(targetStatus_){
					case CMMConnectionStatus_WiFi:{
						NSString *sourceID_ = [NSString stringWithUTF8String:tempAddress_->ifa_name];
						NSString *interfaceIDFormatter_ = @"en%d";
						for(uint index_=0;index_<10;++index_){
							if([sourceID_ isEqualToString:[NSString stringWithFormat:interfaceIDFormatter_,index_]]){
								isCorrectInterface_ = YES;
							}
						}
						
						break;
					}
					case CMMConnectionStatus_WWAN:
					default:
						if([[NSString stringWithUTF8String:tempAddress_->ifa_name] isEqualToString:@"pdp_ip0"]){
							isCorrectInterface_ = YES;
						}
						break;
				}
				
				if(isCorrectInterface_){
					address_ = [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)tempAddress_->ifa_addr)->sin_addr)];
					break;
				}
			}
			
			tempAddress_ = tempAddress_->ifa_next;
		}
	}
	
    freeifaddrs(interfaces_);
    return address_;
}
-(NSString *)IPAddress{
	return [self IPAddressOfTargetStatus:[self connectionStatus]];
}

-(void)addObserverForConnectionStatus:(id)target_ selector:(SEL)selector_{
	[[NSNotificationCenter defaultCenter] addObserver:target_ selector:selector_ name:cmmVarCMMConnectionMonitor_reachabilityChangedNotification object:nil];
}
-(void)removeObserverForConnectionStatus:(id)target_{
	[[NSNotificationCenter defaultCenter] removeObserver:target_ name:cmmVarCMMConnectionMonitor_reachabilityChangedNotification object:nil];
}

-(void)dealloc{
	if(_reachability!= NULL){
		SCNetworkReachabilityUnscheduleFromRunLoop(_reachability, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
		CFRelease(_reachability);
	}
	[super dealloc];
}

@end
