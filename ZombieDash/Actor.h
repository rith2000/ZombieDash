#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor: public GraphObject {
public:
    Actor(int imageID, int startX, int startY, StudentWorld* src);
    virtual ~Actor();
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    
    
private:
    StudentWorld* m_world;
};

class Penelope: public Actor {
public:
    Penelope(int startX, int startY, StudentWorld* src);
    virtual ~Penelope();
    virtual void doSomething();

};

class Wall: public Actor {
public:
    Wall(int startX, int startY, StudentWorld* src);
    virtual ~Wall();
    virtual void doSomething();
    
};


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
