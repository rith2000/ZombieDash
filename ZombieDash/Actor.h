#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor: public GraphObject {
public:
    Actor(int imageID, int startX, int startY, StudentWorld* src, int dir, int depth, char ID);
    virtual ~Actor();
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    virtual bool overlaps(Actor* a);
    char getID();
    
    
private:
    StudentWorld* m_world;
    char m_ID;
};

//class AliveActor: public Actor {
//public:
//    AliveActor(int imageID, int startX, int startY, StudentWorld* src);
//    virtual void doSomething() = 0;
//    bool isDead();
//    bool setDead();
//    
//    
//private:
//    bool isAlive;
//};

class Penelope: public Actor {
public:
    Penelope(int startX, int startY, StudentWorld* src);
    virtual void doSomething();

};

class Wall: public Actor {
public:
    Wall(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    virtual bool overlaps(Actor* a);
    
};

class Exit: public Actor {
public:
    Exit(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class Pit: public Actor {
public:
    Pit(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class Flame: public Actor {
public:
    Flame(int startX, int startY, StudentWorld* src, int dir);
    virtual void doSomething();
    
};

class Vomit: public Actor {
public:
    Vomit(int startX, int startY, StudentWorld* src, int dir);
    virtual void doSomething();
    
};

class VaccineGoodie: public Actor {
public:
    VaccineGoodie(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class GasCanGoodie: public Actor {
public:
    GasCanGoodie(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class LandmineGoodie: public Actor {
public:
    LandmineGoodie(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class Landmine: public Actor {
public:
    Landmine(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class DumbZombie: public Actor {
public:
    DumbZombie(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class SmartZombie: public Actor {
public:
    SmartZombie(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class Citizen: public Actor {
public:
    Citizen(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};




// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
