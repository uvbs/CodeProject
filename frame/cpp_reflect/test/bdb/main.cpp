#include <iostream>
using std::cout;
using std::endl;

#include "PersistentHashtable.h"

int main()
{
	Reflect::TypeOf<int>::get();
	Reflect::TypeOf<std::string>::get();

	//PersistentHashtable<int, std::string> map("test.db");
	//map.open();

	//map.remove(12);

	//PersistentHashtable<int, std::string>::Iterator it = map.getIterator();
	//while (it.hasMore())
	//{
	//	PersistentHashtable<int, std::string>::Pair cur = it.current();
	//	cout << cur.key << endl;
	//	cout << cur.value << endl;
	//}

	//map.close();

	Reflect::ObjHolder* key = Reflect::ObjHolder::create(new int(12));
	Reflect::ObjHolder* value = Reflect::ObjHolder::create(new std::string("pluto"));

	ObjPersistentHashtable objmap("test2.db");
	objmap.open();
	
	ObjPersistentHashtable::Iterator it1 = objmap.getIterator();
	while (it1.hasMore())
	{
		ObjPersistentHashtable::Pair cur = it1.current();
		cout << cur.key->toString() << endl;
		cout << cur.value->toString() << endl;

		cur.dispose();
	}

	objmap.close();

	system ("pause");
	return 0;
}
