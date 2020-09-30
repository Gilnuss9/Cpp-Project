#include <iostream>
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"
using namespace std;


Watchable::Watchable(long id, int length, const vector<string>& tags): id(id),length(length),tags() {
    vector<string> Watchtags = vector<string>() ;
    for (size_t i = 0; i < tags.size(); i++) {
        string s = tags[i] ;
        Watchtags.push_back(s) ;
    }
    this->tags = Watchtags ;
}
vector<string> Watchable::getTags() const {
    return this->tags;
}
long Watchable::getId() {
    return this->id ;
}
int Watchable::getLength() const {
    return this->length ;
}

string Episode::getType()
{
    return "Episode";
}

string Movie::getType()
{
    return "Movie";
}


Watchable::~Watchable() {}



Movie::Movie(long id, const string& name, int length, const std::vector<std::string>& tags): Watchable(id,length,tags) , name(name) {}

string Movie::toString() const {
    string s = "" ;
    s = s + this->name ;
    s = s + " " + to_string(getLength()) + " minutes " + "[";
    vector<string> m = getTags() ;
    for(size_t i = 0; i < m.size(); i++){
        s = s+m[i] ;
        if(i!=m.size()-1){
            s= s+ "," ;
        }
        if(i == m.size()-1) {
            s = s + "]";
        }
    }
    m.clear() ;
    return s ;

}
string Movie::getSeasonCode() const{
    return "" ;
}

int Movie::getEpisode() {
    return 0 ;
}

int Movie::getSeason() {
    return 0;
}

string Movie::getName() const {
    return this->name ;
}

Watchable* Movie:: getNextWatchable(Session& ses) const{ // asks for the next watchable according to the user preferences
    vector<Watchable*> currentses= ses.getContent() ;
    User* user = ses.getActiveUser();
    return user->getRecommendation(ses);
}


Episode::Episode(long id, const string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags): Watchable(id,length,tags), seriesName(seriesName) , season(season), episode(episode), nextEpisodeId(id+1) {}


string Episode::toString() const {
    string s = "";
    s = s + this->seriesName ;
    s = s + getSeasonCode() ;
    s = s + " " +to_string(getLength()) + " minutes " + "[";
    vector<string> m = getTags() ;
    for(size_t i = 0; i < m.size(); i++){
        s = s+m[i] + ", " ;
        size_t j = m.size() - 1;
        if(i==j) {
            s = s +m[i] + "]";
        }
    }
    m.clear() ;
    return s ;
}
string Episode::getSeasonCode() const{
    string s = "" ;
    string f = to_string(this->season);
    s = s +" S" + f;
    string e = "" ;
    if (this->episode <= 9){
        e = e + "0" + to_string(this->episode) ;
    }
    else {
        e = e + to_string(this->episode) ;
    }
    s = s + "E" + e+" " ;
    return s ;
}

string Episode::getName() const {
    return this->seriesName ;
}

Watchable* Episode:: getNextWatchable(Session& ses) const { //1st he suggest the next episode of the series, if doens't exist, calls for user reccomendation algorithm
    vector<Watchable *> currentses = ses.getContent();
    size_t nextepisode = this->nextEpisodeId;
    if (nextepisode < currentses.size()) {
        if (this->seriesName == currentses[nextepisode]->getName())
            return currentses[nextepisode];
        else {
            User *user = ses.getActiveUser();
            return user->getRecommendation(ses);
        }
        //else //left to comlete user prefrences.
    }
    User *user = ses.getActiveUser();
    return user->getRecommendation(ses);

}
int Episode::getSeason() {
    return this->season ;
}
int Episode::getEpisode() {
    return this->episode ;
}

int Episode::getNextId() {
    return this->nextEpisodeId ;
}




