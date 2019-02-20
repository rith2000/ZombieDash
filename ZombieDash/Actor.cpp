#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, StudentWorld* src): GraphObject(imageID, startX, startY){
    m_world = src;
}

Actor::~Actor(){
    
}

StudentWorld* Actor::getWorld(){
    return m_world;
}

Penelope::Penelope(int startX, int startY, StudentWorld* src): Actor(IID_PLAYER, startX, startY, src){
    setDirection(right);
}

Penelope::~Penelope(){
    
}

void Penelope::doSomething(){
    int dir = 0;
    if (getWorld()->getKey(dir))
    {
        switch (dir)
        {
            case KEY_PRESS_LEFT:
                setDirection(left);
                if (!getWorld()->isInWall(getX() - 4, getY()))
                    moveTo(getX() - 4, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if (!getWorld()->isInWall(getX() + 4, getY()))
                    moveTo(getX() + 4, getY());
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if (!getWorld()->isInWall(getX(), getY() + 4))
                    moveTo(getX(), getY() + 4);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if (!getWorld()->isInWall(getX(), getY() - 4))
                    moveTo(getX(), getY() - 4);
                break;
            default:
                break;
        }

    }
    
}

Wall::Wall(int startX, int startY, StudentWorld* src): Actor( IID_WALL, startX, startY, src){
    setDirection(right);
}

Wall::~Wall(){
    
}

void Wall::doSomething(){
    
}



