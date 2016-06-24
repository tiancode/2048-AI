//
//  GameOverLayer.cpp
//  2048Ai
//
//  Created by tianshuai on 6/23/16.
//
//

#include "GameOverLayer.h"

#include "Header.h"
#include "GameScene.h"

USING_NS_CC;
GameOverLayer::GameOverLayer()
{
    
}

GameOverLayer::~GameOverLayer()
{
    
}

bool GameOverLayer::init()
{
    if( !LayerColor::initWithColor( Color4B(255, 255, 255, 150)) )
    {
        return false;
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameOverLayer::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    Size _visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 _origin = Director::getInstance()->getVisibleOrigin();
    
    Label *gameOver = Label::createWithTTF("Game Over!", "fonts/ClearSans-Bold.ttf", _visibleSize.width/7);
    gameOver->setColor(Color3B(121,110,100) );
    gameOver->setPosition(Vec2( _visibleSize.width/2, _visibleSize.height/2) );
    this->addChild(gameOver);
    
    DrawNode *tryagain = DrawNode::create();
    tryagain->drawSolidRect( Vec2(-_visibleSize.width/6 ,-g_tileSize/4), Vec2(_visibleSize.width/6 ,g_tileSize/4), Color4F(Color3B(147,122,98)) );
    tryagain->setPosition( Vec2(_visibleSize.width/4 , _visibleSize.height/4 ) );
    this->addChild(tryagain);
    
    Label *ngame = Label::createWithTTF("Try Again", "fonts/ClearSans-Bold.ttf",  _visibleSize.width/17);
    ngame->setColor(Color3B(250,246,241));
    //tryagain->addChild(ngame);
    
    DrawNode *tween = DrawNode::create();
    tween->drawSolidRect( Vec2(-_visibleSize.width/6 ,-g_tileSize/4), Vec2(_visibleSize.width/6 ,g_tileSize/4), Color4F(Color3B(147,122,98)) );
    tween->setPosition( Vec2(_visibleSize.width-_visibleSize.width/4 , _visibleSize.height/4 ) );
    this->addChild(tween);
    
    Label *tweenl = Label::createWithTTF("Tweet!", "fonts/ClearSans-Bold.ttf",  _visibleSize.width/17);
    tweenl->setColor(Color3B(250,246,241));
    //tween->addChild(tweenl);
    
    MenuItemLabel *newgamelabel = MenuItemLabel::create(ngame, CC_CALLBACK_1(GameOverLayer::startNewGame, this));
    newgamelabel->setPosition(tryagain->getPosition());
    
    MenuItemLabel *tweenmenu = MenuItemLabel::create(tweenl, CC_CALLBACK_1(GameOverLayer::startTween, this));
    tweenmenu->setPosition(tween->getPosition());
    
    Menu *menu = Menu::create(newgamelabel,tweenmenu, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    
    return true;
}

void GameOverLayer::startTween(Ref* pSender)
{
    log("starttween");
}

void GameOverLayer::startNewGame(Ref* pSender)
{
    Director::getInstance()->replaceScene(GameScene::create());
}

bool GameOverLayer::onTouchBegan(Touch *touch, Event *event)
{
    return true;
}
