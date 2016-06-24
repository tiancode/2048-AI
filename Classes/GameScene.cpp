//
//  GameScene.cpp
//  2048Ai
//
//  Created by tianshuai on 6/23/16.
//
//

#include "GameScene.h"
#include "GameLayer.h"

USING_NS_CC;

GameScene::GameScene()
{
    
}

GameScene::~GameScene()
{
    
}

bool GameScene::init()
{
    if( !Scene::init() )
    {
        return false;
    }
    
    GameLayer *gamelayer = GameLayer::create();
    this->addChild(gamelayer);
    
    return true;
}