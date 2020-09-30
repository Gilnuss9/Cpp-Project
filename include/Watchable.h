#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    virtual std::vector<std::string> getTags() const ;
    virtual long getId() ;
    virtual int getLength() const ;
    virtual std::string getName() const = 0;
    virtual std::string getSeasonCode() const = 0;
    Watchable(const Watchable &aWatchable) ;
    void copy(Watchable &aWatchable);
    virtual std::string getType() =0;
    virtual int getEpisode() = 0 ;
    virtual int getSeason() = 0 ;
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
//    Movie(Movie &aMovie) ;
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual std::string getName() const ;
    virtual std::string getSeasonCode() const;
    virtual std::string getType();
    virtual int getEpisode() ;
    virtual int getSeason() ;
private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
//    Episode(Episode &aEpisode) ;
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual std::string getName() const ;
    virtual std::string getSeasonCode() const;
    virtual int getEpisode() ;
    virtual int getSeason() ;
    virtual int getNextId() ;
    virtual std::string getType();
private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
