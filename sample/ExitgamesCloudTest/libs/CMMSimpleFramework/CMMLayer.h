//  Created by JGroup(kimbobv22@gmail.com)

#import "CMMType.h"
#import "CMMMacro.h"
#import "CMMTouchUtil.h"
#import "CMMTouchDispatcher.h"

@interface CMMLayer : CCLayerColor<CMMTouchDispatcherDelegate>{
	CMMTouchDispatcher *touchDispatcher;
}

@property (nonatomic, readonly) CMMTouchDispatcher *touchDispatcher;

@end