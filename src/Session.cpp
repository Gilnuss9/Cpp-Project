#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include "../include/User.h"
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/json.hpp"
#include "../include/Watchable.h"

using namespace std;
using namespace nlohmann;


/*
 * We start by reading the json file, for each file in the json we create a watchable, depend if it's episode or movie.
 * after we create the content list, we create a new vector that will hold all of our actions, at 1st he's empty.
 * we create a new user named "DEFAULT", he has "length" recommendation algorithm and we make sure he enters the userMap.
 * we set the activeUser to be "DEFAULT"
 * start the command vector so we can read from it later on.
 */
Session::Session(const std::string &configFilePath): content() , actionsLog() , userMap() , activeUser() , command() {
    ifstream i(configFilePath);
    json j;
    i >> j;
    vector<string> garbage ={"comedy"} ;
    Watchable *garbage2 = new Movie(0,"Shawarma",100,garbage) ;
    content.push_back(garbage2) ;
    size_t moviesize = j.at("movies").size() ;
    for(size_t id=0; id<moviesize; id++)
    {
        json movies = j.at("movies");
        string name = movies.at(id).at("name");
        int length = movies.at(id).at("length");
        vector<string> tags = movies.at(id).at("tags");
        Watchable *Mptr = new Movie(id+1, name,length,tags);
        content.push_back(Mptr) ;

    }
    long episode_id = moviesize+1 ;// the id of the 1st tv episode, right after the movies
    json tvSeriesJson = j.at("tv_series");
    for(unsigned long list=0; list<tvSeriesJson.size(); list++)// a for "loop, to make sure i'm going through all the tv series.
    {
        json tvSeries = j.at("tv_series").at(list);
        vector<string> tags = tvSeries.at("tags") ;
        string name = tvSeries.at("name") ;
        int length = tvSeries.at("episode_length") ;
        int seasons = tvSeries.at("seasons").size() ;
        for(int i = 0 ; i<seasons ; i++)//we go through each season of the tvshow
        {
            int num_episodes = tvSeries.at("seasons").at(i) ;//we need an upper limit of how many episodes we have for each season.
            for (int j = 1 ; j<= num_episodes; j++ )//we need to make sure we are making a "Episode class" for each episde.
            {
                Watchable *e = new Episode(episode_id , name ,length,i+1 ,j ,tags) ;
                content.push_back(e) ;
                episode_id++;//each episode we need to have a specific id, and we need to make sure we go through all the episodes.
            }

        }
    }
    LengthRecommenderUser* defaultUser = new LengthRecommenderUser("DEFAULT");
    pair<string,User*> defaultPair ("DEFAULT",defaultUser);
    userMap.insert(defaultPair);
    activeUser = userMap["DEFAULT"];
}

//Copy Constructor
/*
 * copy constructor copies each indivudal object from the former session and create a new one:
 * we create a new content vector that will hold new watchable's - who are identical to the former one.
 * we make sure we have a new users, with the exact same prefrences and watch history - only now it points to the new content log.
 * we make sure the userMap has the same Users - now using the new users.
 * we create new actions that will be identical to the old ones and enter them in the same order they were in the former session.
 */
Session::Session(const Session &aSession):content() , actionsLog(),userMap(),activeUser(),command(aSession.getCommand()) {
    copy(aSession) ;
}
void Session::copy(const Session &aSession) {
    vector<Watchable*> newContent = vector<Watchable*>() ;
    for(size_t i = 0 ; i<aSession.getContent().size() ; i++)
    {
        if(aSession.getContent()[i]->getType() == "Episode")
        {
            Watchable* tmpWatchable = aSession.getContent()[i];
            Watchable *copyWatchable = new Episode(tmpWatchable->getId(),tmpWatchable->getName(),tmpWatchable->getLength(),tmpWatchable->getSeason(),tmpWatchable->getEpisode(),tmpWatchable->getTags());
            newContent.push_back(copyWatchable) ;
        }
        else
        {
            Watchable* tmpWatchable1 = aSession.getContent()[i];
            Watchable *copyWatchable = new Movie(tmpWatchable1->getId(),tmpWatchable1->getName(),tmpWatchable1->getLength(),tmpWatchable1->getTags()) ;
            newContent.push_back(copyWatchable) ;
        }
    }
    this->content = newContent;

    vector<BaseAction*> newActionLog = vector<BaseAction*>() ;
    for(size_t i = 0 ; i<aSession.getActionsLog().size() ; i++)
    {
        if(aSession.getActionsLog()[i]->getType() == "CreateUser")
        {
            BaseAction* tmpAction = aSession.getActionsLog()[i];
            BaseAction *copyAction = new CreateUser();
            copyAction->setErrorMsgAndStatus(tmpAction->getErrorMsg(),tmpAction->getStatus());
            newActionLog.push_back(copyAction) ;
        }

        else if(aSession.getActionsLog()[i]->getType() == "ChangeActiveUser")
        {
            BaseAction* tmpAction = aSession.getActionsLog()[i];
            BaseAction *copyAction = new ChangeActiveUser();
            copyAction->setErrorMsgAndStatus(tmpAction->getErrorMsg(),tmpAction->getStatus());
            newActionLog.push_back(copyAction) ;
        }

        else if(aSession.getActionsLog()[i]->getType() == "DeleteUser")
        {
            BaseAction* tmpAction = aSession.getActionsLog()[i];
            BaseAction *copyAction = new DeleteUser();
            copyAction->setErrorMsgAndStatus(tmpAction->getErrorMsg(),tmpAction->getStatus());
            newActionLog.push_back(copyAction) ;
        }

        else if(aSession.getActionsLog()[i]->getType() == "DuplicateUser")
        {
            BaseAction* tmpAction = aSession.getActionsLog()[i];
            BaseAction *copyAction = new DuplicateUser();
            copyAction->setErrorMsgAndStatus(tmpAction->getErrorMsg(),tmpAction->getStatus());
            newActionLog.push_back(copyAction) ;
        }

        else if(aSession.getActionsLog()[i]->getType() == "PrintContentList")
        {
            BaseAction* tmpAction = aSession.getActionsLog()[i];
            BaseAction *copyAction = new PrintContentList();
            copyAction->setErrorMsgAndStatus(tmpAction->getErrorMsg(),tmpAction->getStatus());
            newActionLog.push_back(copyAction) ;
        }

        else if(aSession.getActionsLog()[i]->getType() == "PrintWatchHistory")
        {
            BaseAction* tmpAction = aSession.getActionsLog()[i];
            BaseAction *copyAction = new PrintWatchHistory();
            copyAction->setErrorMsgAndStatus(tmpAction->getErrorMsg(),tmpAction->getStatus());
            newActionLog.push_back(copyAction) ;
        }

        else if(aSession.getActionsLog()[i]->getType() == "Watch")
        {
            BaseAction* tmpAction = aSession.getActionsLog()[i];
            BaseAction *copyAction = new Watch();
            copyAction->setErrorMsgAndStatus(tmpAction->getErrorMsg(),tmpAction->getStatus());
            newActionLog.push_back(copyAction) ;
        }

        else if(aSession.getActionsLog()[i]->getType() == "PrintActionsLog")
        {
            BaseAction* tmpAction = aSession.getActionsLog()[i];
            BaseAction *copyAction = new PrintActionsLog();
            copyAction->setErrorMsgAndStatus(tmpAction->getErrorMsg(),tmpAction->getStatus());
            newActionLog.push_back(copyAction) ;
        }

        else if(aSession.getActionsLog()[i]->getType() == "Exit")
        {
            BaseAction* tmpAction = aSession.getActionsLog()[i];
            BaseAction *copyAction = new Exit();
            copyAction->setErrorMsgAndStatus(tmpAction->getErrorMsg(),tmpAction->getStatus());
            newActionLog.push_back(copyAction) ;
        }

    }
    this->actionsLog = newActionLog;

    unordered_map<string,User*> newUserMap = unordered_map<string,User*>();
    for (auto& it: aSession.getUserMap()) { //
        // Do stuff
        vector<Watchable*> oldHistory = it.second->get_history();
        vector<Watchable*> newHistory = vector<Watchable *>();
        for (size_t i = 0; i<oldHistory.size(); i++) {
            long id = oldHistory[i]->getId();
            newHistory.push_back(content[id]);
        }
        string type = it.second->getType();
        string name = it.first;
        if (type == "len") {
            User *newUser = new LengthRecommenderUser(name);
            newUser->set_history(newHistory);
            pair<string, User *> newUserPair = pair<string, User *>(name, newUser);
            newUserMap.insert(newUserPair);
        } else if (type == "rer") {
            User *newUser = new RerunRecommenderUser(name);
            newUser->set_history(newHistory);
            pair<string, User *> newUserPair = pair<string, User *>(name, newUser);
            newUserMap.insert(newUserPair);
        } else {
            User *newUser = new GenreRecommenderUser(name);
            newUser->set_history(newHistory);
            pair<string, User *> newUserPair = pair<string, User *>(name, newUser);
            newUserMap.insert(newUserPair);
        }
    }

    this->userMap = newUserMap ;
    string newSessionActiveUser = aSession.getActiveUser()->getName() ;
    this->activeUser = newUserMap[newSessionActiveUser] ;


    }
//copy assignment
Session& Session::operator=(const Session& rhs) {
    if (&rhs != this) {
        clean();
        copy(rhs);
    }
    return *this;
}
//move const
Session::Session(Session &&other): content(), actionsLog(),userMap(), activeUser(), command() {
    if(this != &other)
    {
        this->content = move(other.content);
        for (size_t i = 0; i < other.content.size(); i++) {
            if (other.content[i] != nullptr)
                delete (other.content[i]);
        }
        this->actionsLog = move(other.actionsLog);
        for (size_t i = 0; i < other.actionsLog.size(); i++) {
            if (other.actionsLog[i] != nullptr)
                delete (other.actionsLog[i]);
        }
        this->userMap = move(other.userMap);
        this->activeUser = move(other.activeUser);
        this->activeUser = nullptr;
    }
}
//move assignment
Session& Session::operator=(Session &&other){
    if(this != &other)
    {
        this->clean();
        this->content = move(other.content);
        this->actionsLog = move(other.actionsLog);
        this->userMap = move(other.userMap) ;
        this->activeUser = move(other.activeUser);
        other.activeUser = nullptr;
        return *this ;
    }
    return *this ;
}

User* Session::getActiveUser() {
    return this->activeUser ;
}

vector<Watchable*> Session::getContent() const {
    return this->content ;
}

User* Session::getActiveUser() const {
    return this->activeUser ;
}


unordered_map<string,User*> Session::getUserMap() const {
    return this->userMap ;
}

vector<string> Session::getCommand() const {
    return this->command ;
}

vector<BaseAction*> Session::getActionsLog() const {
    return this->actionsLog ;
}

bool Session::addLengthRecommenderUser(string& name)  {//create a new user from the Length kind, then we enter it to the userMap
    if(userMap.find(name)==userMap.end())
    {
        LengthRecommenderUser* user = new LengthRecommenderUser(name);
        pair<string,User*> newPair (name,user);
        userMap.insert(newPair);
        return true;
    }
    else
    {
        return false;
    }
}

bool Session::addRerunRecommenderUser(string& name)  {//create a new user from the reRun kind, then we enter it to the userMap
    if(userMap.find(name)==userMap.end())
    {
        RerunRecommenderUser* user = new RerunRecommenderUser(name);
        pair<string,User*> newPair (name,user);
        userMap.insert(newPair);
        return true;
    }
    else
    {
        return  false ;
    }
}

bool Session::addGenreRecommenderUser(string& name){//create a new user from the Genre kind, then we enter it to the userMap
    if(userMap.find(name)==userMap.end())
    {
        GenreRecommenderUser* user = new GenreRecommenderUser(name);
        pair<string,User*> newPair (name,user);
        userMap.insert(newPair);
        return true;
    }
    else
    {
        return false ;
    }
}


vector<string> Session::createCommandVector(string command)//we create a vector that will hold what the actions that the user who is using the software is writing.
{
    istringstream iss(command);
    vector<string> results;
    for(string s; iss >> s;)
        results.push_back(s);
    return results;
}

string Session:: CurrentUsername() {
    return activeUser->getName() ;
}
bool Session::ChangeThisActiveUser(string& name) {
     if(userMap.find(name)==userMap.end()) {
        return false;
    }
    else {
        this->activeUser = userMap[name];
        return true;
    }
}

bool Session::DeleteThisUser(string &name) {
    if(userMap.find(name)==userMap.end()) {
        return false;
    }
    else {
        delete(userMap[name]) ;
        userMap.erase(name);
        return true;
    }
}

string Session::DuplicateThisUser(string &name, string &newName) {//we look for the old user, if we find him we look for the new user name, if we don't find the new username in our list we create a new user from the same kind and we copy it's watch history to a new vector so they will not have the same history.
    if(userMap.find(name)==userMap.end()) {
        return "username doesn't exist";
    }
    else if(userMap.find(newName)!=userMap.end())
    {
        return "new username already exist";
    }
    else {
        User* user = userMap[name];
        string type = user->getType();
        if(type == "gen")
        {
            GenreRecommenderUser* newUser = new GenreRecommenderUser(newName);
            pair<string,User*> newPair (newName,newUser);
            userMap.insert(newPair);
            newUser->set_history(user->get_history());
            return "succeed";
        }
        else if(type == "len")
        {
            LengthRecommenderUser* newUser = new LengthRecommenderUser(newName);
            pair<string,User*> newPair (newName,newUser);
            userMap.insert(newPair);
            newUser->set_history(user->get_history());
            return "succeed";
        }
        else
        {
            RerunRecommenderUser* newUser = new RerunRecommenderUser(newName);
            pair<string,User*> newPair (newName,newUser);
            userMap.insert(newPair);
            newUser->set_history(user->get_history());
            return "succeed";
        }
    }
}

void Session::clean() {
    for (size_t i = 0; i < content.size(); i++) {
        if (content[i] != nullptr)
            delete (content[i]);
    }
    for (size_t i = 0; i < actionsLog.size(); i++) {
        if (actionsLog[i] != nullptr)
            delete (actionsLog[i]);
    }
    for (auto it = userMap.begin(); it != userMap.end(); it++) {//auto it = userMap.begin(); it != userMap.end()
        delete (it->second);
    }
}

    Session::~Session()//the destructor calls for the clean function who make sure we delete each user we have, we delete the content and the action log.
    {
        clean();
    }

    void Session::start() {//we recive from the user the command, and we send it to the right function according to what he asked for.
        cout << "SPLFLIX is now on!" << endl;
        string command1 = "";

        while (command1 != "exit") {
            getline(cin, command1);

            this->command = createCommandVector(command1);

            if (this->command[0] == "createuser") {
                string name = this->command[1];
                string recommendCode = this->command[2];
                CreateUser *action = new CreateUser();
                action->act(*this);
                actionsLog.push_back(action);
            }

            if (this->command[0] == "changeuser") {
                string name = this->command[1];
                ChangeActiveUser *action = new ChangeActiveUser();
                action->act(*this);
                actionsLog.push_back(action);
            }

            if (this->command[0] == "log") {
                PrintActionsLog *action = new PrintActionsLog();
                action->act(*this);
                actionsLog.push_back(action);
            }

            if (this->command[0] == "deleteuser") {
                DeleteUser *action = new DeleteUser();
                action->act(*this);
                actionsLog.push_back(action);
            }

            if (this->command[0] == "dupuser") {
                DuplicateUser *action = new DuplicateUser();
                action->act(*this);
                actionsLog.push_back(action);
            }

            if (this->command[0] == "watch") {//the 1st watch is responsiable for letting the user watch what he asked, later it enters a while loop if he wanted to watch the reccomended show, it will exit it only if he says "n" and want's to end this "watch" Session and he will wait for a new command(it will not end the Session itself!). each time he clicks "y" it will be counted as a whole new action.
                Watch *action = new Watch();
                action->act(*this);
                actionsLog.push_back(action);
                long d = stol(this->command[1]);
                Watchable *watch = content[d];
                Watchable *recommendation = watch->getNextWatchable(*this);
                long nextid = recommendation->getId();
                string newCommand = "";
                cout << "We recommend watching " + recommendation->getName() + " " + recommendation->getSeasonCode() +
                        "continue watching? [y/n]" << endl;
                getline(cin, newCommand);
                while (newCommand != "n") {
                    string s = "watch " + to_string(nextid);
                    this->command = createCommandVector(s);
                    Watch *action1 = new Watch();
                    action1->act(*this);
                    actionsLog.push_back(action1);
                    Watchable *watch = content[nextid];
                    Watchable *recommendation1 = watch->getNextWatchable(*this);
                    cout << "We recommend watching " + recommendation1->getName() + " " +
                            recommendation1->getSeasonCode() + "continue watching? [y/n]" << endl;
                    nextid = recommendation1->getId();
                    getline(cin, newCommand);
                }
            }


            if (this->command[0] == "watchhist") {
                PrintWatchHistory *action = new PrintWatchHistory();
                action->act(*this);
                actionsLog.push_back(action);
            }
            if (this->command[0] == "content") {
                PrintContentList *action = new PrintContentList();
                action->act(*this);
                actionsLog.push_back(action);

            }

            if (this->command[0] == "exit") {
                Exit *action = new Exit();
                action->act(*this);
                actionsLog.push_back(action);
            }
        }
    }




