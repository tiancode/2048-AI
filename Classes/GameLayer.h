//
//  GameLayer.hpp
//  2048Ai
//
//  Created by tianshuai on 6/23/16.
//
//

#ifndef GameLayer_hpp
#define GameLayer_hpp

#include "cocos2d.h"
#include "Map2048.h"

class GameLayer : public cocos2d::LayerColor
{
public:
    GameLayer();
    ~GameLayer();
    bool init();
    CREATE_FUNC(GameLayer);
    
private:
    cocos2d::Size _visibleSize;
    cocos2d::Vec2 _origin;
    
    cocos2d::Vec2 _beginTouchPos;
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
    void Ai(float dt);
    void aiControl(float dt);
    
private:
    void addMapBg();
    
private:
    Map2048 *_map;
    
private:
    char buff[16];
    cocos2d::Label *bestscore;
    
    void addScore();
    
    cocos2d::Label *score;
    
    void updateScore(float fd);
    void startNewGame(cocos2d::Ref* pSender);
    void startAi(cocos2d::Ref* pSender);
    
    cocos2d::MenuItemLabel *ailable;
    bool aiRuning;
    
    cocos2d::Vec2 beginPos;
    cocos2d::Vec2 movePos;
    long curTime;
    int timeInter;
    
    bool isGameOver;

};

#endif /* GameLayer_hpp */
