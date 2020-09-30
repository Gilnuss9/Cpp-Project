#include <iostream>
#include <cmath>
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/tagPair.h"
#include <unordered_map>
#include <utility>

using namespace std;


User::User(std::string name): history(vector<Watchable*>()), name(std::move(name)){}

string User::getName() const{
    return this->name ;
}

User:: ~User(){}

void User::set_history(vector<Watchable*> newHistory)
{
    this->history = std::move(newHistory);
}

void User::watched(Watchable *w) {
    this->history.push_back(w) ;
}

vector<Watchable*> User::get_history() const {
    return this->history ;
}



bool User::isExistsInHistory(long watchId) { //looks inside the user history in order to understand if the user watched this episode/movie.
    for (size_t j = 0; j < this->history.size(); ++j) {
        if(history[j]->getId() == watchId)
            return true;
    }
    return false;
}

int User::getAverageLength() {
    int sum = 0;
    for (size_t i = 0; i < this->history.size(); ++i) {
        sum = sum +history[i]->getLength();
    }
    return sum/history.size();
}

string LengthRecommenderUser::getType() {
    return "len";
}

string RerunRecommenderUser::getType() {
    return "rer";
}

string GenreRecommenderUser::getType() {
    return "gen";
}

LengthRecommenderUser::LengthRecommenderUser(const string &name): User(name) {}

Watchable* LengthRecommenderUser ::getRecommendation(Session &s) { //this function counts everysingle movie/episode in the user history, then calculate the avg so it can look into the content list and get the right recommendation according to the length recommendation
    int average = getAverageLength();
    long minDistance = 10000000000;
    vector<Watchable*> content = s.getContent();
    Watchable* output = content[1];

    for (size_t i = 1; i < content.size(); ++i) {
        if(!isExistsInHistory(content[i]->getId()))
        {
            int currentDistance = abs(content[i]->getLength()-average);
            if(currentDistance<minDistance)
            {
                minDistance = currentDistance;
                output = content[i];
            }
        }

    }
    return output;
}



RerunRecommenderUser::RerunRecommenderUser(const string &name): User(name),reRunindex(0) {}

Watchable* RerunRecommenderUser ::getRecommendation(Session &s) {
    Watchable* output = history[reRunindex] ;
    this->reRunindex= (reRunindex+1) ;
    return output ;
}

GenreRecommenderUser::GenreRecommenderUser(const string &name): User(name){}
Watchable* GenreRecommenderUser ::getRecommendation(Session &s) {//1st we create a vector with an object of tagPair, the object have the tag and how many times it appears in the history. and we sort that vector.
    vector<tagPair> tagList = vector<tagPair>();
    vector<string> Listoftags = vector<string>();
    vector<Watchable *> content = s.getContent();
    Watchable *output = content[1];
    for (auto & i : history) {
//        Watchable *watched = i;
        for (size_t j = 0; j < i->getTags().size(); j++) {
            string s = i->getTags()[j];
            Listoftags.push_back(s);
        }
    }
    for (auto s : Listoftags) {
        bool found = false;
        for (size_t j = 0; j < tagList.size() && !found; j++) {
            if (s == tagList[j].getTag()) {
                tagList[j].raiseCounter();
                found = true;
            }
        }
        if (!found) {
            tagPair tag = tagPair(s, 1);
            tagList.push_back(tag);
        }
    }
    for (size_t i = 0; i < tagList.size() - 1; i++) {
        int minInd = findMax(tagList, i);
        swap(tagList, i, minInd);
    }
    for (auto & i : tagList){
        string s = i.getTag() ;
    }
    for (auto & i : tagList) {
        string s = i.getTag();
        for (auto & i : content) {
            if (haveTag(s, i)) {
                if (!isExistsInHistory(i->getId())) {
                    return i;
                }
            }
        }
    }
    return output ;
}

int User::findMax(vector<tagPair> tagVector, int index) //helps find the "best" value according to amount of times you watched the genre and the lexicographic order.
{
    int maxIndex = index;
    for (size_t i = index+1; i < tagVector.size(); i++) {
        if(tagVector[i].compareTag(tagVector[maxIndex]) == 1)
        {
            maxIndex = i;
        }
    }
    return maxIndex;
}

void User::swap(vector<tagPair> &tagVector, int i, int j) const //a swap function in order to swap between tagPairs in the vector.
{
    tagPair tmp= tagVector[i];
    tagVector[i] = tagVector[j];
    tagVector[j] = tmp;
}

bool User::haveTag(string preferTag,Watchable* watch) {//find if the tag already exist in order not to create duplicates.
     vector<string> tags =watch->getTags() ;
     for (const auto & tag : tags)
     {
         if(tag == preferTag ){
             return true ;
         }
     }
     return false ;
    }



