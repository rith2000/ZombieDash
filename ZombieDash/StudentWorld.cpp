#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"

#include <string>
#include <vector>
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
    if (getLevel() == 2) {
        if (!isGameAlreadyWon) {
            isGameAlreadyWon = true;
            return GWSTATUS_PLAYER_WON;
        }
    }
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
    vector<Actor*>::iterator itr;
    itr = actors.begin();
    for (; itr != actors.end(); itr++) {
        if ((*itr)->isAlive()) {
            (*itr)->doSomething();
            if (!p1->isAlive()){
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }else if (LevelComplete){
                playSound(SOUND_LEVEL_FINISHED);
                //return GWSTATUS_PLAYER_WON;
            
                return GWSTATUS_FINISHED_LEVEL;;
            }
        }
    }
    p1->doSomething();
    
    vector<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end();) {
        if (!(*it)->isAlive()) {
            if ((*it)->blowsup())
                cout << "landmine: " << endl;
            delete *it;
            it = actors.erase(it);
        }else
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
    vector<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        delete (*it);
    }
    actors.clear();

    //only delete if gameAlreadyWon is true
    if (!isGameAlreadyWon) {
        delete p1;
    }
    
    isCleanedUp = true;

}

//void StudentWorld::getLevelString(string levelFile)
//{
//    Level lev(assetPath());
//    //string levelFile = "level01.txt";
//    Level::LoadResult result = lev.loadLevel(levelFile);
//    if (result == Level::load_fail_file_not_found)
//        cerr << "Cannot find level01.txt data file" << endl;
//    else if (result == Level::load_fail_bad_format)
//        cerr << "Your level was improperly formatted" << endl;
//    else if (result == Level::load_success)
//    {
//        cerr << "Successfully loaded level" << endl;
//        cerr << "Level: " << getLevel() << endl;
//
//        oss.fill('0');
//        int x = 0;
//        Level::MazeEntry ge; //not changing from wall
//        for (; x < LEVEL_WIDTH; x++){
//            int y = 0;
//            for (; y < LEVEL_HEIGHT; y++){
//                ge = lev.getContentsOf(x,y); // level_x=5, level_y=10
//                switch (ge) // so x=80 and y=160
//                {
//                    case Level::landmine_goodie:
//                        cout << "Location " << x << "," << y << " starts with a landmine goodie" << endl;
//                        oss << "L";
//                        oss << setw(2) << x << setw(2) << y;
//                        break;
//                    case Level::gas_can_goodie:
//                        cout << "Location " << x << "," << y << " starts with a gas can goodie" << endl;
//                        oss << "G";
//                        oss << setw(2) << x << setw(2) << y;
//                        break;
//                    case Level::vaccine_goodie:
//                        cout << "Location " << x << "," << y << " starts with a vaccine goodie" << endl;
//                        oss << "V";
//                        oss << setw(2) << x << setw(2) << y;
//                        break;
//                    case Level::citizen:
//                        cout << "Location " << x << "," << y << " starts with a citizen" << endl;
//                        oss << "C";
//                        oss << setw(2) << x << setw(2) << y;
//                        break;
//                    case Level::dumb_zombie:
//                        cout << "Location " << x << "," << y << " starts with a dumb zombie" << endl;
//                        oss << "D";
//                        oss << setw(2) << x << setw(2) << y;
//                        break;
//                    case Level::smart_zombie:
//                        cout << "Location " << x << "," << y << " starts with a smart zombie" << endl;
//                        oss << "S";
//                        oss << setw(2) << x << setw(2) << y;
//                        break;
//                    case Level::exit:
//                        cout << "Location " << x << "," << y << " is where an exit is" << endl;
//                        oss << "X";
//                        oss << setw(2) << x << setw(2) << y;
//                        break;
//                    case Level::pit:
//                        cout << "Location " << x << "," << y << " has a pit in the ground" << endl;
//                        oss << "O";
//                        oss << setw(2) << x << setw(2) << y;
//                        break;
//                    case Level::empty:
//                        break;
//                    case Level::wall:
//                        cout << "Location " << x << "," << y << " holds a Wall" << endl;
//                        oss << "#";
//                        oss << setw(2) << x << setw(2) << y;
//                        break;
//                    case Level::player:
//                        cout << "Location " << x << "," << y << " is where Penelope starts" << endl;
//                        oss << "@";
//                        oss << setw(2) << x << setw(2) << y;
//                        break;
//                }
//
//            }
//        }
//    }
//}

int StudentWorld::getInt(string s){
    return (s[1] - '0') + 10*(s[0] - '0');
}



double StudentWorld::distToPenelope(Actor* a){
    return sqrt(pow(a->getX() - p1->getX(),2) + pow(a->getY() - p1->getY(),2));
}
double StudentWorld::distToZombie(Actor * a){
    int min = -1;
    vector<Actor*>::iterator it;
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
    if (p1->isAlive() || locateNearestCitizenThreat(x, y, otherX, otherY, distance)) {
        double distP = sqrt(pow(x - p1->getX(),2) + pow(y - p1->getY(),2));
        if (distP < distance) {
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
    vector<Actor*>::iterator it;
    distance = -1;
    it = actors.begin();
    bool b = false;
    for (; it != actors.end(); it++) {
        if ((*it)->threatensCitizens() && (*it)->isAlive()) {
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
    
    vector<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        if ((*it)->blocksMovement() && (a->getX()!= (*it)->getX() || a->getY() != (*it)->getY()))
             if (overlaps(x - (*it)->getX(), y - (*it)->getY()))
                 return true;
    }
    if (a->getX()!= p1->getX() || a->getY() != p1->getY())
        if (overlaps(x - p1->getX(), y - p1->getY()))
            return true;
    return false;
}

bool StudentWorld::isZombieVomitTriggerAt(double x, double y){
    
    vector<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        if ((*it)->triggersZombieVomit() && (*it)->isAlive())
            if (overlaps(x - (*it)->getX(), y - (*it)->getY()))
                return true;
    }
    return overlaps(x - p1->getX(), y - p1->getY());
}

// Return true if there is a living human, otherwise false.  If true,
// otherX, otherY, and distance will be set to the location and distance
// of the human nearest to (x,y).
bool StudentWorld::locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance){
    vector<Actor*>::iterator it;
    distance = -1;
    it = actors.begin();
    bool b = false;
    for (; it != actors.end(); it++) {
        if ((*it)->triggersZombieVomit() && (*it)->isAlive()) {
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
    vector<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        if (overlaps(a->getX() - (*it)->getX(), a->getY() - (*it)->getY())){
            if (a->infects())
                (*it)->beVomitedOnIfAppropriate();
            if (a->kills())
                (*it)->dieByFallOrBurnIfAppropriate();
            if (a->exits()){
                (*it)->useExitIfAppropriate();
            } if (a->blowsup()){
                a->explode();
                cout << "exploseee" << endl;
                return;
            }
            
        }
        
    }
    if (overlaps(a->getX() - (p1)->getX(), a->getY() - (p1)->getY())) {
        if (a->infects())
            (p1)->beVomitedOnIfAppropriate();
        if (a->kills())
            p1->dieByFallOrBurnIfAppropriate();
        if (a->exits())
            p1->useExitIfAppropriate();
        if (a->blowsup()){
            a->explode();
            return;
        }
        
    }
}



bool StudentWorld::isFlameBlockedAt(double x, double y){
    vector<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        if ((*it)->blocksFlame())
            if (overlaps(x - (*it)->getX(), y - (*it)->getY()))
                return true;
    }
    return false;
    
}

bool StudentWorld::overlaps(double x, double y){
    return abs(x) < SPRITE_WIDTH && abs(y) < SPRITE_HEIGHT;
}

void StudentWorld::recordCitizenGone(){
    numCitizens--;
    cout << "num of citizens minus: " << numCitizens << endl;
}

void StudentWorld::recordLevelFinishedIfAllCitizensGone(){
    if (numCitizens <= 0) {
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

