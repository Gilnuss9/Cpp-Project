#include <iostream>
#include <vector>
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/User.h"
using namespace std;

BaseAction::BaseAction(): errorMsg(),status() {
    errorMsg= ("") ;
    status = {PENDING};
}

ActionStatus BaseAction::getStatus() const {
    return this-> status;
}

void BaseAction ::complete(){
    this->status = {COMPLETED} ;
}
void BaseAction ::error(const std::string& errorMsg){
    this->status = {ERROR};
    this->errorMsg = errorMsg ;
}
string BaseAction::getErrorMsg() const {
    return this-> errorMsg;
}

string BaseAction::enumToString() const{
    string num = to_string(getStatus());
    if(num == "0")
        return "PENDING";
    else if(num == "1")
        return "COMPLETED";
    else
        return "ERROR";
}

string CreateUser::getType() {
    return "CreateUser";
}

string ChangeActiveUser::getType() {
    return "ChangeActiveUser";
}

string DeleteUser::getType() {
    return "DeleteUser";
}

string DuplicateUser::getType() {
    return "DuplicateUser";
}

string PrintContentList::getType() {
    return "PrintContentList";
}


string PrintWatchHistory::getType() {
    return "PrintWatchHistory";
}

string Watch::getType() {
    return "Watch";
}

string PrintActionsLog::getType() {
    return "PrintActionsLog";
}

string Exit::getType() {
    return "Exit";
}

void BaseAction::setErrorMsgAndStatus(string errorMsg, ActionStatus actionStatus) {
    this->errorMsg = errorMsg;
    this->status = actionStatus;
}



void CreateUser::act(Session &sess)  {//we check in the function here which kind of user we need to create, then we send it to the session to create the user and it to the usermap.
    vector<string> command = sess.getCommand();
    bool isSucceded = true;
    if(command[2] == "len")
        isSucceded = sess.addLengthRecommenderUser(command[1]);
    else if(command[2] == "rer")
        isSucceded = sess.addRerunRecommenderUser(command[1]);
    else if(command[2] == "gen")
        isSucceded = sess.addGenreRecommenderUser(command[1]);
    else {
        error("invalid recommendation");
        cout<< getErrorMsg()<<endl ;
        }
    if(!isSucceded) {
        error("username is already taken");
        cout<< getErrorMsg()<<endl  ;
    }
    else complete() ;

}
string CreateUser::toString() const {
    if (getStatus() == ERROR) {
        string s = "CreateUser " + enumToString()+" "+ getErrorMsg();
        return s;
    } else {
        string s = "Createuser " + enumToString();
        return s;
    }
}


void ChangeActiveUser::act(Session &sess){//we send the name of the user to the session to check if he exist.
    vector<string> command = sess.getCommand();
    if (sess.CurrentUsername()==command[1]) {
        error("You are already ActiveUser");
        cout << getErrorMsg()<<endl ;
    }
    if (sess.ChangeThisActiveUser(command[1]) == true){
        complete() ;

    }
    else {
        error("None Existing User");
        cout << getErrorMsg()<<endl ;
    }
}
string ChangeActiveUser::toString() const {
    if (getStatus() == ERROR) {
        return "ChangeActiveUser " + enumToString() +""+ getErrorMsg();
    } else {
       return "ChangeActiveUser " + enumToString();
    }
}

void DeleteUser::act(Session &sess) {//we check if we have a user with that name in the session.
    vector<string> command = sess.getCommand();
    bool isSucceded = sess.DeleteThisUser(command[1]);;
    if(isSucceded)
    {
        complete();
    }
    else
    {
        error("user doesn't exist");
        cout << getErrorMsg()<<endl ;
    }
}
string DeleteUser::toString() const {
    if (getStatus() == ERROR) {
        return "DeleteUser " + enumToString() +" "+ getErrorMsg();
    } else {
       return "DeleteUser " + enumToString();
    }
}

void DuplicateUser::act(Session &sess) {//we send the names to the session to search in the user map if one of them exist or not.
    vector<string> command = sess.getCommand();
    string statusMassage = sess.DuplicateThisUser(command[1], command[2]);
    if(statusMassage == "succeed")
    {
        complete();
    }
    else
    {
        error(statusMassage);
        cout << getErrorMsg()<<endl ;
    }
}
string DuplicateUser::toString() const {
    if (getStatus() == ERROR) {
        return "DuplicateUser " + enumToString() +" "+ getErrorMsg();
    } else {
       return "DuplicateUser " + enumToString();
    }
}

void PrintContentList::act(Session &sess) {//going through the content list and printing them in the order they appear in the vector
    vector<Watchable*> thisSessionContent = sess.getContent() ;
    if (thisSessionContent.size() == 0)
    {
        error("No content to print") ;
    }
    for (size_t i = 1 ;i<thisSessionContent.size() ; i++)
    {
        string s = to_string(i) + ". " ;
        s = s + thisSessionContent[i]->toString() ;
        cout<<s<<endl ;
    }
    complete() ;
}
string PrintContentList::toString() const {
    if (getStatus() == ERROR) {
        return "PrintContentList " + enumToString() +" "+ getErrorMsg();
    } else {
       return "PrintContentList " + enumToString();
    }
}

void PrintWatchHistory::act(Session &sess) {//we ask for the user history, then we print the name and each of the watchable content in his history list.
    User* u = sess.getActiveUser() ;
    vector<Watchable*> watchHistory = u->get_history() ;
    string name = u->getName() ;
    cout<<"Watch history for " + name <<endl ;
    for(size_t i = 0 ; i<watchHistory.size() ; i++){
        int j = i ;
        string s = to_string(j+1) + "." ;
        s = s + " " + watchHistory[i]->getName()+" "+watchHistory[i]->getSeasonCode();
        cout<<s<<endl ;
    }
    complete() ;

}

string PrintWatchHistory::toString() const {
    if (getStatus() == ERROR) {
        return "PrintWatchHistory " + enumToString() +" "+ getErrorMsg();
    } else {
       return  "PrintWatchHistory " + enumToString();
    }
}

void Watch::act(Session &sess) {//we send the id to the session, if we receive it's existence we make sure we play it, and add it to the user watch history.
    vector<string> command = sess.getCommand();
    vector<Watchable*> content = sess.getContent();
    size_t i =stol(command[1]) ;
    if (i > content.size()){
        error("Content ID doesn't exist") ;
        cout<< "404 File not found"<<endl ;
    }
    string watch = content[i]->getName()+""+content[i]->getSeasonCode();
    cout<<"watching " + watch<<endl ;
    Watchable* w = content[i];
    User* u = sess.getActiveUser() ;
    u->watched(w);
    complete() ;
}
string Watch::toString() const {
    if (getStatus() == ERROR) {
        return "Watch " + enumToString() +" "+ getErrorMsg();
    } else {
       return "Watch " + enumToString();
    }
}

void PrintActionsLog::act(Session &sess) {//go through the actions log and make sure we print it one by one.
    vector<BaseAction*> log = sess.getActionsLog();
    for(size_t i=0; i<log.size(); i++)
    {
        cout << log[i]->toString() << endl;
    }
    complete();
}
string PrintActionsLog::toString() const {
    if (getStatus() == ERROR) {
        return "PrintActionsLog" + enumToString() + getErrorMsg();
    } else {
        return "PrintActionsLog " + enumToString();
    }
}

void Exit::act(Session &sess) {
    complete() ;
}
string Exit::toString() const {
    if (getStatus() == ERROR) {
        return "Exit " + enumToString() +" "+ getErrorMsg();
    } else {
        return "Exit " + enumToString();
    }
}

BaseAction::~BaseAction() {}
