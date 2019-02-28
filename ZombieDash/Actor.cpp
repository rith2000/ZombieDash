#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>


#include <iostream>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR
Actor::Actor(int imageID, int startX, int startY, StudentWorld* src, int dir, int depth): GraphObject(imageID, startX, startY, dir, depth){
    m_world = src;
    m_isAlive = true;
    tickCount = 0;
}

Actor::~Actor(){}

bool Actor::overlaps(Actor* a){
    return (pow(a->getX() - this->getX(), 2) + pow(a->getY() - this->getY(), 2) <= 100);
}

StudentWorld* Actor::getWorld(){
    return m_world;
}

void Actor::setDead(){
    m_isAlive = false;
}

bool Actor::isAlive(){
    return m_isAlive;
}

void Actor::doSomething(){
    if (!m_isAlive) {
        return;
    }
}


int Actor::getTickCount(){ return tickCount;}
void Actor::incTick(){ tickCount++;}
bool Actor::shouldBlockWalls(){ return false;}
bool Actor::shouldBlockAgents(){ return false;}
bool Actor::isAWall(){ return false;}
bool Actor::isAnAgent(){ return false;}
void Actor::activateIfAppropriate(Actor* a) { };
void Actor::useExitIfAppropriate(){};
bool Actor::blocksMovement() const{ return false;}
bool Actor::triggersCitizens() const { return false;}
bool Actor::threatensCitizens() const { return false;}
bool Actor::triggersZombieVomit() const { return false;}
bool Actor::paralysis() {return false; }
bool Actor::blocksFlame() const{ return false;}
void Actor::beVomitedOnIfAppropriate() { };
bool Actor::infects(){ return false;}


    


//WALL
Wall::Wall(int startX, int startY, StudentWorld* src): Actor(IID_WALL, startX, startY, src, right, 0){}

void Wall::doSomething(){}
bool Wall::overlaps(Actor* a){ return false; }
bool Wall::isAWall() { return true; }
bool Wall::blocksMovement() const{ return true;}
bool Wall::blocksFlame() const{ return true;}

//ACTIVATINGOBJECT
ActivatingObject::ActivatingObject(int imageID, int startX, int startY, StudentWorld* src, int dir, int depth): Actor(imageID, startX, startY, src, dir, depth){}
void ActivatingObject::doSomething(){ Actor::doSomething();}


//EXIT
Exit::Exit(int startX, int startY, StudentWorld* src): ActivatingObject(IID_EXIT, startX, startY, src, right, 1){}
void Exit::doSomething(){ActivatingObject::doSomething();}
bool Exit::blocksFlame() const{ return true;}

//PIT
Pit::Pit(int startX, int startY, StudentWorld* src): ActivatingObject(IID_PIT, startX, startY, src, right, 0){}
void Pit::doSomething(){ ActivatingObject::doSomething();}

//FLAME
Flame::Flame(int startX, int startY, StudentWorld* src, int dir): ActivatingObject(IID_FLAME, startX, startY, src, dir, 0){}
void Flame::doSomething(){
    ActivatingObject::doSomething();
    
    incTick();
    if (getTickCount() == 2) {
        setDead();
    }
    
    
}

//VOMIT
Vomit::Vomit(int startX, int startY, StudentWorld* src, int dir): ActivatingObject(IID_VOMIT, startX, startY, src, dir, 0){}

void Vomit::doSomething(){
    ActivatingObject::doSomething();
//    if (!isAlive())
//        return;
    incTick();
    if (getTickCount() == 2) {
        setDead();
    }
    getWorld()->activateOnAppropriateActors(this);
}
bool Vomit::infects(){ return true;}


//LANDMINE
Landmine::Landmine(int startX, int startY, StudentWorld* src): ActivatingObject(IID_LANDMINE, startX, startY, src, right, 1){}

void Landmine::doSomething(){ ActivatingObject::doSomething();}

//GOODIE
Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* src): ActivatingObject(imageID, startX, startY, src, right, 1){}
void Goodie::doSomething(){ActivatingObject::doSomething();}

//VACCINEGOODIE
VaccineGoodie::VaccineGoodie(int startX, int startY, StudentWorld* src): Goodie(IID_VACCINE_GOODIE, startX, startY, src){}

void VaccineGoodie::doSomething(){Goodie::doSomething();}

//GASCANGOODIE
GasCanGoodie::GasCanGoodie(int startX, int startY, StudentWorld* src): Goodie(IID_GAS_CAN_GOODIE, startX, startY, src){}

void GasCanGoodie::doSomething(){Goodie::doSomething();}

//LANDMINEGOODIE
LandmineGoodie::LandmineGoodie(int startX, int startY, StudentWorld* src): Goodie(IID_LANDMINE_GOODIE, startX, startY, src){}

void LandmineGoodie::doSomething(){Goodie::doSomething();}


//AGENTS

Agent::Agent(int imageID, int startX, int startY, StudentWorld* src): Actor(imageID, startX, startY, src, right, 0){}

void Agent::doSomething(){
    Actor::doSomething();
    if (paralysis()){
        incTick();
//        if (tickCount % 2 == 0)
//            return;
    }
    
}

bool Agent::moveDirBy(int dir, int amt){
    int amtx = 0, amty = 0;
    if (dir == right){
        amtx = amt;
    } else if (dir == left){
        amtx = -amt;
    } else if (dir == up){
        amty = amt;
    } else if (dir == down){
        amty = -amt;
    }
    
        if (!getWorld()->isAgentMovementBlockedAt(this, getX() + amtx, getY() + amty)) {
            setDirection(dir);
            moveTo(getX() + amtx, getY() + amty);
            return true;
        }
 
    return false;
    
}
bool Agent::blocksMovement() const{ return true;}
bool Agent::paralysis(){ return true;}

int Agent::closerToTarget(int otherX, int otherY){
    if (getX() == otherX){
        if (otherY > getY())
            setDirection(up);
        else
            setDirection(down);
    } else if (getY() == otherY){
        if (otherX > getX())
            setDirection(right);
        else
            setDirection(left);
    } else {
        
        int d1, d2;
        
        if (otherX > getX()){
            d1 = right;
        }else if (otherX < getX()){
            d1 = left;
        }
        
        if (otherY > getY()){
            d2 = up;
        }else if (otherY < getY()){
            d2 = down;
        }
        
        if (randInt(0,1))
            setDirection(d1);
        else
            setDirection(d2);
    }
    return getDirection();
        
}

//HUMAN

Human::Human(int imageID, int startX, int startY, StudentWorld* src): Agent(imageID, startX, startY, src){ infectionCount = 0; isInfected = false;}
void Human::doSomething(){Agent::doSomething();}
bool Human::getInfectionStatus(){return isInfected;}
int Human::getInfectionCount(){return infectionCount;}
void Human::incInfectionCount(){ infectionCount++;}
void Human::setInfectionStatus(bool b) { isInfected = b;}
void Human::useExitIfAppropriate() { };
bool Human::triggersZombieVomit() const { return true;}
void Human::beVomitedOnIfAppropriate() {isInfected = true; cout << "wtffff man " << endl;}

//CITIZEN
Citizen::Citizen(int startX, int startY, StudentWorld* src): Human(IID_CITIZEN, startX, startY, src){}

//probably not correct
void Citizen::doSomething(){
    Human::doSomething();
    if (getTickCount() % 2 == 0)
        return;
    if (getInfectionStatus()) {
        cout << "HELLLOOOO IS ANYONE THEREEE" <<
        endl;
        incInfectionCount();
        if (getInfectionCount() == 500) {
            setDead();
            getWorld()->playSound(SOUND_ZOMBIE_BORN);
            getWorld()->increaseScore(-1000);
            int a = randInt(0, 99);
            if (a < 70)
                getWorld()->addActor(new DumbZombie(getX(), getY(), getWorld()));
             else
                getWorld()->addActor(new SmartZombie(getX(), getY(), getWorld()));
            return;
        }
    }
    
    
    double x, y, distance;
    bool isThreat;
    getWorld()->locateNearestCitizenThreat(getX(), getY(), x, y, distance);
    
    //cout << "nearest zombie at: " << static_cast<int>(x) << ", " << static_cast<int>(y) << endl;
    getWorld()->locateNearestCitizenTrigger(getX(), getY(), x, y, distance, isThreat);
    if (!isThreat && distance <= 80) {
        closerToTarget(x, y);
        moveDirBy(getDirection(), 2);
        return;
    }
       
     else if (isThreat && distance <= 80){
        double dist1 = 0, dist2 = 0, dist3 = 0, dist4 = 0;
        if (!getWorld()->isAgentMovementBlockedAt(this, getX() + 2, getY()))
            getWorld()->locateNearestCitizenThreat(getX() + 2, getY(), x, y, dist1);
        else if (!getWorld()->isAgentMovementBlockedAt(this, getX() - 2, getY()))
            getWorld()->locateNearestCitizenThreat(getX() - 2, getY(), x, y, dist2);
        else if (!getWorld()->isAgentMovementBlockedAt(this, getX(), getY() + 2))
            getWorld()->locateNearestCitizenThreat(getX(), getY() + 2, x, y, dist3);
        else if (!getWorld()->isAgentMovementBlockedAt(this, getX(), getY() - 2))
            getWorld()->locateNearestCitizenThreat(getX(), getY() - 2, x, y, dist4);

        int max1 = max(dist1, dist2);
        int max2 = max(dist3, dist4);
        int totalMax = max(max1, max2);

        if (totalMax > distance){
            //go in that direction
            if (totalMax == dist1){
                setDirection(right);
                moveTo(getX() + 2, getY());
            }else if (totalMax == dist2){
                setDirection(left);
                moveTo(getX() - 2, getY());
            }else if (totalMax == dist3){
                setDirection(up);
                moveTo(getX(), getY() + 2);
            }else if (totalMax == dist4){
                setDirection(down);
                moveTo(getX(), getY() - 2);
            }
            return;
        }else
            return;
        
    }
    return;
 
}

//PENELOPE
//CHECK CAN'T SWITCH SIDE DIRECTIONS 
Penelope::Penelope(int startX, int startY, StudentWorld* src): Human(IID_PLAYER, startX, startY, src){
    vaccineCount = 3;
    flameCount = 3;
    landmineCount = 3;
}
bool Penelope::triggersCitizens() const{ return true;}
bool Penelope::paralysis(){ return false;}
void Penelope::beVomitedOnIfAppropriate(){ Human::beVomitedOnIfAppropriate();}
void Penelope::doSomething(){
    Human::doSomething();
    
    if (getInfectionStatus() == true) {
        cout << "hi " << endl;
        //DOESN'T GO IN HERE WHYYYY
        incInfectionCount();
        if (getInfectionCount() == 500) {
            //turn into zombie
            setDead();
            getWorld()->playSound(SOUND_PLAYER_DIE);
            return;
            //StudentWorld should detect she's dead and restart level
        }
    }
    
    int dir = 0;
    if (getWorld()->getKey(dir))
    {
        switch (dir)
        {
                //make sure she doesn't overlap w citizens and zombies
            case KEY_PRESS_LEFT:
                moveDirBy(left, 4);
                break;
            case KEY_PRESS_RIGHT:
                moveDirBy(right, 4);
                break;
            case KEY_PRESS_UP:
                moveDirBy(up, 4);
                break;
            case KEY_PRESS_DOWN:
                moveDirBy(down, 4);
                break;
            case KEY_PRESS_SPACE:
                if (flameCount > 0) {
                    flameCount--;
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    double px = 0, py = 0;
                    for (int i = 1; i <= 3; i++){
                        
                        if (getDirection() == right) {
                            px = i*SPRITE_WIDTH;
                        } else if (getDirection() == left) {
                            px = - i*SPRITE_WIDTH;
                        } else if (getDirection() == up) {
                            py = i*SPRITE_HEIGHT;
                        } else if (getDirection() == down) {
                            py = - i*SPRITE_HEIGHT;
                        }
                    
                        if (getWorld()->isFlameBlockedAt(getX() + px, getY() + py))
                            break;
                    getWorld()->addActor(new Flame(getX() + px, getY() + py, getWorld(), getDirection()));
                        
                    }
                }
                break;
            case KEY_PRESS_TAB:
                //add new landmine at penelope's location
                if (landmineCount > 0){
                    landmineCount--;
                getWorld()->addActor(new Landmine(getX(), getY(), getWorld()));
                }
                break;
            case KEY_PRESS_ENTER:
                if (vaccineCount > 0) {
                    vaccineCount--;
                    setInfectionStatus(false);
                }
                break;
            default:
                break;
        }
        
    }
    
}
//ZOMBIE
Zombie::Zombie(int imageID, int startX, int startY, StudentWorld* src): Agent(imageID, startX, startY, src){mvplanDist = 0;}
void Zombie::setmvplanDist(int x) { mvplanDist = x; }

int Zombie::getmvplanDist(){ return mvplanDist;}
void Zombie::doSomething(){
    
    Agent::doSomething();
    if (getTickCount() % 2 == 0)
        return;

    
    //EUCLIDEAN DISTANCE should be 10???
    bool b = false;
    if (getDirection() == right)
        b = getWorld()->isZombieVomitTriggerAt(getX() + 4, getY());
    else if (getDirection() == left)
        b = getWorld()->isZombieVomitTriggerAt(getX() - 4, getY());
    else if (getDirection() == up)
        b = getWorld()->isZombieVomitTriggerAt(getX(), getY() + 4);
    else if (getDirection() == down)
         b = getWorld()->isZombieVomitTriggerAt(getX(), getY() - 4);
    
    //cout << "b: " << b << endl;
    if (b){
        double vx, vy;
        getVomitCoords(vx, vy);
        int random = randInt(1, 3);
        if (random == 1){
            getWorld()->addActor(new Vomit(vx, vy, getWorld(), getDirection()));
            getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
        }
    }
    
    if (mvplanDist == 0) {
        MvPlan();
    }
    if (moveDirBy(getDirection(), 1))
        mvplanDist--;
    else
        mvplanDist = 0;
    
    
    
}
void Zombie::getVomitCoords(double &x, double &y) const{
    double xchange = 0;
    double ychange = 0;
    if (getDirection() == right)
        xchange = SPRITE_WIDTH;
     else if (getDirection() == left)
        xchange = -SPRITE_WIDTH;
     else if (getDirection() == up)
         ychange = SPRITE_HEIGHT;
    else if (getDirection() == down)
        ychange = -SPRITE_HEIGHT;
    
    x = getX() + xchange;
    y = getY() + ychange;
    
    
    
}
bool Zombie::threatensCitizens() const{ return true;}

//DUMB
DumbZombie::DumbZombie(int startX, int startY, StudentWorld* src): Zombie(IID_ZOMBIE, startX, startY, src){}
void DumbZombie::doSomething(){Zombie::doSomething();}
void DumbZombie::MvPlan(){
    setmvplanDist(randInt(3, 10));
    setDirection(randInt(0, 3) * 90);
}

//SMART
SmartZombie::SmartZombie(int startX, int startY, StudentWorld* src): Zombie(IID_ZOMBIE, startX, startY, src){}
void SmartZombie::doSomething(){Zombie::doSomething();}
void SmartZombie::MvPlan(){
    setmvplanDist(randInt(3, 10));
    double otherX, otherY, distance;
    getWorld()->locateNearestVomitTrigger(getX(), getY(), otherX, otherY, distance);
    if (distance > 80) {
        setDirection(randInt(0, 3) * 90);
    }else{
        closerToTarget(otherX, otherY);
    }
}









