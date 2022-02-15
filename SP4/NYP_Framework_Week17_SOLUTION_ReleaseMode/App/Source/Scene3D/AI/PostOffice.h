#ifndef POST_OFFICE_H
#define POST_OFFICE_H

#include "SingletonTemplate.h"

#include <string>
#include <map>
#include "ObjectBase.h"
#include "Message.h"

class PostOffice : public Singleton<PostOffice>
{
	friend Singleton<PostOffice>;

public:
	void Register(const std::string &address, ObjectBase *object);
	bool Send(const std::string &address, Message *message);
	//void BroadCast(Message *message);

private:
	PostOffice();
	~PostOffice();
	std::map<std::string, ObjectBase*> m_addressBook;
};

#endif
