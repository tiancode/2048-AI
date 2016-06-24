//
//  GameOverLayer.hpp
//  2048Ai
//
//  Created by tianshuai on 6/23/16.
//
//

#ifndef GameOverLayer_hpp
#define GameOverLayer_hpp

#include "cocos2d.h"

class GameOverLayer : public cocos2d::LayerColor
{
public:
    GameOverLayer();
    ~GameOverLayer();
    bool init();
    CREATE_FUNC(GameOverLayer);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void startNewGame(cocos2d::Ref* pSender);
    void startTween(cocos2d::Ref* pSender);
    
};

#endif /* GameOverLayer_hpp */
