#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, StudentWorld* src, int dir, int depth, char ID): GraphObject(imageID, startX, startY, dir, depth){
    m_world = src;
    m_ID = ID;
}

Actor::~Actor(){}

char Actor::getID(){
    return m_ID;
}

bool Actor::overlaps(Actor* a){
    return (pow(a->getX() - this->getX(), 2) + pow(a->getY() - this->getY(), 2) <= 100);
}

StudentWorld* Actor::getWorld(){
    return m_world;
}

Penelope::Penelope(int startX, int startY, StudentWorld* src): Actor(IID_PLAYER, startX, startY, src, right, 0, '@'){}

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

Wall::Wall(int startX, int startY, StudentWorld* src): Actor(IID_WALL, startX, startY, src, right, 0, '#'){
}

void Wall::doSomething(){}

bool Wall::overlaps(Actor* a){ return false; }

Exit::Exit(int startX, int startY, StudentWorld* src): Actor(IID_EXIT, startX, startY, src, right, 1, 'X'){
    
}

void Exit::doSomething(){}

Pit::Pit(int startX, int startY, StudentWorld* src): Actor(IID_PIT, startX, startY, src, right, 0, 'O'){}

void Pit::doSomething(){}

Flame::Flame(int startX, int startY, StudentWorld* src, int dir): Actor(IID_FLAME, startX, startY, src, dir, 0, 'F'){
}

void Flame::doSomething(){}

Vomit::Vomit(int startX, int startY, StudentWorld* src, int dir): Actor(IID_VOMIT, startX, startY, src, dir, 0, 'B'){
}

void Vomit::doSomething(){}

VaccineGoodie::VaccineGoodie(int startX, int startY, StudentWorld* src): Actor(IID_VACCINE_GOODIE, startX, startY, src, right, 1, 'V'){}

void VaccineGoodie::doSomething(){}

GasCanGoodie::GasCanGoodie(int startX, int startY, StudentWorld* src): Actor(IID_GAS_CAN_GOODIE, startX, startY, src, right, 1, 'G'){}

void GasCanGoodie::doSomething(){}

LandmineGoodie::LandmineGoodie(int startX, int startY, StudentWorld* src): Actor(IID_LANDMINE_GOODIE, startX, startY, src, right, 1, 'L'){}

void LandmineGoodie::doSomething(){}

Landmine::Landmine(int startX, int startY, StudentWorld* src): Actor(IID_LANDMINE, startX, startY, src, right, 1, 'M'){}

void Landmine::doSomething(){}

DumbZombie::DumbZombie(int startX, int startY, StudentWorld* src): Actor(IID_ZOMBIE, startX, startY, src, right, 0, 'D'){}

void DumbZombie::doSomething(){}

SmartZombie::SmartZombie(int startX, int startY, StudentWorld* src): Actor(IID_ZOMBIE, startX, startY, src, right, 0, 'S'){}

void SmartZombie::doSomething(){}

Citizen::Citizen(int startX, int startY, StudentWorld* src): Actor(IID_CITIZEN, startX, startY, src, right, 0, 'C'){}

void Citizen::doSomething(){}









