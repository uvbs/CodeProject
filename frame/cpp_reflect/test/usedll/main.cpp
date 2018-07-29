#include <iostream>
using std::cout;
using std::endl;

#include <Reflect.h>
using namespace Reflect;

#include <windows.h>

TypeRegistry* importTypeRegistry(const char* dllname)
{
	typedef TypeRegistry& (*pfunt) ();
	HMODULE module = LoadLibrary(dllname);
	if (!module) return 0;
	pfunt pfun = (pfunt) GetProcAddress(module, "exportTypeRegistry");
	if (!pfun) return 0;
	return &((*pfun)());
}

void stampa(ObjHolder* obj)
{
    if (!obj)
    {
        return;
    }
	if (obj->type().isClass())
	{
		const ClassType& type = obj->type().toClassType();
		for (ClassType::FieldsIterator it = type.fieldsBegin(); it != type.fieldsEnd(); ++it)
		{
			ObjHolder* subObj = (*it)->get(obj);
			cout << (*it)->name() << " : " << (*it)->type().name();
			stampa(subObj);
			delete subObj;

			for (ClassType::BaseClassesIterator itb = type.baseClassesBegin(); itb != type.baseClassesEnd(); ++itb)
			{
				ObjHolder* subObj = (*itb).upCaster().apply(obj);
				stampa(subObj);
				delete subObj;
			}
		}
	}
	else if (obj->type().isBuiltin())
	{
		cout << "; obj: " << obj->toString() << endl;
	}
	else if (obj->type().isPointer())
	{
		ObjHolder* der = obj->deref();
		cout << " Pointee type: " << der->type().name() << endl;
		stampa(der);
		delete der;
	}
}

int main()
{
	TypeRegistry* reg = importTypeRegistry("makedll.dll");
	const ClassType& ct = reg->find("Film")->toClassType();
	ObjHolder* obj = ct.newInstance();
	std::string titolo("titolo");
	std::string regista("regista");
	int anno(1234);
	ct.findField("_title")->set(obj, ObjHolder::create(&titolo));
	ct.findField("_director")->set(obj, ObjHolder::create(&regista));
	ct.findField("_year")->set(obj, ObjHolder::create(&anno));

	stampa(obj);

	cout << "invoke" << endl;
	std::vector<ObjHolder*> params;
	params.push_back(obj);
	ObjHolder* res = ct.findMethod("test3")->invoke(obj, params);
	stampa(res);
	res = ct.findMethod("test2")->invoke(obj, std::vector<ObjHolder*>());
    stampa(res);

	system ("pause");
	return 0;
}


