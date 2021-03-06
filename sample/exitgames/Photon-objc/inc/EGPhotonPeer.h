/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_PHOTON_PEER_H
#define __EG_PHOTON_PEER_H

#import <Foundation/NSNull.h>
#import "PhotonPeer.h"
#import "Logger.h"
#import "EGPhotonListener.h"
#import "EGOperationRequest.h"
#import "EGBase.h"
#import "EGTime.h"
#import "NSString+UTF32.h"
#import "EGDictionary.h"
#import "NSObject+CopyDeep.h"
#import "EGCustomTypeUtils.h"

@interface EGPhotonPeer : NSObject
{
@protected
	ExitGames::Photon::PhotonPeer* m_pPhotonPeer;
	ExitGames::Common::Logger mLogger;
	ExitGames::Photon::PhotonListener* m_pListenerWrapper;
@private
	id<EGPhotonListener> m_plistener;
}

@property (readonly) id<EGBaseListener> Listener;
@property (readonly) int ServerTimeOffset;
@property (readonly) int ServerTime;
@property (readonly) int BytesOut;
@property (readonly) int BytesIn;
@property (readonly) ExitGames::Photon::PeerState::PeerState Peerstate;
@property (readwrite) int SentCountAllowance;
@property (readwrite) int TimePingInterval;
@property (readonly) int RoundTripTime;
@property (readonly) int RoundTripTimeVariance;
@property (readwrite) ExitGames::Common::DebugLevel::DebugLevel DebugOutputLevel;
@property (readonly) short PeerID;
@property (readonly) bool IsEncryptionAvailable;
@property (readonly) int IncomingReliableCommandsCount;
@property (readwrite) int SentTimeAllowance;
@property (readonly) int QueuedIncomingCommands;
@property (readonly) int QueuedOutgoingCommands;
@property (readonly) const NSString* ServerAddress;
@property (readonly) short PeerCount;

- (id) initWithPhotonListener:(id<EGPhotonListener>)listener; // = false
- (id) initWithPhotonListener:(id<EGPhotonListener>)listener :(bool)useTcp;
- (bool) connect:(const NSString* const)ipAddr; // = NULL
- (bool) connect:(const NSString* const)ipAddr :(const nByte[ExitGames::Photon::Internal::InternalProperties::APP_NAME_LENGTH])appID;
- (void) disconnect;
- (void) service; // = true
- (void) service:(bool)dispatchIncomingCommands;
- (void) serviceBasic;
- (bool) opCustom:(const EGOperationRequest* const)operationRequest :(bool)sendReliable; // = 0, = false
- (bool) opCustom:(const EGOperationRequest* const)operationRequest :(bool)sendReliable :(nByte)channelID; // = false
- (bool) opCustom:(const EGOperationRequest* const)operationRequest :(bool)sendReliable :(nByte)channelID :(bool)encrypt;
- (void) sendOutgoingCommands;
- (bool) dispatchIncomingCommands;
- (bool) establishEncryption;
- (void) fetchServerTimestamp;

@end

#endif