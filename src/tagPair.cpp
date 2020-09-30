#include <iostream>
#include <cmath>
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/tagPair.h"
#include <unordered_map>

//tagPair is a class defined to help the user compare the most common "tags" in his watch history. you have 2 private fields so it can know how to compare between them.

using namespace std;
tagPair::tagPair(string tag, int counter): tag(tag), counter(counter) {} ;

tagPair::~tagPair() {}

void tagPair::raiseCounter() {
    this->counter = counter +1 ;
}

int tagPair::compareTag(tagPair otherTag) {// return 0 if the tag we are looking at is bigger than us. return 1 otherwise. include lexicographic order
    int otherTagSize = otherTag.getTag().size() ;
    int ThisTagSize = this->tag.size();
    if (this->counter > otherTag.getCounter()) {
        return 1;
    } else if (this->counter < otherTag.getCounter()) {
        return 0;
    } else {
        if (this->tag.size() >= otherTag.getTag().size()) {
            for (int i = 0; i < otherTagSize; i++) {
                if (this->tag[i] > otherTag.getTag()[i]) {
                    return 1;
                } else if (this->tag[i] < otherTag.getTag()[i]) {
                    return 0;
                }
                return 0;
            }
        }
            if (this->tag.size() < otherTag.getTag().size()) {
                for (int i = 0; i < ThisTagSize; i++) {
//                    char c = otherTag.getTag()[i];
//                    char g = this->tag[i];
//                    bool f = this->tag[i] > otherTag.getTag()[i];
                    if (this->tag[i] > otherTag.getTag()[i]) {
                        return 0;
                    } else if (this->tag[i] < otherTag.getTag()[i]) {
                        return 1;
                    }
                }
                return 1;
            }
        }
    return 1;
}

int tagPair::getCounter() {
    return this->counter ;
}

string tagPair::getTag() {
    return this->tag;
}




