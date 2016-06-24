//
//  TileSprite.cpp
//  2048Ai
//
//  Created by tianshuai on 6/23/16.
//
//

#include "TileSprite.h"
#include "Header.h"

USING_NS_CC;


float g_tileSize = 0;

TileSprite::TileSprite()
{
    _num = 0;
    _size = Vec2::ZERO;
}

TileSprite::~TileSprite()
{
    
}

bool TileSprite::init()
{
    if( !Node::init() )
    {
        return false;
    }
    
    _backgound = DrawNode::create();
    this->addChild(_backgound);
    
    return true;
}

void TileSprite::drawBackGround()
{
    switch (_num)
    {
        case 2:
            _backgound->drawSolidRect( Vec2(-_size.x/2, -_size.y/2), Vec2(_size.x/2, _size.y/2), Color4F(Color3B(240,228,216) ) );
            break;
        case 4:
            _backgound->drawSolidRect( Vec2(-_size.x/2, -_size.y/2), Vec2(_size.x/2, _size.y/2), Color4F(Color3B(239,225,197) ) );
            break;
        case 8:
            _backgound->drawSolidRect( Vec2(-_size.x/2, -_size.y/2), Vec2(_size.x/2, _size.y/2), Color4F(Color3B(254,175,106) ) );
            break;
        case 16:
            _backgound->drawSolidRect( Vec2(-_size.x/2, -_size.y/2), Vec2(_size.x/2, _size.y/2), Color4F(Color3B(255,143,79) ) );
            break;
        case 32:
            _backgound->drawSolidRect( Vec2(-_size.x/2, -_size.y/2), Vec2(_size.x/2, _size.y/2), Color4F(Color3B(255,115,76) ) );
            break;
        case 64:
            _backgound->drawSolidRect( Vec2(-_size.x/2, -_size.y/2), Vec2(_size.x/2, _size.y/2), Color4F(Color3B(255,75,0) ) );
            break;
        case 128:
            _backgound->drawSolidRect( Vec2(-_size.x/2, -_size.y/2), Vec2(_size.x/2, _size.y/2), Color4F(Color3B(242,210,94) ) );
            break;
        case 256:
            _backgound->drawSolidRect( Vec2(-_size.x/2, -_size.y/2), Vec2(_size.x/2, _size.y/2), Color4F(Color3B(244,207,69) ) );
            break;
        case 512:
            _backgound->drawSolidRect( Vec2(-_size.x/2, -_size.y/2), Vec2(_size.x/2, _size.y/2), Color4F(Color3B(243,205,36) ) );
            break;
        case 1024:
            _backgound->drawSolidRect( Vec2(-_size.x/2, -_size.y/2), Vec2(_size.x/2, _size.y/2), Color4F(Color3B(245,201,0) ) );
            break;
        case 2048:
            _backgound->drawSolidRect( Vec2(-_size.x/2, -_size.y/2), Vec2(_size.x/2, _size.y/2), Color4F(Color3B(245,190,0) ) );
            break;
        default:
            //printf("drawbackground");
            break;
    }
}

void TileSprite::addLable()
{
    char buff[16];
    sprintf(buff, "%d", _num);
    if(_num < 100)
    {
        Label *str = Label::createWithTTF( buff, "fonts/ClearSans-Bold.ttf", g_tileSize/1.9);
        if(_num < 5)
        str->setColor(Color3B(121,110,100) );
        else
        str->setColor(Color3B(250,246,241) );
        _backgound->addChild(str, 1);
        str->setPosition( Vec2::ZERO);
    }
    if(_num >100 && _num < 999)
    {
        Label *str = Label::createWithTTF( buff, "fonts/ClearSans-Bold.ttf", g_tileSize/2.2);
        str->setColor(Color3B(250,246,241) );
        _backgound->addChild(str, 1);
        str->setPosition( Vec2::ZERO );
    }
    if(_num > 1000)
    {
        Label *str = Label::createWithTTF( buff, "fonts/ClearSans-Bold.ttf", g_tileSize/2.6);
        str->setColor(Color3B(250,246,241) );
        _backgound->addChild(str, 1);
        str->setPosition( Vec2::ZERO );
    }
}

void TileSprite::onEnter()
{
    Node::onEnter();
    
    drawBackGround();
    addLable();
}

void TileSprite::setNumber(unsigned int num)
{
    _num = num;
}

void TileSprite::setSize(Vec2 size)
{
    _size = size;
}

void TileSprite::addToLayer(bool isScale)
{
    if(isScale)
    {
        this->setScale(0.1);
        this->runAction(ScaleTo::create(0.15, 1));
    }
    else
    {
        this->setScale(0.5);
        Sequence *seq = Sequence::create(ScaleTo::create(0.1, 1.2), ScaleTo::create(0.05, 1), NULL);
        this->runAction(seq);
    }
    g_mapBg->addChild(this, 1);
}

void TileSprite::removeFromLayer()
{
    g_mapBg->removeChild(this);
}
