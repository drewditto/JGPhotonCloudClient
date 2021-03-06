/* Exit Games Photon LoadBalancing - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_LOAD_BALANCING_LISTENER_H
#define __EG_LOAD_BALANCING_LISTENER_H

#import <Foundation/Foundation.h>

#import "EGBaseListener.h"
#import "Enums/DebugLevel.h"

@class EGLoadBalancingPlayer;
@class EGArray;

@protocol EGLoadBalancingListener <EGBaseListener>

// receive and print out PhotonLoadBalancing debug out here
- (void) debugReturn:(ExitGames::Common::DebugLevel::DebugLevel)debugLevel :(const NSString* const)string;

// implement your error-handling here
- (void) connectionErrorReturn:(int)errorCode;
- (void) clientErrorReturn:(int)errorCode;
- (void) warningReturn:(int)warningCode;
- (void) serverErrorReturn:(int)errorCode;

// events, triggered by certain operations of all players in the same room
- (void) joinRoomEventAction:(int)playerNr :(const EGArray* const)playernrs :(const EGLoadBalancingPlayer* const)player;
- (void) leaveRoomEventAction:(int)playerNr;
- (void) customEventAction:(int)playerNr :(nByte)eventCode :(const NSDictionary* const)eventContent;

// callbacks for operations on PhotonLoadBalancing server
- (void) connectReturn:(int)errorCode :(const NSString* const)errorString;
- (void) disconnectReturn;
- (void) createRoomReturn:(int)localPlayerNr :(const NSDictionary* const)roomProperties :(const NSDictionary* const)playerProperties :(int)errorCode :(const NSString* const)errorString;
- (void) joinRoomReturn:(int)localPlayerNr :(const NSDictionary* const)roomProperties :(const NSDictionary* const)playerProperties :(int)errorCode :(const NSString* const)errorString;
- (void) joinRandomRoomReturn:(int)localPlayerNr :(const NSDictionary* const)roomProperties :(const NSDictionary* const)playerProperties :(int)errorCode :(const NSString* const)errorString;
- (void) leaveRoomReturn:(int)errorCode :(const NSString* const)errorString;
- (void) gotQueuedReturn;
- (void) joinLobbyReturn;
- (void) leaveLobbyReturn;

@end

/** @file */

#endif