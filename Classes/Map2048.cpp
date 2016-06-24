//
//  Map2048.cpp
//  2048Ai
//
//  Created by tianshuai on 6/23/16.
//
//

#include "Map2048.h"

#include "TileSprite.h"

USING_NS_CC;

Map2048::Map2048()
{
    srand(time(NULL));
    memset(_map, 0, sizeof(_map));
    
    score = 0;
    isThreadRuning = false;
    
    init_tables();
}

void Map2048::initStart2()
{
    //初始化两个2
    int num = 0;
    while(num <2)
    {
        int row = random(0, 3);
        int col = random(0, 3);
        if(_map[row][col].num == 0)
        {
            _map[row][col].num = 2;
            num++;
            
            /*******************************************************/
            {
                TileSprite *tile = TileSprite::create();
                tile->setNumber(2);
                tile->setSize(Vec2(g_tileSize, g_tileSize));
                tile->setPosition(_map[row][col].pos);
                tile->addToLayer(true);
                _map[row][col].tile = tile;
            }
            /*******************************************************/
        }
    }
    
}

void Map2048::setTilePos(int row,int col, cocos2d::Vec2 pos)
{
    _map[row][col].pos = pos;
}

bool Map2048::doControl(CONTROL contr)
{
    setBoard();
    
    if(isGameOver())
    {
        return false;
    }
    
    if(contr == LEFT)
    {
        remove_left_blank();
        left();
    }
    else if(contr == RIGHT)
    {
        remove_right_blank();
        right();
    }
    else if(contr == UP)
    {
        remove_up_blank();
        up();
    }
    else
    {
        remove_down_blank();
        down();
    }
    
    if(getBoard() != cboard)
    genRandom_2_4();
    
    return true;
}

void Map2048::setBoard()
{
    memset(board, 0, sizeof(board));
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(_map[i][j].num == 0)
            continue;
            board[i][j] = (int)round(log2f(_map[i][j].num));
        }
    }
    
    cboard = 0;
    int k = 0;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            cboard = cboard | (board[i][j] << (4*k) );
            k++;
        }
    }
}
unsigned long long Map2048::getBoard()
{
    memset(board, 0, sizeof(board));
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(_map[i][j].num == 0)
            continue;
            board[i][j] = (int)round(log2f(_map[i][j].num));
        }
    }
    
    unsigned long long cboard_t = 0;
    int k = 0;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            cboard_t = cboard_t | (board[i][j] << (4*k) );
            k++;
        }
    }
    return cboard_t;
}
bool Map2048::aiDoControl()
{
    setBoard();
    
    move = find_best_move(cboard);
    if(move < 0 || move > 3)
    return false;
    
    if(move == 0)
    {
        if(doControl(UP) == false)
        return false;
    }
    if(move == 1)
    {
        if(doControl(DOWN) == false)
        {
            return false;
        }
    }
    if(move == 2)
    {
        if(doControl(LEFT) == false)
        {
            return false;
        }
    }
    if(move == 3)
    {
        if(doControl(RIGHT) == false)
        return false;
    }
    
    if(getBoard() == cboard)
    {
        return false;
    }
    
    return true;
}

void Map2048::findBestMoveThread()
{
    isThreadRuning = true;
    setBoard();
    move = find_best_move(cboard);
    unused = true;
    isThreadRuning = false;
}
void Map2048::getBestMove()
{
    if(isThreadRuning == false && unused == false)
    {
        std::thread t(&Map2048::findBestMoveThread, this);
        t.detach();
    }
}
bool Map2048::Control()
{
    if(move < 0 || move > 3)
    return false;
    
    if(move == 0)
    {
        if(doControl(UP) == false)
        return false;
    }
    if(move == 1)
    {
        if(doControl(DOWN) == false)
        {
            return false;
        }
    }
    if(move == 2)
    {
        if(doControl(LEFT) == false)
        {
            return false;
        }
    }
    if(move == 3)
    {
        if(doControl(RIGHT) == false)
        return false;
    }
    
    if(getBoard() == cboard)
    {
        return false;
    }
    
    return true;
}

void Map2048::print()
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            printf("%d ",_map[i][j].num);
        }
        printf("\n");
    }
    printf("\n");
}

unsigned int Map2048::getScore()
{
    return score;
}

bool Map2048::isGameOver()
{
    if(getBlankNum() != 0)
    return false;
    
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if( (i < 3 && _map[i][j].num == _map[i+1][j].num) ||
               (i > 0 && _map[i][j].num == _map[i-1][j].num) ||
               (j < 3 && _map[i][j].num == _map[i][j+1].num) ||
               (j > 0 && _map[i][j].num == _map[i][j-1].num) )
            {
                return false;
            }
        }
    }
    
    return true;
}

void Map2048::genRandom_2_4()
{
    int blankNum = getBlankNum();
    if(blankNum == 0)
    return;
    
    int num = 0;
    while(num < 1)
    {
        int row = random(0, 3);
        int col = random(0, 3);
        if(_map[row][col].num == 0)
        {
            _map[row][col].num = random(1, 10) == 1 ? 4 : 2;
            num++;
            
            /*******************************************************/
            {
                TileSprite *tile = TileSprite::create();
                tile->setNumber(_map[row][col].num);
                tile->setSize(Vec2(g_tileSize, g_tileSize));
                tile->setPosition(_map[row][col].pos);
                tile->addToLayer(true);
                _map[row][col].tile = tile;
            }
            /*******************************************************/
        }
    }
}

void Map2048::remove_up_blank()
{
    int k;
    for(int j=0; j<4; j++)
    {
        for(int i=1; i<4 ;i++)
        {
            k = i;
            while( k-1 >= 0 && _map[k-1][j].num == 0 )
            {
                if(_map[k][j].num != 0)
                {
                    std::swap( _map[k][j].num,_map[k-1][j].num );
                    
                    /*******************************************************/
                    {
                        _map[k-1][j].tile = _map[k][j].tile;
                        _map[k][j].tile = NULL;
                        _map[k-1][j].tile->runAction(MoveTo::create(0.06, Vec2(_map[k-1][j].tile->getPositionX(),_map[k-1][j].tile->getPositionY()+ g_tileSize + g_gap )));
                        //_map[k-1][j].tile->setPosition(_map[k-1][j].pos);
                    }
                    /*******************************************************/
                }
                
                k--;
            }
        }
    }
}
void Map2048::up()
{
    for(int j=0; j<4; j++)
    {
        for(int i=0; i<3; i++)
        {
            if( _map[i][j].num == _map[i+1][j].num && _map[i][j].num != 0 &&  _map[i][j].num != MAXPOWLIMITE)
            {
                _map[i][j].num += _map[i+1][j].num;
                _map[i+1][j].num = 0;
                
                /*******************************************************/
                {
                    _map[i+1][j].tile->removeFromLayer();
                    _map[i+1][j].tile = NULL;
                    _map[i][j].tile->removeFromLayer();
                    _map[i][j].tile = NULL;
                    TileSprite *tile = TileSprite::create();
                    tile->setNumber(_map[i][j].num);
                    tile->setSize(Vec2(g_tileSize, g_tileSize));
                    tile->setPosition(_map[i][j].pos);
                    tile->addToLayer(false);
                    _map[i][j].tile = tile;
                }
                /*******************************************************/
                
                remove_up_blank();
                
                score += _map[i][j].num;
            }
        }
    }
}

void Map2048::remove_down_blank()
{
    int k;
    for(int j=0; j<4; j++)
    {
        for(int i=2; i>=0; i--)
        {
            k = i;
            while(k+1 <= 3 && _map[k+1][j].num == 0)
            {
                if(_map[k][j].num != 0)
                {
                    std::swap(_map[k][j].num, _map[k+1][j].num);
                    
                    /*******************************************************/
                    {
                        _map[k+1][j].tile = _map[k][j].tile;
                        _map[k][j].tile = NULL;
                        _map[k+1][j].tile->runAction(MoveTo::create(0.06, Vec2(_map[k+1][j].tile->getPositionX(),_map[k+1][j].tile->getPositionY()-g_tileSize -g_gap) ));
                        //_map[k+1][j].tile->setPosition(_map[k+1][j].pos);
                    }
                    /*******************************************************/
                }
                
                k++;
            }
        }
    }
}
void Map2048::down()
{
    for(int j=0; j<4; j++)
    {
        for(int i=3; i>=1; i--)
        {
            if(_map[i][j].num == _map[i-1][j].num && _map[i][j].num != 0 && _map[i][j].num != MAXPOWLIMITE)
            {
                _map[i][j].num = _map[i][j].num + _map[i-1][j].num;
                _map[i-1][j].num = 0;
                
                /*******************************************************/
                {
                    _map[i-1][j].tile->removeFromLayer();
                    _map[i-1][j].tile = NULL;
                    _map[i][j].tile->removeFromLayer();
                    _map[i][j].tile = NULL;
                    TileSprite *tile = TileSprite::create();
                    tile->setNumber(_map[i][j].num);
                    tile->setSize(Vec2(g_tileSize, g_tileSize));
                    tile->setPosition(_map[i][j].pos);
                    tile->addToLayer(false);
                    _map[i][j].tile = tile;
                }
                /*******************************************************/
                
                remove_down_blank();
                
                score += _map[i][j].num;
            }
        }
    }
}

void Map2048::remove_left_blank()
{
    int k;
    for(int i=0; i<4; i++)
    {
        for(int j=1; j<4; j++)
        {
            k = j;
            while( k-1 >= 0 && _map[i][k-1].num == 0)
            {
                if(_map[i][k].num != 0)
                {
                    std::swap( _map[i][k].num, _map[i][k-1].num );
                    
                    /*******************************************************/
                    {
                        _map[i][k-1].tile = _map[i][k].tile;
                        _map[i][k].tile = NULL;
                        _map[i][k-1].tile->runAction(MoveTo::create(0.06, Vec2(_map[i][k-1].tile->getPositionX()-g_tileSize-g_gap, _map[i][k-1].tile->getPositionY()) ));
                        //_map[i][k-1].tile->setPosition(_map[i][k-1].pos);
                    }
                    /*******************************************************/
                }
                k--;
            }
        }
    }
}
void Map2048::left()
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(_map[i][j].num == _map[i][j+1].num && _map[i][j].num != 0 && _map[i][j].num != MAXPOWLIMITE)
            {
                _map[i][j].num += _map[i][j+1].num;
                _map[i][j+1].num = 0;
                
                /*******************************************************/
                {
                    _map[i][j+1].tile->removeFromLayer();
                    _map[i][j+1].tile = NULL;
                    _map[i][j].tile->removeFromLayer();
                    _map[i][j].tile = NULL;
                    TileSprite *tile = TileSprite::create();
                    tile->setNumber(_map[i][j].num);
                    tile->setSize(Vec2(g_tileSize, g_tileSize));
                    tile->setPosition(_map[i][j].pos);
                    tile->addToLayer(false);
                    _map[i][j].tile = tile;
                }
                /*******************************************************/
                
                remove_left_blank();
                
                score += _map[i][j].num;
            }
        }
    }
}


void Map2048::remove_right_blank()
{
    int k;
    for(int i=0; i<4; i++)
    {
        for(int j=2; j>=0; j--)
        {
            k=j;
            while( k+1 <= 3 && _map[i][k+1].num == 0 )
            {
                if(_map[i][k].num != 0)
                {
                    std::swap( _map[i][k].num,_map[i][k+1].num );
                    
                    /*******************************************************/
                    {
                        _map[i][k+1].tile = _map[i][k].tile;
                        _map[i][k].tile = NULL;
                        _map[i][k+1].tile->runAction(MoveTo::create(0.06, Vec2(_map[i][k+1].tile->getPositionX()+g_tileSize+g_gap,_map[i][k+1].tile->getPositionY() )));
                        //_map[i][k+1].tile->setPosition(_map[i][k+1].pos);
                    }
                    /*******************************************************/
                }
                
                k++;
            }
        }
    }
}
void Map2048::right()
{
    for(int i=0; i<4; i++)
    {
        for(int j=3; j>=1; j--)
        {
            if( _map[i][j].num == _map[i][j-1].num && _map[i][j].num != 0 && _map[i][j].num != MAXPOWLIMITE )
            {
                _map[i][j].num += _map[i][j-1].num;
                _map[i][j-1].num = 0;
                
                /*******************************************************/
                {
                    _map[i][j-1].tile->removeFromLayer();
                    _map[i][j-1].tile = NULL;
                    _map[i][j].tile->removeFromLayer();
                    _map[i][j].tile = NULL;
                    TileSprite *tile = TileSprite::create();
                    tile->setNumber(_map[i][j].num);
                    tile->setSize(Vec2(g_tileSize, g_tileSize));
                    tile->setPosition(_map[i][j].pos);
                    tile->addToLayer(false);
                    _map[i][j].tile = tile;
                }
                /*******************************************************/
                
                remove_right_blank();
                
                score += _map[i][j].num;
            }
        }
    }
}

int Map2048::getBlankNum()
{
    int n = 0;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(_map[i][j].num == 0)
            {
                n++;
            }
        }
    }
    return n;
}
