#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include "../include/tagPair.h"
#include "../include/User.h"

using namespace std;

class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    Session(const Session &aSession) ;
    Session& operator=(const Session& rhs);
    Session(Session &&other);
    void copy(const Session &aSession) ;
    Session& operator=(Session &&other);
    virtual ~Session();
    void start();
    virtual string CurrentUsername() ;
    virtual User* getActiveUser();
    virtual bool addLengthRecommenderUser(string& name)   ;
    virtual bool addRerunRecommenderUser(string& name)   ;
    virtual bool addGenreRecommenderUser(string& name) ;
    virtual vector<Watchable*> getContent() const  ;
    virtual unordered_map<string,User*> getUserMap() const  ;
    virtual User* getActiveUser() const  ;
    virtual vector<string> getCommand()  const ;
    virtual vector<BaseAction*> getActionsLog()  const ;
    virtual bool ChangeThisActiveUser(string& name) ;
    virtual bool DeleteThisUser(string& name);
    virtual string DuplicateThisUser(string &name, string &newName);
    virtual void clean();
    vector<string> createCommandVector(string command);
private:
    vector<Watchable*> content;
    vector<BaseAction*> actionsLog;
    unordered_map<string,User*> userMap;
    User* activeUser;
    vector<string> command;


};

#endif
