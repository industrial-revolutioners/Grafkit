#include "persistent.h"
#include "dynamics.h"

#include <vector>

#include "../../debug/exceptions.h"

/**
	Persistent
*/

using namespace FWutils;
using namespace std;

void Persistent::store(Archive& stream) const 
{
#if 0 // ha nincs RTTI, akkor ez nem jo valasztas
	string className = typeid(*this).name();
	className = className.substr(className.find(' ') + 1);
#else
	string className(this->getClassName());
#endif
	
	stream << className;
	
	int ver = version();
	stream << ver;
	
	stream.setDirection(true);
	
	const_cast<Persistent *>(this)->serialize(stream);
	
}

Persistent* Persistent::load(Archive& stream)
{           
	string className;
	stream >> className;

	Clonable* clone = Clonables::Instance().create(className.c_str());
	if(clone == NULL) 
		throw new ExceptionEX(1001,"Persistent::load : Error creating object");

	auto_ptr<Clonable> delitor(clone);

	Persistent * obj = dynamic_cast<Persistent *>(clone);
	if(obj == NULL) {
		throw new ExceptionEX(1001,"Persistent::load : Error creating object");
	}

	int ver = -1;

	stream >> ver;

	if(ver != obj->version())
		throw new ExceptionEX(1001,"Persistent::load : unmatched version number");

	stream.setDirection(false);

	obj->serialize(stream);
	delitor.release();

	return obj;
}

/**
	Archive
*/

Archive::Archive(bool isStoring) : 
	_isStoring(isStoring)
{
}


void Archive::initCheck(){
	std::string signature; 
	short major= -1, minor = -1 ;

	if (isStoring()){
		signature.assign(PERSISTENT_SIGNATURE);
		major = PERSISTENT_MAJOR_VER;
		minor = PERSISTENT_MINOR_VER;
		*this << signature << major << minor;

	}else{
		*this >> signature >> major >> minor;

		///@todo: vercheck
	}
}

Archive::~Archive()
{

}

///@todo zero vegu stringeket taroljon el, es nem kell ele a string hossza.
Archive& Archive::operator<<(const string& str)
{ 
	int length = str.length();
	
	*this << length;
	write(str.c_str(), sizeof(char) * length+1);

	return *this;
}

Archive& Archive::operator>>(string& str)
{
	int length = -1;
	
	*this >> length;
	vector<char> mem(length + 1);

	char* pChars = &mem[0];
	read(pChars, sizeof(char) * length + 1);
	mem[length] = NULL; // elvileg zero vegu a string
	str = pChars;
	
	return *this;
}

///@todo zero vegu stringeket taroljon el, es nem kell ele a string hossza.
Archive& Archive::operator<<(const char*& str){
	int length = strlen(str);
	
	*this << length;
	write(str, sizeof(char) * length+1);

	return *this;
}

Archive& Archive::operator>>(char*& str){
	int length = -1;
	
	*this >> length;
	
	char* pChars = str;
	read(pChars, sizeof(char) * length + 1);
	pChars [length] = NULL;	// elvileg zero vegu a string
	str = pChars;
	
	return *this;
}