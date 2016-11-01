/*                                                                            
  Copyright (c) 2014-2015, GoBelieve     
    All rights reserved.		    				     			
 
  This source code is licensed under the BSD-style license found in the
  LICENSE file in the root directory of this source tree. An additional grant
  of patent rights can be found in the PATENTS file in the same directory.
*/

#import <Foundation/Foundation.h>
#import "Message.h"
#import "TCPConnection.h"

@class IMessage;

@protocol IMPeerMessageHandler <NSObject>
-(BOOL)handleMessage:(IMMessage*)msg uid:(int64_t)uid;
-(BOOL)handleMessageACK:(int)msgLocalID uid:(int64_t)uid;
-(BOOL)handleMessageFailure:(int)msgLocalID uid:(int64_t)uid;
@end

@protocol IMGroupMessageHandler <NSObject>

-(BOOL)handleMessage:(IMMessage*)msg;
-(BOOL)handleMessageACK:(int)msgLocalID gid:(int64_t)gid;
-(BOOL)handleMessageFailure:(int)msgLocalID gid:(int64_t)gid;

-(BOOL)handleGroupNotification:(NSString*)notification;
@end

@protocol IMCustomerMessageHandler <NSObject>
-(BOOL)handleCustomerSupportMessage:(CustomerMessage*)msg;
-(BOOL)handleMessage:(CustomerMessage*)msg;
-(BOOL)handleMessageACK:(CustomerMessage*)msg;
-(BOOL)handleMessageFailure:(CustomerMessage*)msg;
@end


@protocol PeerMessageObserver <NSObject>
@optional
-(void)onPeerMessage:(IMMessage*)msg;

//服务器ack
-(void)onPeerMessageACK:(int)msgLocalID uid:(int64_t)uid;

//消息发送失败
-(void)onPeerMessageFailure:(int)msgLocalID uid:(int64_t)uid;

//对方正在输入
-(void)onPeerInputing:(int64_t)uid;

@end

@protocol GroupMessageObserver <NSObject>
@optional
-(void)onGroupMessage:(IMMessage*)msg;
-(void)onGroupMessageACK:(int)msgLocalID gid:(int64_t)gid;
-(void)onGroupMessageFailure:(int)msgLocalID gid:(int64_t)gid;

-(void)onGroupNotification:(NSString*)notification;
@end

@protocol RoomMessageObserver <NSObject>
@optional
-(void)onRoomMessage:(RoomMessage*)rm;
-(void)onRoomMessageACK:(RoomMessage*)rm;
-(void)onRoomMessageFailure:(RoomMessage*)rm;

@end

@protocol RTMessageObserver <NSObject>

@optional
-(void)onRTMessage:(RTMessage*)rt;

@end

@protocol SystemMessageObserver <NSObject>
@optional
-(void)onSystemMessage:(NSString*)sm;

@end

@protocol CustomerMessageObserver <NSObject>
@optional
-(void)onCustomerMessage:(CustomerMessage*)msg;
-(void)onCustomerSupportMessage:(CustomerMessage*)msg;

//服务器ack
-(void)onCustomerMessageACK:(CustomerMessage*)msg;
//消息发送失败
-(void)onCustomerMessageFailure:(CustomerMessage*)msg;
@end

@protocol VOIPObserver <NSObject>

-(void)onVOIPControl:(VOIPControl*)ctl;

@end

@interface IMService : TCPConnection
@property(nonatomic, copy) NSString *deviceID;
@property(nonatomic, copy) NSString *token;
@property(nonatomic) int64_t uid;
//客服app需要设置，普通app不需要设置
@property(nonatomic) int64_t appID;

@property(nonatomic, weak)id<IMPeerMessageHandler> peerMessageHandler;
@property(nonatomic, weak)id<IMGroupMessageHandler> groupMessageHandler;
@property(nonatomic, weak)id<IMCustomerMessageHandler> customerMessageHandler;

+(IMService*)instance;

-(BOOL)isPeerMessageSending:(int64_t)peer id:(int)msgLocalID;
-(BOOL)isGroupMessageSending:(int64_t)groupID id:(int)msgLocalID;
-(BOOL)isCustomerSupportMessageSending:(int)msgLocalID
                            customerID:(int64_t)customerID
                         customerAppID:(int64_t)customerAppID;
-(BOOL)isCustomerMessageSending:(int)msgLocalID storeID:(int64_t)storeID;

-(BOOL)sendPeerMessage:(IMMessage*)msg;
-(BOOL)sendGroupMessage:(IMMessage*)msg;
-(BOOL)sendRoomMessage:(RoomMessage*)msg;
//顾客->客服
-(BOOL)sendCustomerMessage:(CustomerMessage*)im;
//客服->顾客
-(BOOL)sendCustomerSupportMessage:(CustomerMessage*)im;
-(BOOL)sendRTMessage:(RTMessage*)msg;

-(void)enterRoom:(int64_t)roomID;
-(void)leaveRoom:(int64_t)roomID;

//正在输入
-(void)sendInputing:(MessageInputing*)inputing;
//更新未读的消息数目
-(void)sendUnreadCount:(int)unread;

-(void)addPeerMessageObserver:(id<PeerMessageObserver>)ob;
-(void)removePeerMessageObserver:(id<PeerMessageObserver>)ob;

-(void)addGroupMessageObserver:(id<GroupMessageObserver>)ob;
-(void)removeGroupMessageObserver:(id<GroupMessageObserver>)ob;

-(void)addRoomMessageObserver:(id<RoomMessageObserver>)ob;
-(void)removeRoomMessageObserver:(id<RoomMessageObserver>)ob;

-(void)addSystemMessageObserver:(id<SystemMessageObserver>)ob;
-(void)removeSystemMessageObserver:(id<SystemMessageObserver>)ob;

-(void)addCustomerMessageObserver:(id<CustomerMessageObserver>)ob;
-(void)removeCustomerMessageObserver:(id<CustomerMessageObserver>)ob;

-(void)addRTMessageObserver:(id<RTMessageObserver>)ob;
-(void)removeRTMessageObserver:(id<RTMessageObserver>)ob;
    
-(void)pushVOIPObserver:(id<VOIPObserver>)ob;
-(void)popVOIPObserver:(id<VOIPObserver>)ob;

-(BOOL)sendVOIPControl:(VOIPControl*)ctl;

@end

