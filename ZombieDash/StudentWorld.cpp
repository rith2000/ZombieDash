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
    
    
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    oss2 << "level0"  << getLevel() << ".txt";
    getLevelString(oss2.str());
    int level_x, level_y;
    string s = oss.str();
    
    
    for (int i = 0; i < s.size() - 4; i+=5){
        level_x = getInt(s.substr(i + 1,2));
        level_y = getInt(s.substr(i + 3,2));
        if (s[i] == '@'){
            
            p1 = new Penelope(SPRITE_WIDTH * 1,SPRITE_HEIGHT * 13, this);
        } else if (s[i] == '#'){
            actors.push_back(new Wall(SPRITE_WIDTH * level_x,SPRITE_HEIGHT * level_y, this));
        } else if (s[i] == 'X'){
            actors.push_back(new Exit(SPRITE_WIDTH * level_x,SPRITE_HEIGHT * level_y, this));
        } else if (s[i] == 'O'){
            actors.push_back(new Pit(SPRITE_WIDTH * level_x,SPRITE_HEIGHT * level_y, this));
        } else if (s[i] == 'S'){
            actors.push_back(new SmartZombie(SPRITE_WIDTH * level_x,SPRITE_HEIGHT * level_y, this));
        } else if (s[i] == 'D'){
            actors.push_back(new DumbZombie(SPRITE_WIDTH * level_x,SPRITE_HEIGHT * level_y, this));
        } else if (s[i] == 'C'){
            actors.push_back(new Citizen(SPRITE_WIDTH * level_x,SPRITE_HEIGHT * level_y, this));
        } else if (s[i] == 'V'){
            actors.push_back(new VaccineGoodie(SPRITE_WIDTH * level_x,SPRITE_HEIGHT * level_y, this));
        } else if (s[i] == 'G'){
            actors.push_back(new GasCanGoodie(SPRITE_WIDTH * level_x,SPRITE_HEIGHT * level_y, this));
        } else if (s[i] == 'L'){
            actors.push_back(new LandmineGoodie(SPRITE_WIDTH * level_x,SPRITE_HEIGHT * level_y, this));
        }
        
    }
    
        oss.str("");
        oss2.str("");
        cout << "emptystring: " << oss.str() << endl;
        return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    vector<Actor*>::iterator itr;
    itr = actors.begin();
    for (; itr != actors.end(); itr++) {
        if ((*itr)->isAlive()) {
            (*itr)->doSomething();
            if (!p1->isAlive())
                return GWSTATUS_PLAYER_DIED;
            if (overlapsWithA(*itr, p1)) {
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
        
    }
    
    p1->doSomething();
    
    vector<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end();) {
        if (!(*it)->isAlive()) {
            delete *it;
            it = actors.erase(it);
        }else
            it++;
        
    }
    //for
    stringstream s;
    s.fill(' ');
    s << "Score: " << setw(2) << "idk yet lolzzz";
    
    
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

    delete p1;
}

void StudentWorld::getLevelString(string levelFile)
{
    Level lev(assetPath());
    //string levelFile = "level01.txt";
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result == Level::load_fail_file_not_found)
        cerr << "Cannot find level01.txt data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "Your level was improperly formatted" << endl;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level" << endl;
        cerr << "Level: " << getLevel() << endl;
        
        oss.fill('0');
        int x = 0;
        Level::MazeEntry ge; //not changing from wall
        for (; x < LEVEL_WIDTH; x++){
            int y = 0;
            for (; y < LEVEL_HEIGHT; y++){
                ge = lev.getContentsOf(x,y); // level_x=5, level_y=10
                switch (ge) // so x=80 and y=160
                {
                    case Level::landmine_goodie:
                        cout << "Location " << x << "," << y << " starts with a landmine goodie" << endl;
                        oss << "L";
                        oss << setw(2) << x << setw(2) << y;
                        break;
                    case Level::gas_can_goodie:
                        cout << "Location " << x << "," << y << " starts with a gas can goodie" << endl;
                        oss << "G";
                        oss << setw(2) << x << setw(2) << y;
                        break;
                    case Level::vaccine_goodie:
                        cout << "Location " << x << "," << y << " starts with a vaccine goodie" << endl;
                        oss << "V";
                        oss << setw(2) << x << setw(2) << y;
                        break;
                    case Level::citizen:
                        cout << "Location " << x << "," << y << " starts with a citizen" << endl;
                        oss << "C";
                        oss << setw(2) << x << setw(2) << y;
                        break;
                    case Level::dumb_zombie:
                        cout << "Location " << x << "," << y << " starts with a dumb zombie" << endl;
                        oss << "D";
                        oss << setw(2) << x << setw(2) << y;
                        break;
                    case Level::smart_zombie:
                        cout << "Location " << x << "," << y << " starts with a smart zombie" << endl;
                        oss << "S";
                        oss << setw(2) << x << setw(2) << y;
                        break;
                    case Level::exit:
                        cout << "Location " << x << "," << y << " is where an exit is" << endl;
                        oss << "X";
                        oss << setw(2) << x << setw(2) << y;
                        break;
                    case Level::pit:
                        cout << "Location " << x << "," << y << " has a pit in the ground" << endl;
                        oss << "O";
                        oss << setw(2) << x << setw(2) << y;
                        break;
                    case Level::empty:
                        break;
                    case Level::wall:
                        cout << "Location " << x << "," << y << " holds a Wall" << endl;
                        oss << "#";
                        oss << setw(2) << x << setw(2) << y;
                        break;
                    case Level::player:
                        cout << "Location " << x << "," << y << " is where Penelope starts" << endl;
                        oss << "@";
                        oss << setw(2) << x << setw(2) << y;
                        break;
                }
                
            }
        }
    }
}

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
    int startX, startY;
    for (; it != actors.end(); it++) {
        if ((*it)->blocksMovement() && (a->getX()!= (*it)->getX() || a->getY() != (*it)->getY())) {
            startX = (*it)->getX();
            startY = (*it)->getY();
            if (abs(x - startX) < SPRITE_WIDTH && abs(y - startY) < SPRITE_HEIGHT){
                return true;
            }
        }
    }
    //how to check that something doesn't block itself
    startX = p1->getX();
    startY = p1->getY();
    if (a->getX()!= p1->getX() || a->getY() != p1->getY())
        if (abs(x - startX) < SPRITE_WIDTH && abs(y - startY) < SPRITE_HEIGHT)
            return true;

    return false;
}

bool StudentWorld::isZombieVomitTriggerAt(double x, double y, int dir){
    vector<Actor*>::iterator it;
    it = actors.begin();
    int X, Y;
    for (; it != actors.end(); it++) {
        if ((*it)->triggersZombieVomit() && (*it)->isAlive()) {
            X = (*it)->getX();
            Y = (*it)->getY();
            if ((dir == Actor::right && X == x + 1) || (dir == Actor::left && X == x - 1) || (dir == Actor::up && Y == y + 1) || (dir == Actor::down && Y == y -1))
                return true;
           
        }
    }
    return false;
        
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
    return b;
}


// For each actor overlapping a, activate a if appropriate.
void StudentWorld::activateOnAppropriateActors(Actor* a){
    vector<Actor*>::iterator it;
    it = actors.begin();
    for (; it != actors.end(); it++) {
        if (!(*it)->blocksFlame() && (*it)->isAlive() && overlapsWithA(a, *it))
            (*it)->setDead();
    }
}

// Is creation of a flame blocked at the indicated location?
bool StudentWorld::isFlameBlockedAt(double x, double y) {
//    vector<Actor*>::iterator it;
//    it = actors.begin();
//    int startX, startY;
//    for (; it != actors.end(); it++) {
//        if ((*it)->blocksMovement() && (a->getX()!= (*it)->getX() || a->getY() != (*it)->getY())) {
//            startX = (*it)->getX();
//            startY = (*it)->getY();
//            if (abs(x - startX) < SPRITE_WIDTH && abs(y - startY) < SPRITE_HEIGHT){
//                return true;
//            }
//        }
//    }
    return 0;
}
