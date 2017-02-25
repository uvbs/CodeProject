//////////////////////////////////////////////////////////////////////////

#ifndef APP_GENSOFT_P2P_CLIENT_LIVE_CODEPRJ_REFLECT_V2_PROPERTYSET_H
#define APP_GENSOFT_P2P_CLIENT_LIVE_CODEPRJ_REFLECT_V2_PROPERTYSET_H

#include <map>
#include <string>
#include "property.h"

typedef std::map<std::string, Property*> PropertyContainer;
typedef std::map<std::string, Property*>::value_type PropertyContainerValueType;
typedef std::map<std::string, Property*>::iterator PropertyItor;

// -----------------------------------------------------------------------------------------------------
class PropertySet
{
protected:
	PropertyContainer _properties;
	
public:
	PropertySet(void);
	virtual ~PropertySet(void);

	void registerProperty(std::string const& name, int* value);
	void registerProperty(std::string const& name, float* value);
	void registerProperty(std::string const& name, std::string* value);
	void registerProperty(std::string const& name, bool* value);

	Property* lookUp(std::string const& name);

	bool setValue(std::string const& name, std::string& value);
	bool set(std::string const& name, int value);
	bool set(std::string const& name, float value);
	bool set(std::string const& name, std::string value);
	bool set(std::string const& name, bool value);
	bool set(std::string const& name, char* value);

	int getInt(std::string const& name);
	float getFloat(std::string const& name);
	std::string getString(std::string const& name);
	bool getBool(std::string const& name);

	PropertyContainer getPropertyContainer() const;
};

// -----------------------------------------------------------------------------------------------------
#endif // _PropertySet_H

