#include "PostOffice.h"

void PostOffice::Register(const std::string & address, ObjectBase * object)
{
	if (!object)
		return;
	if (m_addressBook.find(address) != m_addressBook.end())
		return;
	m_addressBook.insert(std::pair<std::string, ObjectBase*>(address, object));
}

bool PostOffice::Send(const std::string & address, Message * message)
{
	if (!message)
		return false;
	std::map<std::string, ObjectBase*>::iterator it = m_addressBook.find(address);
	if (m_addressBook.find(address) == m_addressBook.end())
	{
		delete message;
		return false;
	}
	ObjectBase *object = (ObjectBase*)it->second;
	return object->Handle(message);
}

PostOffice::PostOffice()
{
}

PostOffice::~PostOffice()
{
	m_addressBook.clear();
}
