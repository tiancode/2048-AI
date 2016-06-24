//
//  GameScene.hpp
//  2048Ai
//
//  Created by tianshuai on 6/23/16.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <cocos2d.h>

class GameScene : public cocos2d::Scene
{
public:
    GameScene();
    ~GameScene();
    
    bool init();
    CREATE_FUNC(GameScene);
};

#endif /* GameScene_hpp */
