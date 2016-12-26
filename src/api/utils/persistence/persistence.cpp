#define USE_STD_ARRAY
#include <vector>

#include "stdafx.h"

#include "persistence.h"
#include "dynamics.h"

/**
	Persistent
*/

using namespace FWdebugExceptions;
using namespace Grafkit;
using namespace std;

void Persistent::store(Archive& ar)
{
	string className(this->getClassName());
	int ver = this->version();

	Log::Logger().Info("Storing object %s %d", className.c_str(), ver);

	ar & PERSIST_STRING(&className);
	ar & PERSIST_FIELD(ver);

	this->serialize(ar);
}

//void Grafkit::Persistent::serialize(Archive & stream)
//{
//}

Persistent* Persistent::load(Archive& ar)
{
	string className;
	ar & PERSIST_STRING(&className);

	Log::Logger().Info("Loading object %s %d", className.c_str());

	Clonable* clone = Clonables::Instance().create(className.c_str());
	if (clone == NULL) {
#ifdef DEBUG
		Clonables::Instance().dumpClonables();
#endif
		throw EX_DETAILS(PersistentCreateObjectExcpetion, className.c_str());
	}
	//auto_ptr<Clonable> delitor(clone);

	Persistent * obj = dynamic_cast<Persistent *>(clone);
	if (obj == NULL) {
		throw EX_DETAILS(PersistentCreateObjectExcpetion, className.c_str());
	}

	int ver = -1;
	ar & PERSIST_FIELD(ver);

	//
	if (ver != obj->version()) {
		throw EX(PersistentVersionMismatch);
	}

	obj->serialize(ar);
	//delitor.release();

	return obj;
}

/**
	Archive
*/

Archive::Archive(int isStoring) :
	_isStoring(isStoring)
{
}

Archive::~Archive()
{

}

size_t Grafkit::Archive::writeString(const char * input)
{
	unsigned int slen = strlen(input);
	this->write(&slen, sizeof(slen));
	this->write(input, slen + 1);
	return slen;
}

size_t Grafkit::Archive::readString(char*& output)
{
	unsigned int slen = 0;
	this->read(&slen, sizeof(slen));
	output = new char[slen + 1];
	this->read(output, slen + 1);
	return slen;
}

/**
	Persistent helpers
*/

void Grafkit::PersistString::store(Archive & ar)
{
	const char *out = nullptr;
	if (m_pString) {
		out = m_pString->c_str();
	}
	else {
		out = *m_pszString;
	}
	ar.writeString(out);
}

void Grafkit::PersistString::load(Archive & ar)
{
	char *in = nullptr;
	ar.readString(in);
	if (m_pString) {
		m_pString->clear();
		m_pString->assign(in);
	}
	else {
		*m_pszString = in;
	}
}