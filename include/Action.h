#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

using namespace std;

class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
	BaseAction();
	ActionStatus getStatus() const;
	virtual void act(Session& sess)=0;
	virtual std::string toString() const=0;
	virtual std::string enumToString() const;
	virtual ~BaseAction() ;
    virtual std::string getType() =0;
    std::string getErrorMsg() const;
    virtual void setErrorMsgAndStatus(string errorMsg,ActionStatus actionStatus);
protected:
	void complete();
	void error(const std::string& errorMsg);
private:
	std::string errorMsg;
	ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
	virtual void act(Session& sess) ;
	virtual std::string toString() const;
    virtual string getType();
};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual string getType();
};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual string getType();
};


class DuplicateUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual string getType();
};

class PrintContentList : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual string getType();
};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual string getType();
};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual string getType();
};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual string getType();
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual string getType();
};
#endif
