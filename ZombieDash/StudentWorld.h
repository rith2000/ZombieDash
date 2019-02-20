#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include <string>
#include <sstream>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    void getLevelString(std::string levelFile);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isInWall(int x, int y);

private:
    std::vector<Actor*> actors;
    std::vector<Wall*> walls;
    std::stringstream oss;
    Penelope* p1;
    int getInt(std::string s);
//    Actor* test;
};

#endif // STUDENTWORLD_H_
