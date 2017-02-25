#include "property.h"
#include "assert.h"
#include <stdlib.h>
// -----------------------------------------------------------------------------------------------------
Property::Property()
{
}

// -----------------------------------------------------------------------------------------------------
Property::Property(const std::string &name)
{
	this->_name = name;
}

// -----------------------------------------------------------------------------------------------------
Property::Property(const std::string &name, bool *value)
{
	this->_name = name;
	this->_type = BOOL;
	this->_data.pbValue = value;
}

// -----------------------------------------------------------------------------------------------------
Property::Property(const std::string &name, float *value)
{
	this->_name = name;
	this->_type = FLOAT;
	this->_data.pfValue = value;
}

// -----------------------------------------------------------------------------------------------------
Property::Property(const std::string &name, int *value)
{
	this->_name = name;
	this->_type = INT;
	this->_data.piValue = value;
}

// -----------------------------------------------------------------------------------------------------
Property::Property(const std::string &name, std::string *value)
{
	this->_name = name;
	this->_type = STRING;
	this->_data.pstrValue = value;
}

// -----------------------------------------------------------------------------------------------------
bool Property::setUnknownValue(std::string const& value)
{
	bool return_value = true;
	switch (_type)
	{
	case INT:
		*_data.piValue = atoi(value.c_str());
		break;
	case FLOAT:
		*_data.pfValue = static_cast<float>(atof(value.c_str()));
		break;
	case STRING:
		*_data.pstrValue = value;
		break;
	case BOOL:
		*_data.pbValue = (value.compare("true") == 0) || (value.compare("TRUE") == 0 || (value.compare("1") == 0));
		break;
	default:
		return_value = false;
		break;
	}

	return return_value;
}

// -----------------------------------------------------------------------------------------------------
bool Property::set(bool value)
{
	if (_type == BOOL)
	{
		*_data.pbValue = value;
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------------------------------
bool Property::set(float value)
{
	if (_type == FLOAT)
	{
		*_data.pfValue = value;
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------------------------------
bool Property::set(int value)
{
	if (_type == INT)
	{
		*_data.piValue = value;
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------------------------------
bool Property::set(std::string value)
{
	if (_type == STRING)
	{
		*_data.pstrValue = value;
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------------------------------
void Property::setName(const std::string &name)
{
	_name = name;
}

// -----------------------------------------------------------------------------------------------------
std::string Property::getName() const
{
	return _name;
}

// -----------------------------------------------------------------------------------------------------
int Property::getInt()
{
	Assert(_type == INT);
	return *_data.piValue;
}

// -----------------------------------------------------------------------------------------------------
float Property::getFloat()
{
	Assert(_type == FLOAT);
	return *_data.pfValue;
}

// -----------------------------------------------------------------------------------------------------
std::string Property::getString()
{
	Assert(_type == STRING);
	return *_data.pstrValue;
}

// -----------------------------------------------------------------------------------------------------
bool Property::getBool()
{
	Assert(_type == BOOL);
	return *_data.pbValue;
}

// -----------------------------------------------------------------------------------------------------
void Property::erase()
{
	_type = EMPTY;
}

// -----------------------------------------------------------------------------------------------------
void Property::regist(int* value)
{
	_type = INT;
	_data.piValue = value;
}

// -----------------------------------------------------------------------------------------------------
void Property::regist(float* value)
{
	_type = FLOAT;
	_data.pfValue = value;
}

// -----------------------------------------------------------------------------------------------------
void Property::regist(std::string* value)
{
	_type = STRING;
	_data.pstrValue = value;
}

// -----------------------------------------------------------------------------------------------------
void Property::regist(bool* value)
{
	_type = BOOL;
	_data.pbValue = value;
}
// -----------------------------------------------------------------------------------------------------
// END OF FILE
