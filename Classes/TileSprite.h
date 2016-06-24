//
//  TileSprite.hpp
//  2048Ai
//
//  Created by tianshuai on 6/23/16.
//
//

#ifndef TileSprite_hpp
#define TileSprite_hpp

#include "cocos2d.h"

class TileSprite : public cocos2d::Node
{
public:
    TileSprite();
    ~TileSprite();
    bool init();
    CREATE_FUNC(TileSprite);
    
    void onEnter();
    
    void setSize(cocos2d::Vec2 size);
    void setNumber(unsigned int num);
    
    void addToLayer(bool isScale);
    void removeFromLayer();
private:
    unsigned int _num;
    cocos2d::Vec2 _size;
    cocos2d::DrawNode *_backgound;
    
    void drawBackGround();
    void addLable();
};


#endif /* TileSprite_hpp */
