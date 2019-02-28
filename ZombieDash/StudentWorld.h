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
    //void getLevelString(std::string levelFile);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void changeLevel();
    bool overlapsWithA(Actor* a, Actor *b);
    double distToPenelope(Actor* a);
    double distToZombie(Actor * a);
    bool overlapsWithA(int x, int y, int otherX, int otherY);
    
    
    // Add an actor to the world.
    void addActor(Actor* a);
    
    // Record that one more citizen on the current level is gone (exited,
    // died, or turned into a zombie).
    void recordCitizenGone();
    
    // Indicate that the player has finished the level if all citizens
    // are gone.
    void recordLevelFinishedIfAllCitizensGone();
    
    // For each actor overlapping a, activate a if appropriate.
    
    void activateOnAppropriateActors(Actor* a);
    
    // Is an agent blocked from moving to the indicated location? //is supposed to be const???
    bool isAgentMovementBlockedAt(Actor* a, double x, double y) ;
    
    // Is creation of a flame blocked at the indicated location?
    bool isFlameBlockedAt(double x, double y) ;
    
    // Is there something at the indicated location that might cause a
    // zombie to vomit (i.e., a human)?
    bool isZombieVomitTriggerAt(double x, double y) ;
    
    // Return true if there is a living human, otherwise false.  If true,
    // otherX, otherY, and distance will be set to the location and distance
    // of the human nearest to (x,y).
    bool locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance);
    
    // Return true if there is a living zombie or Penelope, otherwise false.
    // If true, otherX, otherY, and distance will be set to the location and
    // distance of the one nearest to (x,y), and isThreat will be set to true
    // if it's a zombie, false if a Penelope.
    bool locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) ;
    
    // Return true if there is a living zombie, false otherwise.  If true,
    // otherX, otherY and distance will be set to the location and distance
    // of the one nearest to (x,y).
    bool locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) ;
    
    void activate(Goodie* a);
    
    void addFlames(int x);
    void addVaccines(int x);
    void addLandmines(int x);
    

private:
    std::vector<Actor*> actors;
    //std::vector<Wall*> walls;
    std::stringstream oss, oss2;
    Penelope* p1;
    //std::vector<Exit*> exits;
    int getInt(std::string s);
    bool overlaps(double x, double y);
    int numCitizens;
    bool LevelComplete;
    bool isCleanedUp;
    bool isGameAlreadyWon;
    
//    Actor* test;
};

#endif // STUDENTWORLD_H_
