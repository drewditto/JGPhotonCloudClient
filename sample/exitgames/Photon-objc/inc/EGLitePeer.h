/* Exit Games Photon - objC Client Lib
 * Copyright (C) 2004-2013 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#ifndef __EG_LITE_PEER_H
#define __EG_LITE_PEER_H

#import "EGPhotonPeer.h"

@interface EGLitePeer : EGPhotonPeer
{
}

- (id) initWithPhotonListener:(id<EGPhotonListener>)listener; // = false
- (id) initWithPhotonListener:(id<EGPhotonListener>)listener :(bool)useTcp;
- (bool) opRaiseEvent:(bool)sendReliable :(NSDictionary*)evData :(nByte)eventCode; // = 0
- (bool) opRaiseEvent:(bool)sendReliable :(NSDictionary*)evData :(nByte)eventCode :(nByte)channelID; // = NULL, = 0
- (bool) opRaiseEvent:(bool)sendReliable :(NSDictionary*)evData :(nByte)eventCode :(nByte)channelID  :(int*)targetActors :(short)numTargetActors;
- (bool) opRaiseEventCached:(bool)sendReliable :(NSDictionary*)evData :(nByte)eventCode :(nByte)channelID :(nByte)eventCaching; // = RG_OTHERS
- (bool) opRaiseEventCached:(bool)sendReliable :(NSDictionary*)evData :(nByte)eventCode :(nByte)channelID :(nByte)eventCaching :(nByte)receiverGroup;
- (bool) opJoin:(NSString*)gameid; // = NULL, = NULL, = false
- (bool) opJoin:(NSString*)gameId :(NSDictionary*)gameProperties :(NSDictionary*)actorProperties :(bool)broadcastActorProperties;
- (bool) opLeave;
- (bool) opSetPropertiesOfActor:(int)actorNr :(NSDictionary*)properties :(bool)broadcast; // = 0
- (bool) opSetPropertiesOfActor:(int)actorNr :(NSDictionary*)properties :(bool)broadcast :(nByte)channelID;
- (bool) opSetPropertiesOfGame:(NSDictionary*)properties :(bool)broadcast; // = 0
- (bool) opSetPropertiesOfGame:(NSDictionary*)properties :(bool)broadcast :(nByte)channelID;
- (bool) opGetProperties; // = 0
- (bool) opGetProperties:(nByte)channelID;
- (bool) opGetPropertiesOfActorByStringKeys:(NSString**)properties :(short)numProperties :(int*)actorNrList :(short)numActors; // = 0
- (bool) opGetPropertiesOfActorByStringKeys:(NSString**)properties :(short)numProperties :(int*)actorNrList :(short)numActors :(nByte)channelID;
- (bool) opGetPropertiesOfActorByByteKeys:(nByte*)properties :(short)numProperties :(int*)actorNrList :(short)numActors; // = 0
- (bool) opGetPropertiesOfActorByByteKeys:(nByte*)properties :(short)numProperties :(int*)actorNrList :(short)numActors :(nByte)channelID;
- (bool) opGetPropertiesOfGameByStringKeys:(NSString**)properties :(short)numProperties; // = 0
- (bool) opGetPropertiesOfGameByStringKeys:(NSString**)properties :(short)numProperties :(nByte)channelID;
- (bool) opGetPropertiesOfGameByByteKeys:(nByte*)properties :(short)numProperties; // = 0
- (bool) opGetPropertiesOfGameByByteKeys:(nByte*)properties :(short)numProperties :(nByte)channelID;
@end

#endif