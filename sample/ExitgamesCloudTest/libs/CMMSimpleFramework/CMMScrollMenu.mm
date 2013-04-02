//  Created by JGroup(kimbobv22@gmail.com)
// 12.08.15

#import "CMMScrollMenu.h"

@interface CMMScrollMenu(Private)

-(void)_removeItemDirect:(CCNode *)item_;

@end

@implementation CMMScrollMenu(Private)

-(void)_removeItemDirect:(CCNode *)item_{
	[innerTouchDispatcher cancelTouchAtNode:(id<CMMTouchDispatcherDelegate>)item_];
	[itemList removeObject:item_];
	[item_ removeFromParentAndCleanup:NO];
	[self doUpdateInnerSize];
	[self setTouchState:CMMTouchState_onScroll];
	
	int count_ = [self count];
	if(index>=count_)
		[self setIndex:count_-1];
}

@end

@implementation CMMScrollMenu
@synthesize index,count,itemList,marginPerItem,canSelectItem;
@synthesize callback_whenIndexChanged,callback_whenTapAtIndex,callback_whenItemAdded,callback_whenItemRemoved,callback_whenItemSwitched,callback_whenItemLinkSwitched;
@synthesize filter_canChangeIndex,filter_canAddItem,filter_canRemoveItem,filter_canSwitchItem,filter_canLinkSwitchItem;

+(id)scrollMenuWithFrameSize:(CGSize)frameSize_ color:(ccColor4B)tcolor_{
	return [[[self alloc] initWithColor:tcolor_ width:frameSize_.width height:frameSize_.height] autorelease];
}
+(id)scrollMenuWithFrameSize:(CGSize)frameSize_{
	return [[[self alloc] initWithColor:cmmVarCMMScrollMenu_defaultColor width:frameSize_.width height:frameSize_.height] autorelease];
}

+(id)scrollMenuWithFrameSeq:(uint)frameSeq_ batchBarSeq:(uint)batchBarSeq_ frameSize:(CGSize)frameSize_{
	CMMScrollMenu *scrollMenu_ = [self scrollMenuWithFrameSize:frameSize_];
	
	CCSprite *frameSprite_ = [CCSprite spriteWithTexture:[[CMMDrawingManager sharedManager] textureBatchBarWithFrameSeq:frameSeq_ batchBarSeq:batchBarSeq_ size:frameSize_]];
	[scrollMenu_ addChild:frameSprite_];
	frameSprite_.position = cmmFuncCommon_positionInParent(scrollMenu_, frameSprite_);
	CMMLayerMDScrollbar scrollBar_ = CMMLayerMDScrollbar();
	scrollBar_.distanceX = scrollBar_.distanceY = 6.0f;
	[scrollMenu_ setScrollbar:scrollBar_];
	
	return scrollMenu_;
}

-(id)initWithColor:(ccColor4B)color width:(GLfloat)w height:(GLfloat)h{
	if(!(self = [super initWithColor:color width:w height:h])) return self;
	
	index = -1;
	itemList = [[CCArray alloc] init];
	marginPerItem = 1.0f;
	canSelectItem = YES;
	
	return self;
}

-(void)setContentSize:(CGSize)contentSize{
	[super setContentSize:contentSize];
	if(itemList) [self doUpdateInnerSize];
}

-(int)count{
	return itemList.count;
}

-(void)update:(ccTime)dt_{
	[super update:dt_];
	//arragne item
	[self updateMenuArrangeWithInterval:dt_];
}

-(void)touchDispatcher:(CMMTouchDispatcher *)touchDispatcher_ whenTouchBegan:(UITouch *)touch_ event:(UIEvent *)event_{
	if(!canSelectItem || touchState == CMMTouchState_onScroll){
		[self setTouchState:CMMTouchState_onDrag];
		return;
	}

	[super touchDispatcher:touchDispatcher_ whenTouchBegan:touch_ event:event_];
}

-(void)touchDispatcher:(CMMTouchDispatcher *)touchDispatcher_ whenTouchEnded:(UITouch *)touch_ event:(UIEvent *)event_{
	CMMTouchDispatcherItem *touchItem_ = [innerTouchDispatcher touchItemAtTouch:touch_];
	
	switch(touchState){
		case CMMTouchState_onTouchChild:{
			int touchedIndex_ = [self indexOfItem:(CMMMenuItem *)[touchItem_ node]];
			if(callback_whenTapAtIndex){
				callback_whenTapAtIndex(touchedIndex_);
			}
			[self setIndex:touchedIndex_];
			
			break;
		}
		default: break;
	}
	
	[super touchDispatcher:touchDispatcher_ whenTouchEnded:touch_ event:event_];
}

-(void)setIndex:(int)index_{
	if(index == index_) return;

	if(filter_canChangeIndex)
		if(!filter_canChangeIndex(index_)) return;
	
	index = index_;
	
	if(callback_whenIndexChanged)
		callback_whenIndexChanged(index);
}

-(NSUInteger)countByEnumeratingWithState:(NSFastEnumerationState *)state objects:(id *)stackbuf count:(NSUInteger)len{
	return [itemList countByEnumeratingWithState:state objects:stackbuf count:len];
}

-(void)cleanup{
	[self setCallback_whenIndexChanged:nil];
	[self setCallback_whenTapAtIndex:nil];
	[self setCallback_whenItemAdded:nil];
	[self setCallback_whenItemRemoved:nil];
	[self setCallback_whenItemSwitched:nil];
	[self setCallback_whenItemLinkSwitched:nil];
	
	[self setFilter_canChangeIndex:nil];
	[self setFilter_canAddItem:nil];
	[self setFilter_canRemoveItem:nil];
	[self setFilter_canSwitchItem:nil];
	[self setFilter_canLinkSwitchItem:nil];
	
	[super cleanup];
}

-(void)dealloc{
	[callback_whenIndexChanged release];
	[callback_whenTapAtIndex release];
	[callback_whenItemAdded release];
	[callback_whenItemRemoved release];
	[callback_whenItemSwitched release];
	[callback_whenItemLinkSwitched release];
	
	[filter_canChangeIndex release];
	[filter_canAddItem release];
	[filter_canRemoveItem release];
	[filter_canSwitchItem release];
	[filter_canLinkSwitchItem release];
	
	[itemList release];
	[super dealloc];
}

@end

@implementation CMMScrollMenu(Display)

-(void)doUpdateInnerSize{}
-(void)updateMenuArrangeWithInterval:(ccTime)dt_{}

@end

@implementation CMMScrollMenu(Common)

-(void)addItem:(CMMMenuItem *)item_ atIndex:(int)index_{
	if([self indexOfItem:item_] != NSNotFound) return;
	
	if(filter_canAddItem)
		if(!filter_canAddItem(item_,index_)) return;
	
	[itemList insertObject:item_ atIndex:index_];
	[self doUpdateInnerSize];
	[innerLayer addChild:item_ z:cmmVarCMMScrollMenu_defaultChildZOrder];
	
	if(callback_whenItemAdded)
		callback_whenItemAdded(item_,index_);
}
-(void)addItem:(CMMMenuItem *)item_{
	[self addItem:item_ atIndex:[self count]];
}

-(void)removeItem:(CMMMenuItem *)item_{
	int targetIndex_ = [self indexOfItem:item_];
	if(targetIndex_ == NSNotFound) return;
	
	if(filter_canRemoveItem)
		if(!filter_canRemoveItem(item_)) return;
	
	if(callback_whenItemRemoved)
		callback_whenItemRemoved(item_);
	
	[self _removeItemDirect:item_];
}
-(void)removeItemAtIndex:(int)index_{
	[self removeItem:[self itemAtIndex:index_]];
}
-(void)removeAllItems{
	ccArray *data_ = itemList->data;
	for(int index_=data_->num-1;index_>=0;--index_){
		CMMMenuItem *item_ = data_->arr[index_];
		[self removeItem:item_];
	}
}

-(CMMMenuItem *)itemAtIndex:(int)index_{
	if(index_ < 0 || index_>=[self count] || index_ == NSNotFound) return nil;
	return [itemList objectAtIndex:index_];
}

@end

@implementation CMMScrollMenu(Switch)

-(BOOL)switchItem:(CMMMenuItem *)item_ toIndex:(int)toIndex_{
	int targetIndex_ = [self indexOfItem:item_];
	if(targetIndex_ == NSNotFound || targetIndex_ == toIndex_) return NO;
	
	
	if(filter_canSwitchItem){
		if(!filter_canSwitchItem(item_,toIndex_)) return NO;
	}else return NO; //must have filter
	
	[itemList exchangeObjectAtIndex:targetIndex_ withObjectAtIndex:toIndex_];
	
	if(callback_whenItemSwitched)
		callback_whenItemSwitched(item_,toIndex_);
	
	return YES;
}
-(BOOL)switchItemAtIndex:(int)index_ toIndex:(int)toIndex_{
	return [self switchItem:[self itemAtIndex:index_] toIndex:toIndex_];
}

-(BOOL)linkSwitchItem:(CMMMenuItem *)item_ toScrolMenu:(CMMScrollMenu *)toScrolMenu_ toIndex:(int)toIndex_{
	if([self indexOfItem:item_] == NSNotFound || !toScrolMenu_) return NO;
	
	if(filter_canLinkSwitchItem){
		if(!filter_canLinkSwitchItem(item_,toScrolMenu_,toIndex_)) return NO;
	}else return NO; //must have filter
	
	item_ = [[item_ retain] autorelease];
	[self _removeItemDirect:item_]; // delete directly
	[toScrolMenu_ addItem:item_ atIndex:toIndex_];
	
	if(callback_whenItemLinkSwitched)
		callback_whenItemLinkSwitched(item_,toScrolMenu_,toIndex_);
	
	return YES;
}
-(BOOL)linkSwitchItemAtIndex:(int)index_ toScrolMenu:(CMMScrollMenu *)toScrolMenu_ toIndex:(int)toIndex_{
	return [self linkSwitchItem:[self itemAtIndex:index_] toScrolMenu:toScrolMenu_ toIndex:toIndex_];
}

@end

@implementation CMMScrollMenu(Search)

-(int)indexOfItem:(CMMMenuItem *)item_{
	return [itemList indexOfObject:item_];
}
-(int)indexOfPoint:(CGPoint)worldPoint_{
	CGPoint convertPoint_ = [innerLayer convertToNodeSpace:worldPoint_];
	ccArray *data_ = itemList->data;
	int count_ = data_->num;
	for(uint index_=0;index_<count_;++index_){
		CMMMenuItem *item_ = data_->arr[index_];
		if(CGRectContainsPoint([item_ boundingBox], convertPoint_))
			return index_;
	}
	
	return NSNotFound;
}
-(int)indexOfTouch:(UITouch *)touch_{
	return [self indexOfPoint:[CMMTouchUtil pointFromTouch:touch_]];
}

@end
