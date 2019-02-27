#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
class Goodie;

//ADD CONST TO FUNCTION NAMES
class Actor: public GraphObject {
public:
    Actor(int imageID, int startX, int startY, StudentWorld* src, int dir, int depth);
    virtual ~Actor();
    virtual void doSomething();
    StudentWorld* getWorld();
    virtual bool overlaps(Actor* a);
    void setDead();
    bool isAlive();
    virtual bool shouldBlockWalls();
    virtual bool shouldBlockAgents();
    virtual bool isAWall();
    virtual bool isAnAgent();
    virtual bool paralysis();
    int getTickCount();
    void incTick();

    
    // If this is an activated object, perform its effect on a (e.g., for an
    // Exit have a use the exit).
    virtual void activateIfAppropriate(Actor* a);

    // If this object uses exits, use the exit.
    virtual void useExitIfAppropriate();
    
    // Does this object block agent movement?
    virtual bool blocksMovement() const;
    
        // Is this object a threat to citizens?
        virtual bool threatensCitizens() const;
    
        // Does this object trigger citizens to follow it or flee it?
        virtual bool triggersCitizens() const;
    
        // Can this object cause a zombie to vomit?
        virtual bool triggersZombieVomit() const;
    
    
        // Does this object block flames?
        virtual bool blocksFlame() const;
    
    //    // If this object can be infected by vomit, get infected.
    //    virtual void beVomitedOnIfAppropriate();
//
//    // If this object can die by falling into a pit or burning, die.
//    virtual void dieByFallOrBurnIfAppropriate();
//

//
//    // If this object can pick up goodies, pick up g
//    virtual void pickUpGoodieIfAppropriate(Goodie* g);
//


//
//    // Does this object trigger landmines only when they're active?
//    virtual bool triggersOnlyActiveLandmines() const;
//

//

    
    
private:
    StudentWorld* m_world;
    bool m_isAlive;
    int tickCount;
};

class Wall: public Actor {
public:
    Wall(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    virtual bool overlaps(Actor* a);
    virtual bool isAWall();
    virtual bool blocksMovement() const;
    
};

//ActivatingObject
class ActivatingObject: public Actor{
public:
    ActivatingObject(int imageID, int startX, int startY, StudentWorld* src, int dir, int depth);
    virtual void doSomething();

    
};

class Exit: public ActivatingObject {
public:
    Exit(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class Pit: public ActivatingObject {
public:
    Pit(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class Flame: public ActivatingObject {
public:
    Flame(int startX, int startY, StudentWorld* src, int dir);
    virtual void doSomething();
    
};

class Vomit: public ActivatingObject {
public:
    Vomit(int startX, int startY, StudentWorld* src, int dir);
    virtual void doSomething();
    
};

class Landmine: public ActivatingObject {
public:
    Landmine(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    virtual bool blocksFlame() const;
    
};

class Goodie: public ActivatingObject {
public:
    Goodie(int imageID, int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    virtual bool blocksFlame() const;
    
};

class VaccineGoodie: public Goodie {
public:
    VaccineGoodie(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class GasCanGoodie: public Goodie {
public:
    GasCanGoodie(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};

class LandmineGoodie: public Goodie {
public:
    LandmineGoodie(int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    
};






//AGENT
class Agent: public Actor{
public:
    Agent(int imageID, int startX, int startY, StudentWorld* src);
    virtual void doSomething();
    virtual bool blocksMovement() const;
    virtual bool moveDirBy(int dir, int amt);
    virtual bool paralysis();
    virtual int closerToTarget( int otherX, int otherY);
    virtual bool blocksFlame() const;
    
private:
    
};

    //HUMAN

    class Human: public Agent{
    public:
        Human(int imageID, int startX, int startY, StudentWorld* src);
        virtual void doSomething();
        bool getInfectionStatus();
        int getInfectionCount();
        void incInfectionCount();
        void setInfectionStatus(bool b);
        virtual void useExitIfAppropriate();
        virtual bool triggersZombieVomit() const;
        
    private:
        int infectionCount;
        bool isInfected;
    };


        //PENELOPE
        class Penelope: public Human {
        public:
            Penelope(int startX, int startY, StudentWorld* src);
            virtual void doSomething();
            virtual bool triggersCitizens() const;
            virtual bool paralysis();
            
        private:
            int landmineCount, flameCount, vaccineCount;
        };

        //CITIZEN
        class Citizen: public Human {
        public:
            Citizen(int startX, int startY, StudentWorld* src);
            virtual void doSomething();
            
            
        };

    //ZOMBIE

    class Zombie: public Agent{
    public:
        Zombie(int imageID, int startX, int startY, StudentWorld* src);
        virtual void doSomething();
        virtual bool threatensCitizens() const;
        void getVomitCoords(double & x, double & y) const;
        virtual void MvPlan() = 0;
        void setmvplanDist(int x);
        int getmvplanDist();
    private:
        int mvplanDist;
    };

        //DUMBZOMBIE
        class DumbZombie: public Zombie {
        public:
            DumbZombie(int startX, int startY, StudentWorld* src);
            virtual void doSomething();
            virtual void MvPlan();
            
        };

        //SMARTZOMBIE
        class SmartZombie: public Zombie {
        public:
            SmartZombie(int startX, int startY, StudentWorld* src);
            virtual void doSomething();
            virtual void MvPlan();
            
        };




// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
