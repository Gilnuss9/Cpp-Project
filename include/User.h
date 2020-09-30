#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"
//#include "../include/tagPair.h"

class Watchable;
class Session;

class User{
public:
    User(std::string  name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    void set_history(vector<Watchable*> newHistory) ;
    void watched(Watchable* w)  ;
    virtual int getAverageLength();
    virtual bool isExistsInHistory(long watchId);
    virtual int findMax(vector<tagPair> tagVector, int index);
    virtual void swap(vector<tagPair> &tagVector, int i, int j) const;
    virtual bool haveTag(string preferTag, Watchable* watch) ;
    virtual string getType() =0;
    virtual ~User() ;
protected:
    std::vector<Watchable*> history;
private:
    const std::string name;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual string getType();
private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual string getType();
private:
    int reRunindex ;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual string getType();
private:
};

#endif
