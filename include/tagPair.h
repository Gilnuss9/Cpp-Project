#ifndef tagPair_H_
#define tagPair_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "../include/Watchable.h"
#include "../include/Session.h"
//#include "../include/User.h"
#include "../include/Action.h"

//tagPair is a class defined to help the user compare the most common "tags" in his watch history. you have 2 private fields so it can know how to compare between them.

using namespace std;

class Session;
class User;

class tagPair{
public:
    tagPair(string tag,int counter) ;
    virtual ~tagPair() ;
    virtual void raiseCounter() ;
    int compareTag(tagPair tag) ;
    virtual int getCounter() ;
    virtual string getTag();
private:
    string tag ;
    int counter ;
};

#endif
