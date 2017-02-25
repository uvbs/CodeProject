////////////////////////////////////////////////////////////////////////////////////
// 
// @file: property.h
// @author: by Mr.Chen
// @date: 2015/7/15	16:54
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef APP_GENSOFT_P2P_CLIENT_LIVE_CODEPRJ_REFLECT_V2_PROPERTY_H
#define APP_GENSOFT_P2P_CLIENT_LIVE_CODEPRJ_REFLECT_V2_PROPERTY_H

#include <string>
#include <cassert>

// -----------------------------------------------------------------------------------------------------
class Property
{
public:
	Property();
	Property(std::string const& name);
	Property(std::string const& name, int* value);
	Property(std::string const& name, float* value);
	Property(std::string const& name, std::string* value);
	Property(std::string const& name, bool* value);

	~Property();

	bool setUnknownValue(std::string const& value);
	bool set(int value);
	bool set(float value);
	bool set(std::string value);
	bool set(bool value);

	void setName(std::string const& name);
	std::string getName() const;

	int getInt();
	float getFloat();
	std::string getString();
	bool getBool();

// -----------------------------------------------------------------------------------------------------
protected:
	void erase();
	void regist(int* value);
	void regist(float* value);
	void regist(std::string* value);
	void regist(bool* value);

// -----------------------------------------------------------------------------------------------------
protected:
	union Data
	{
		int* piValue;
		float* pfValue;
		std::string* pstrValue;
		bool* pbValue;
	};

	enum Type
	{
		INT,
		FLOAT,
		STRING,
		BOOL,
		EMPTY
	};

	Data _data;
	Type _type;
	std::string _name;
};

#endif // _Property_H
