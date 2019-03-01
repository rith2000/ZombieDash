#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"

#include <string>
#include <list>
#include <iostream>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp



StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    p1 = nullptr;
    numCitizens = 0;
    LevelComplete = false;
    isCleanedUp = false;
    isGameAlreadyWon = false;
    
}

StudentWorld::~StudentWorld()
{
    if (!isCleanedUp) {
         cleanUp();
    }
}

int StudentWorld::init()
{
    numCitizens = 0;
    Level level(assetPath());
    LevelComplete = false;
    oss2 << "level07"  /*<< getLevel() <<*/ ".txt"; //CHANGE L8R
   
    Level::LoadResult result = level.loadLevel(oss2.str());
//    if (getLevel() == 2) {
//        if (!isGameAlreadyWon) {
//            isGameAlreadyWon = true;
//            return GWSTATUS_PLAYER_WON;
//        }
//    }
    if (result == Level::load_fail_file_not_found) {
        if (!isGameAlreadyWon) {
            isGameAlreadyWon = true;
            return GWSTATUS_PLAYER_WON;
        }
    } else if (result == Level::load_fail_bad_format){
        return GWSTATUS_LEVEL_ERROR;
    } else {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            for (int y = 0; y < LEVEL_HEIGHT; y++) {
                Level::MazeEntry m = level.getContentsOf(x, y);
                if (m == Level::player) {
                    p1 = new Penelope(SPRITE_WIDTH * 1,SPRITE_HEIGHT * 13, this);
                }else if (m == Level::wall) {
                    actors.push_back(new Wall(SPRITE_WIDTH * x,SPRITE_HEIGHT * y, this));
                } else if (m == Level::exit) {
                    actors.push_back(new Exit(SPRITE_WIDTH * x,SPRITE_HEIGHT * y, this));
                } else if (m == Level::pit) {
                    actors.push_back(new Pit(SPRITE_WIDTH * x,SPRITE_HEIGHT * y, this));
                } else if (m == Level::smart_zombie) {
                    actors.push_back(new SmartZombie(SPRITE_WIDTH * x,SPRITE_HEIGHT * y, this));
                    
                } else if (m == Level::dumb_zombie) {
                    actors.push_back(new DumbZombie(SPRITE_WIDTH * x,SPRITE_HEIGHT * y, this));
                    
                } else if (m == Level::citizen) {
                    actors.push_back(new Citizen(SPRITE_WIDTH * x,SPRITE_HEIGHT * y, this));
                    numCitizens++;
                } else if (m == Level::vaccine_goodie) {
                    actors.push_back(new VaccineGoodie(SPRITE_WIDTH * x,SPRITE_HEIGHT * y, this));
                    
                } else if (m == Level::gas_can_goodie) {
                    actors.push_back(new GasCanGoodie(SPRITE_WIDTH * x,SPRITE_HEIGHT * y, this));
                    
                } else if (m == Level::landmine_goodie) {
                    actors.push_back(new LandmineGoodie(SPRITE_WIDTH * x,SPRITE_HEIGHT * y, this));
                    
                }
            }
        }
        
        
    }
    cout << "num of citizens: " << numCitizens << endl;
    
        oss.str("");
        oss2.str("");    
    
    
        return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    list<Actor*>::iterator itr;
    itr = actors.begin();
    for (; itr != actors.end(); itr++) {
        if ((*itr)->isAlive()) {
            (*itr)->doSomething();
            if (!p1->isAlive()){
                cout << "i ain't" << endl;
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }else if (LevelComplete){
                playSound(SOUND_LEVEL_FINISHED);
                return GWSTATUS_FINISHED_LEVEL;;
            }
        }
    }
    p1->doSomething();
    
    list<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end();) {
        if (!(*it)->isAlive()) {
            cout << "we deleting yall" << endl;
            delete *it;
            *it = nullptr;
            it = actors.erase(it);
        } else
            it++;

    }
    
    
    stringstream s;
    
    
    s.fill('0');
     s << "Score: ";
    if (getScore() > 0)
        s << setw(6);
   
    s << getScore() << " Level: " << getLevel();
    s <<  " Lives: " << getLives() << " Vaccines: " << p1->getVaccineCount() << " Flames: " << p1->getFlameCount() << " Mines: " << p1->getLandMineCount() << " Infected: " << p1->getInfectionCount();
    
    setGameStatText(s.str());
    
    //decLives();
    return GWSTATUS_CONTINUE_GAME; //change back to died later
}

void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        cout << "we deleting yall 2" << endl;
        delete (*it);
        *it = nullptr;
    }
    actors.clear();

    //only delete if gameAlreadyWon is true
    if (!isGameAlreadyWon) {
        cout << "we deleting yall 3" << endl;
        delete p1;
        p1 = nullptr;
    }
    
    isCleanedUp = true;

}


int StudentWorld::getInt(string s){
    return (s[1] - '0') + 10*(s[0] - '0');
}



double StudentWorld::distToPenelope(Actor* a){
    return sqrt(pow(a->getX() - p1->getX(),2) + pow(a->getY() - p1->getY(),2));
}
double StudentWorld::distToZombie(Actor * a){
    int min = -1;
    list<Actor*>::iterator it;
    it = actors.begin();
    double dist;
    for (; it != actors.end(); it++) {
        dist = sqrt(pow(a->getX() - (*it)->getX(),2) + pow(a->getY() - (*it)->getY(),2));
        if (min == -1 || dist < min ) {
            min = dist;
        }
    }
    
    return min;
    
}

// Return true if there is a living zombie or Penelope, otherwise false.
// If true, otherX, otherY, and distance will be set to the location and
// distance of the one nearest to (x,y), and isThreat will be set to true
// if it's a zombie, false if a Penelope.
bool StudentWorld::locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) {
    if (p1 != nullptr && (p1->isAlive() || locateNearestCitizenThreat(x, y, otherX, otherY, distance))) {
        double distP = sqrt(pow(x - p1->getX(),2) + pow(y - p1->getY(),2));
        if (distance == -1 || distP < distance) {
            otherX = p1->getX();
            otherY = p1->getY();
            distance = distP;
            isThreat = false;
        } else
            isThreat = true;
        return true;
    }

    return false;
}

// Return true if there is a living zombie, false otherwise.  If true,
// otherX, otherY and distance will be set to the location and distance
// of the one nearest to (x,y).
bool StudentWorld::locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) {
    list<Actor*>::iterator it;
    distance = -1;
    it = actors.begin();
    bool b = false;
    for (; it != actors.end(); it++) {
        if ((*it)->isAlive() && (*it)->threatensCitizens()) {
            b = true;
            double dist = sqrt(pow(x - (*it)->getX(),2) + pow(y - (*it)->getY(),2));
            if (distance == -1 || dist < distance){
                distance = dist;
                otherX = (*it)->getX();
                otherY = (*it)->getY();
                //cout << "zombie at " << x << ", " << y << endl;
            }
            
        }
    }
    return b;
}

bool StudentWorld::overlapsWithA(Actor* a, Actor *b){
    return (pow(a->getX() - b->getX(), 2) + pow(a->getY() - b->getY(), 2) <= 100);
}

bool StudentWorld::overlapsWithA(int x, int y, int otherX, int otherY){
    return (pow(x - otherX, 2) + pow(y - otherY, 2) <= 100);
}

void StudentWorld::addActor(Actor* a){
    actors.push_back(a);
}

bool StudentWorld::isAgentMovementBlockedAt(Actor*a, double x, double y) {
    
    list<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        if ((*it)->isAlive() && (*it)->blocksMovement() && (a->getX()!= (*it)->getX() || a->getY() != (*it)->getY()))
             if (bounds(x - (*it)->getX(), y - (*it)->getY()))
                 return true;
    }
    if (a->getX()!= p1->getX() || a->getY() != p1->getY())
        if (bounds(x - p1->getX(), y - p1->getY()))
            return true;
    return false;
}

bool StudentWorld::isZombieVomitTriggerAt(double x, double y){
    
    list<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        if ( (*it)->isAlive() && (*it)->triggersZombieVomit())
            if (bounds(x - (*it)->getX(), y - (*it)->getY()))
                return true;
    }
    return bounds(x - p1->getX(), y - p1->getY());
}

// Return true if there is a living human, otherwise false.  If true,
// otherX, otherY, and distance will be set to the location and distance
// of the human nearest to (x,y).
bool StudentWorld::locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance){
    list<Actor*>::iterator it;
    distance = -1;
    it = actors.begin();
    bool b = false;
    for (; it != actors.end(); it++) {
        if ((*it)->isAlive() && (*it)->triggersZombieVomit()) {
            b = true;
            double dist = sqrt(pow(x - (*it)->getX(),2) + pow(y - (*it)->getY(),2));
            if (distance == -1 || dist < distance){
                distance = dist;
                otherX = (*it)->getX();
                otherY = (*it)->getY();
            }
            
        }
    }
    double dist = sqrt(pow(x - p1->getX(),2) + pow(y - p1->getY(),2));
    if (distance == -1 || dist < distance) {
        distance = dist;
        otherX = p1->getX();
        otherY = p1->getY();
    }
    cout << "distance: " << distance << endl;
    return b;
}


// For each actor overlapping a, activate a if appropriate.
void StudentWorld::activateOnAppropriateActors(Actor* a){
    list<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        if ((*it)->getWorld()!=nullptr && (*it)->isAlive() && overlaps(a->getX() - (*it)->getX(), a->getY() - (*it)->getY())){
            if (a->infects())
                (*it)->beVomitedOnIfAppropriate();
            else if (a->kills()){
                (*it)->dieByFallOrBurnIfAppropriate();
            } else if (a->exits())
                (*it)->useExitIfAppropriate();
            else if ( (*it) != nullptr && (*it)->isAlive() && (*it)->triggersOnlyActiveLandmines() && a->blowsup())
                a->explode();
            
            //WHEN OBJECT IS DELETED BUT IS STILL BEING ACCESSED
        }
        
    }
    if (p1 != nullptr && p1->isAlive() && overlaps(a->getX() - (p1)->getX(), a->getY() - (p1)->getY())) {
        if (a->infects()){
            (p1)->beVomitedOnIfAppropriate();
        }else if (a->kills()){
            cout << "we wanna dieee" << endl;
            p1->dieByFallOrBurnIfAppropriate();
        }else if (a->exits()){
            cout << "exits" << endl;
            p1->useExitIfAppropriate();
        }else if (a->blowsup())
            a->explode();
        
        
    }
}



bool StudentWorld::isFlameBlockedAt(double x, double y){
    list<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        if ((*it)->isAlive() && (*it)->blocksFlame())
            if (bounds(x - (*it)->getX(), y - (*it)->getY()))
                return true;
    }
    return false;
    
}

bool StudentWorld::overlaps(double x, double y){
    return pow(abs(x), 2) + pow(abs(y), 2) <= 100;
}

bool StudentWorld::bounds(double x, double y){
    return abs(x) < SPRITE_WIDTH && abs(y) < SPRITE_HEIGHT;
}


void StudentWorld::recordCitizenGone(){
    numCitizens--;
    cout << "num of citizens minus: " << numCitizens << endl;
}

void StudentWorld::recordLevelFinishedIfAllCitizensGone(){
    if (numCitizens == 0) {
        cout << "level complete" << endl;
        LevelComplete = true;
    }
}

void StudentWorld::addFlames(int x){
    p1->addFlameCount(x);
}
void StudentWorld::addVaccines(int x){
    p1->addVaccineCount(x);
}
void StudentWorld::addLandmines(int x){
    p1->addLandMineCount(x);
}

void StudentWorld::activate(Goodie *a){
    if (overlaps(a->getX() - (p1)->getX(), a->getY() - (p1)->getY()))
            p1->pickUpGoodieIfAppropriate(a);
}

bool StudentWorld::overLaps(double x, double y){
    list<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        if (overlaps(x - (*it)->getX(), y - (*it)->getY())) {
            cout << "thisX: " << x;
            cout << "thisY: " << y;
            cout << "x: " << (*it)->getX();
            cout << "y: " << (*it)->getY();
            cout << "r u sure" << endl;
            return true;
        }
    }
    return false;
}


//just deleted an object, we trying to call on it twice in do Something
//how to fix it? make sure you're not calling on it twice
