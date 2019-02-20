#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"

#include <string>
#include <vector>
#include <iostream>
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>
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
    getLevelString("level01.txt");
    int level_x, level_y;
    string s = oss.str();
    
    
    for (int i = 0; i < s.size() - 4; i+=5){
        level_x = getInt(s.substr(i + 1,2));
        level_y = getInt(s.substr(i + 3,2));
        if (s[i] == '@'){
            p1 = new Penelope(SPRITE_WIDTH * level_x,SPRITE_HEIGHT * level_y, this);
        } else if (s[i] == '#'){
            walls.push_back(new Wall(SPRITE_WIDTH * level_x,SPRITE_HEIGHT * level_y, this));
        }
        
    }
        return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if (p1 != nullptr)
        p1->doSomething();
    
    vector<Wall*>::iterator it;
    it = walls.begin();
    for (; it != walls.end(); it++) {
        (*it)->doSomething();
    }
 
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
    return GWSTATUS_CONTINUE_GAME;; //change back to died later
}

void StudentWorld::cleanUp()
{
//    vector<Actor*>::iterator it;
//    it = actors.begin();
//    for (; it != actors.end(); it++) {
//        delete *it;
//    }
    
    delete p1;
//    delete test;
    
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
        
        oss.fill('0');
        int x = 0;
        Level::MazeEntry ge; //not changing from wall
        for (; x < LEVEL_WIDTH; x++){
            int y = 0;
            for (; y < LEVEL_HEIGHT; y++){
                ge = lev.getContentsOf(x,y); // level_x=5, level_y=10
                switch (ge) // so x=80 and y=160
                {
                    case Level::empty:
                        //cout << "Location " << x << "," << y << " is empty" << endl;
                        break;
                    case Level::wall:
                        cout << "Location " << x << "," << y << " holds a Wall" << endl;
                        oss << "#";
                        oss << setw(2) << x << setw(2) << y;
                        break;
                    case Level::player:
                        cout << "Location " << x << "," << y << " is where Penelope starts" << endl;
                        //y = 7;
                        oss << "@";
                        oss << setw(2) << x << setw(2) << y;
                        
                        string s = oss.str();
                        cout << s << endl;
                        break;
//                    case Level::smart_zombie:
//                        cout << "Location " << x << "," << y << " starts with a smart zombie" << endl;
//                        break;
//                    case Level::dumb_zombie:
//                        cout << "Location " << x << "," << y << " starts with a dumb zombie" << endl;
//                        break;
//
//                    case Level::exit:
//                        cout << "Location " << x << "," << y << " is where an exit is" << endl;
//                        break;
//                    case Level::pit:
//                        cout << "Location " << x << "," << y << " has a pit in the ground" << endl;
//                        break;
                        // etc…
                        
                }
        
            }
        }
    }
}

int StudentWorld::getInt(string s){
    return (s[1] - '0') + 10*(s[0] - '0');
}
