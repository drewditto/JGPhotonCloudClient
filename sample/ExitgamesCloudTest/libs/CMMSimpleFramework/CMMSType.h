#import "GLES-Render.h"

#define PTM_RATIO 32.0f

static inline CGPoint ccpFromb2Vec2(b2Vec2 vector_){
	return ccp(vector_.x,vector_.y);
}
static inline CGPoint ccpFromb2Vec2_PTM_RATIO(b2Vec2 vector_){
	return ccp(vector_.x*PTM_RATIO,vector_.y*PTM_RATIO);
}

static inline b2Vec2 b2Vec2_PTM_RATIO(float x_, float y_){
	return b2Vec2(x_/PTM_RATIO,y_/PTM_RATIO);
}

static inline b2Vec2 b2Vec2Mult(b2Vec2 vector_, float multValue_){
	return b2Vec2(vector_.x*multValue_,vector_.y*multValue_);
}
static inline b2Vec2 b2Vec2Div(b2Vec2 vector_, float divValue_){
	return b2Vec2(vector_.x/divValue_,vector_.y/divValue_);
}

static inline b2Vec2 b2Vec2Fromccp(CGPoint point_){
	return b2Vec2(point_.x,point_.y);
}
static inline b2Vec2 b2Vec2Fromccp_PTM_RATIO(CGPoint point_){
	return b2Vec2(point_.x/PTM_RATIO,point_.y/PTM_RATIO);
}
static inline b2Vec2 b2Vec2FromSize(CGSize size_){
	return b2Vec2(size_.width,size_.height);
}
static inline b2Vec2 b2Vec2FromSize_PTM_RATIO(CGSize size_){
	return b2Vec2(size_.width/PTM_RATIO,size_.height/PTM_RATIO);
}

static inline CGFloat b2Vec2ToAngle(b2Vec2 vector_){
	return atan2f(vector_.y, vector_.x);
}

typedef uint16 CMMb2FixtureType;

typedef int cmmMaskBit;

struct CMMb2ContactMask{
	CMMb2ContactMask(){
		fixtureType = 0x1001;
		maskBit1 = -1;
		maskBit2 = -1;
		checkBit = 0;
		parentBit = -1;
		isNegative = false;
	}
	
	CMMb2FixtureType fixtureType;
	cmmMaskBit maskBit1;
	cmmMaskBit maskBit2;
	cmmMaskBit checkBit;
	cmmMaskBit parentBit;
	bool isNegative;
};
typedef CMMb2ContactMask CMMb2ContactMask;

static inline CMMb2ContactMask CMMb2ContactMaskMake(CMMb2FixtureType fixtureType_, cmmMaskBit maskBit1_, cmmMaskBit maskBit2_, cmmMaskBit checkBit_){
	CMMb2ContactMask contackMask_ = CMMb2ContactMask();
	contackMask_.fixtureType = fixtureType_;
	contackMask_.maskBit1 = maskBit1_;
	contackMask_.maskBit2 = maskBit2_;
	contackMask_.checkBit = checkBit_;
	return contackMask_;
}

static inline NSUInteger cmmFuncCMMb2ContactMask_GetBitCount(CMMb2ContactMask *maskA_,CMMb2ContactMask *maskB_){
	NSUInteger checkCount_ = 0;
	bool negativeCheck_ = (maskA_->isNegative || maskB_->isNegative);
	
	if(maskA_->maskBit1 >= 0 && maskB_->maskBit1 >= 0 && (maskA_->maskBit1 == maskB_->maskBit1) != negativeCheck_)
		++checkCount_;
	if(maskA_->maskBit2 >= 0 && maskB_->maskBit2 >= 0 && (maskA_->maskBit2 == maskB_->maskBit2) != negativeCheck_)
		++checkCount_;
	
	return checkCount_;
}
static inline bool cmmFuncCMMb2ContactMask_IsContact(CMMb2ContactMask *maskA_,CMMb2ContactMask *maskB_){
	if((maskA_->parentBit >= 0 && maskA_->parentBit == maskB_->maskBit1)
	   || (maskB_->parentBit >= 0 && maskB_->parentBit == maskA_->maskBit1)) return false;
	
	int bitCount_ = cmmFuncCMMb2ContactMask_GetBitCount(maskA_,maskB_);
	
	return maskA_->checkBit>bitCount_ || maskB_->checkBit>bitCount_;
}

static inline CGPoint cmmFuncCMMStage_GetContactPoint(b2Contact* contact){
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);
	return ccpFromb2Vec2_PTM_RATIO(worldManifold.points[0]);
}

struct CMMStageDef{
	CMMStageDef(){
		stageSize = worldSize = CGSizeZero;
		gravity = CGPointZero;
		friction = 0.3f;
		restitution = 0.3f;
		density = 0.7f;
		brightness = 1.0f;
	}
	CMMStageDef(CGSize stageSize_, CGSize worldSize_, CGPoint gravity_, float friction_, float restitution_, float density_,float brightness_):stageSize(stageSize_),worldSize(worldSize_),gravity(gravity_),friction(friction_),restitution(restitution_),density(density_),brightness(brightness_){}
	
	CMMStageDef Clone(){
		return CMMStageDef(stageSize,worldSize,gravity,friction,restitution,density,brightness);
	}
	
	CGSize stageSize,worldSize;
	CGPoint gravity;
	float friction,restitution,density,brightness;
};
typedef CMMStageDef CMMStageDef;

static inline CMMStageDef CMMStageDefMake(CGSize stageSize_, CGSize worldSize_, CGPoint gravity_){
	CMMStageDef stageDef_ = CMMStageDef();
	stageDef_.stageSize = stageSize_;
	stageDef_.worldSize = worldSize_;
	stageDef_.gravity = gravity_;
	return stageDef_;
}

@protocol CMMSContactProtocol <NSObject>

@required
-(void)whenContactBeganWithFixtureType:(CMMb2FixtureType)fixtureType_ otherObject:(id<CMMSContactProtocol>)otherObject_ otherFixtureType:(CMMb2FixtureType)otherFixtureType_ contactPoint:(CGPoint)contactPoint_;
-(void)whenContactEndedWithFixtureType:(CMMb2FixtureType)fixtureType_ otherObject:(id<CMMSContactProtocol>)otherObject_ otherFixtureType:(CMMb2FixtureType)otherFixtureType_ contactPoint:(CGPoint)contactPoint_;

-(void)doContactWithFixtureType:(CMMb2FixtureType)fixtureType_ otherObject:(id<CMMSContactProtocol>)otherObject_ otherFixtureType:(CMMb2FixtureType)otherFixtureType_ contactPoint:(CGPoint)contactPoint_ interval:(ccTime)interval_;

@end

@interface NSCoder(Box2d)

-(void)encodeB2Vec2:(b2Vec2)vector_ forKey:(NSString *)key;
-(b2Vec2)decodeB2Vec2ForKey:(NSString *)key;

@end

@implementation NSCoder(Box2d)

-(void)encodeB2Vec2:(b2Vec2)vector_ forKey:(NSString *)key{
	[self encodeCGPoint:ccpFromb2Vec2(vector_) forKey:key];
}
-(b2Vec2)decodeB2Vec2ForKey:(NSString *)key{
	return b2Vec2Fromccp([self decodeCGPointForKey:key]);
}

@end