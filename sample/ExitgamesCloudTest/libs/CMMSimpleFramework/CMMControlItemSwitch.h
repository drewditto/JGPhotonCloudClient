//  Created by JGroup(kimbobv22@gmail.com)

#import "CMMControlItem.h"
#import "CMMMenuItem.h"

@interface CMMControlItemSwitch : CMMControlItem{
	CMMMenuItem *buttonItem;
	CCSprite *_backSprite,*_maskSprite,*_resultBackSprite;
	
	BOOL itemValue,_isTouchMoved;
	
	void (^callback_whenItemValueChanged)(BOOL itemValue_);
}

+(id)controlItemSwitchWithMaskSprite:(CCSprite *)maskSprite_ backSprite:(CCSprite *)backSprite_ buttonSprite:(CCSprite *)buttonSprite_;
+(id)controlItemSwitchWithFrameSeq:(int)frameSeq_;

-(id)initWithMaskSprite:(CCSprite *)maskSprite_ backSprite:(CCSprite *)backSprite_ buttonSprite:(CCSprite *)buttonSprite_;
-(id)initWithFrameSeq:(int)frameSeq_;

-(void)setButtonSprite:(CCSprite *)buttonSprite_;

@property (nonatomic, readwrite) BOOL itemValue;
@property (nonatomic, copy) void (^callback_whenItemValueChanged)(BOOL itemValue_);

-(void)setCallback_whenItemValueChanged:(void (^)(BOOL itemValue_))block_;

@end