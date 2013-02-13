#import "cocos2d.h"

//
//  CCMutableTexture.h
//	Created by Lam Hoang Pham.
//
//  Edited by Kimbobv22.
//

@interface CCMutableTexture2D : CCTexture2D{
	void *_orgData,*_data;
}

-(ccColor4B)pixelAtPoint:(CGPoint)point_;

-(void)setPixelAtPoint:(CGPoint)point_ pixel:(ccColor4B)pixel_;
-(void)setPixelCircleAtPoint:(CGPoint)point_ radius:(float)radius_ pixel:(ccColor4B)pixel_;

-(void)apply;
-(void)applyPixelAtRect:(CGRect)rect_;

-(void)restore;

@end
