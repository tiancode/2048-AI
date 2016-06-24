//
//  GameLayer.cpp
//  2048Ai
//
//  Created by tianshuai on 6/23/16.
//
//

#include "GameLayer.h"
#include "TileSprite.h"
#include "Score.h"

#include "Header.h"

#include "GameScene.h"
#include "GameOverLayer.h"

USING_NS_CC;

GameLayer::GameLayer()
{
    _map = new Map2048;
    aiRuning = false;
    isGameOver = false;
}

GameLayer::~GameLayer()
{
    delete _map;
}

bool GameLayer::init()
{
    if( !LayerColor::initWithColor( Color4B(250, 248, 238, 255)) )
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    _origin = Director::getInstance()->getVisibleOrigin();
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    addMapBg();
    
    addScore();
    
    schedule(schedule_selector(GameLayer::updateScore), 0.2f, kRepeatForever, 0);
    return true;
}

void GameLayer::updateScore(float fd)
{
    int s = _map->getScore();
    sprintf(buff, "%d", s);
    score->setString(buff);
    if(s > Score::getInstance()->getTopScore() )
    {
        Score::getInstance()->setTopScore(s);
    }
    
    // game over
    if(isGameOver)
    {
        GameOverLayer *g = GameOverLayer::create();
        this->addChild(g);
        unschedule(schedule_selector(GameLayer::updateScore));
    }
}

void GameLayer::Ai(float dt)
{
    _map->getBestMove();
}
void GameLayer::aiControl(float dt)
{
    if(_map->unused == true)
    {
        if(_map->Control() == false)
        {
            aiRuning = false;
            unschedule(schedule_selector(GameLayer::Ai));
            unschedule(schedule_selector(GameLayer::aiControl));
            ailable->stopAllActions();
        }
        _map->unused = false;
    }
}

bool GameLayer::onTouchBegan(Touch *touch, Event *event)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    static long preTime = tv.tv_sec * 1000 + tv.tv_usec / 1000 -1000;
    curTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    timeInter = curTime - preTime + timeInter;
    preTime = curTime;
    if(timeInter >= 200)
    {
        beginPos = touch->getLocation();
        
        timeInter = 0;
        
        return true;
    }
    return false;
}

void GameLayer::onTouchEnded(Touch *touch, Event *event)
{
    movePos = touch->getLocation();
    
    Vec2 v = movePos - beginPos;
    if(v.length() > g_tileSize)
    {
        Vec2 pro_X = v.project(Vec2::UNIT_X);
        Vec2 pro_Y = v.project(Vec2::UNIT_Y);
        
        //left right
        if(pro_X.lengthSquared() > pro_Y.lengthSquared())
        {
            if(pro_X.x > 0)
            {
                if(_map->doControl(RIGHT) == false)
                {
                    isGameOver = true;
                }
            }
            else if(pro_X.x < 0)
            {
                if(_map->doControl(LEFT) == false)
                {
                    isGameOver = true;
                }
            }
        }
        else if(pro_Y.lengthSquared() > pro_X.lengthSquared())
        {
            if(pro_Y.y > 0)
            {
                if(_map->doControl(UP) == false)
                {
                    isGameOver = true;
                }
            }
            else if(pro_Y.y < 0)
            {
                if(_map->doControl(DOWN) == false)
                {
                    isGameOver = true;
                }
            }
        }
    }
}

Node* g_mapBg;
float g_gap;
void GameLayer::addMapBg()
{
    float width = _visibleSize.width;
    DrawNode *mapBg = DrawNode::create();
    mapBg->drawSolidRect( Vec2::ZERO , Vec2(width, width), Color4F(Color3B(190,173,158)));
    mapBg->setAnchorPoint(Vec2::ZERO);
    this->addChild(mapBg);
    
    Vec2 mapOrigin = mapBg->getPosition();
    float gap = width/33;
    float tileWidth = (width-5*gap)/4;
    
    g_tileSize = tileWidth;
    g_mapBg = mapBg;
    g_gap = gap;
    
    for(int i = 0; i < MAP_SIZE; i++)
    {
        for(int j = 0; j < MAP_SIZE; j++)
        {
            DrawNode* tileBg = DrawNode::create();
            tileBg->drawSolidRect(Vec2::ZERO, Vec2(tileWidth, tileWidth) , Color4F( Color3B(208,193,179) ));
            tileBg->setPositionX((j+1)*gap+ j*tileWidth);
            tileBg->setPositionY((i+1)*gap+ i*tileWidth);
            _map->setTilePos(3-i, j, tileBg->getPosition() + Vec2(g_tileSize/2 , g_tileSize/2));
            mapBg->addChild(tileBg);
        }
    }
    
    _map->initStart2();
}

void GameLayer::addScore()
{
    // label2048
    Label *_2048 = Label::createWithTTF("2048", "fonts/ClearSans-Bold.ttf", _visibleSize.width/5.9);
    _2048->setColor(Color3B(121,110,100) );
    _2048->setAnchorPoint(Vec2( 0, 0.5 ));
    _2048->setPosition(Vec2( 0, _visibleSize.height-_2048->getContentSize().height/2) );
    this->addChild(_2048);
    
    // best score
    DrawNode *bestscoreback = DrawNode::create();
    bestscoreback->drawSolidRect( Vec2(-_visibleSize.width/6 ,-g_tileSize/3), Vec2(_visibleSize.width/6 ,g_tileSize/3), Color4F(Color3B(190,173,158)) );
    bestscoreback->setPosition( Vec2(_visibleSize.width - _visibleSize.width/6-10 , _2048->getPositionY()-10 ) );
    this->addChild(bestscoreback);
    Label *best = Label::createWithTTF("BEST", "fonts/ClearSans-Bold.ttf", _visibleSize.width/25);
    best->setColor(Color3B(240,228,216));
    best->setPositionY(g_tileSize/6.5 );
    bestscoreback->addChild(best);
    
    sprintf(buff, "%d", Score::getInstance()->getTopScore());
    bestscore = Label::createWithTTF(buff, "fonts/ClearSans-Bold.ttf", _visibleSize.width/15);
    bestscore->setPositionY( -g_tileSize/7.5);
    bestscoreback->addChild(bestscore);
    
    //score
    DrawNode *scoreback = DrawNode::create();
    scoreback->drawSolidRect( Vec2(-_visibleSize.width/6 ,-g_tileSize/3), Vec2(_visibleSize.width/6 ,g_tileSize/3), Color4F(Color3B(190,173,158)) );
    scoreback->setPosition( Vec2(_visibleSize.width - _visibleSize.width/6-10 , _2048->getPositionY() - g_tileSize/1 ) );
    this->addChild(scoreback);
    
    Label *sco = Label::createWithTTF("SCORE", "fonts/ClearSans-Bold.ttf", _visibleSize.width/25);
    sco->setColor(Color3B(240,228,216));
    sco->setPositionY(g_tileSize/6.5 );
    scoreback->addChild(sco);
    
    sprintf(buff, "%d", 0);
    score = Label::createWithTTF(buff, "fonts/ClearSans-Bold.ttf", _visibleSize.width/15);
    score->setPositionY( -g_tileSize/7.5);
    scoreback->addChild(score);
    
    // new game
    DrawNode *newGame = DrawNode::create();
    newGame->drawSolidRect( Vec2(-_visibleSize.width/6 ,-g_tileSize/4), Vec2(_visibleSize.width/6 ,g_tileSize/4), Color4F(Color3B(147,122,98)) );
    newGame->setPosition( Vec2(_visibleSize.width/5.5 , _2048->getPositionY() - g_tileSize/1 ) );
    this->addChild(newGame);
    
    Label *ngame = Label::createWithTTF("New Game", "fonts/ClearSans-Bold.ttf",  _visibleSize.width/17);
    ngame->setColor(Color3B(250,246,241));
    //newGame->addChild(ngame);
    
    // ai
    DrawNode *aiback = DrawNode::create();
    aiback->drawSolidRect( Vec2(-_visibleSize.width/15 ,-g_tileSize/4), Vec2(_visibleSize.width/15 ,g_tileSize/4), Color4F(Color3B(147,122,98)) );
    aiback->setPosition( Vec2(_visibleSize.width/2.3 , _2048->getPositionY() - g_tileSize/1 ) );
    this->addChild(aiback);
    
    Label *ai = Label::createWithTTF("AI", "fonts/ClearSans-Bold.ttf",  _visibleSize.width/17);
    ai->setColor(Color3B(250,246,241));
    //aiback->addChild(ai);
    
    MenuItemLabel *newgamelabel = MenuItemLabel::create(ngame, CC_CALLBACK_1(GameLayer::startNewGame, this));
    newgamelabel->setPosition(newGame->getPosition());
    
    ailable = MenuItemLabel::create(ai, CC_CALLBACK_1(GameLayer::startAi, this));
    ailable->setPosition(aiback->getPosition());
    
    Menu *menu = Menu::create(newgamelabel,ailable, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
}

void GameLayer::startNewGame(Ref* pSender)
{
    Director::getInstance()->replaceScene(GameScene::create());
}

void GameLayer::startAi(Ref* pSender)
{
    _map->unused = false;
    if(aiRuning == false)
    {
        aiRuning = true;
        schedule(schedule_selector(GameLayer::Ai), 0.2f, kRepeatForever, 0);
        schedule(schedule_selector(GameLayer::aiControl), 0.2f, kRepeatForever, 0.1);
        ailable->runAction(RepeatForever::create(RotateBy::create(2, 360)) );
    }
    else
    {
        aiRuning = false;
        unschedule(schedule_selector(GameLayer::Ai));
        unschedule(schedule_selector(GameLayer::aiControl));
        ailable->stopAllActions();
    }
}


