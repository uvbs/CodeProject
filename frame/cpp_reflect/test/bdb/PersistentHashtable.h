#ifndef PERSISTENT_HASHTABLE_H
#define PERSISTENT_HASHTABLE_H

#include <Reflect.h>
#include "db.h"

template <typename KeyType, typename ValueType>
class PersistentHashtable
{
public:
	PersistentHashtable(const char* file)
		:	_file(file)
	{

	}

	void open()
	{
		db_create(&_db, 0, 0);
		_db->open(_db, 0, _file, 0, DB_BTREE, DB_CREATE, 0);
	}

	void close()
	{
		_db->close(_db, 0);
	}

	bool add(const KeyType& key_, const ValueType& value_)
	{
		KeyType* key = const_cast<KeyType*>(&key_);
		ValueType* value = const_cast<ValueType*>(&value_);

		Reflect::ObjHolder* obj_key = Reflect::ObjHolder::create(key);
		Reflect::ObjHolder* obj_value = Reflect::ObjHolder::create(value);
		Serialization::OutArchive* out_key = Serialization::serialize(obj_key);
		Serialization::OutArchive* out_value = Serialization::serialize(obj_value);

		clearKeyValue();

		_keyDBT.size = out_key->size();
		_keyDBT.data = out_key->get();
		_valueDBT.size = out_value->size();
		_valueDBT.data = out_value->get();
		
		int ret = _db->put(_db, 0, &_keyDBT, &_valueDBT, DB_NOOVERWRITE);

		delete obj_key;
		delete obj_value;
		delete out_key;
		delete out_value;
		return (ret == 0) ? true : false;
	}

	ValueType& operator[](const KeyType& key_)
	{
		KeyType* key = const_cast<KeyType*>(&key_);
		Reflect::ObjHolder* obj_key = Reflect::ObjHolder::create(key);
		Serialization::OutArchive* out_key = Serialization::serialize(obj_key);
		
		clearKeyValue();

		_keyDBT.size = out_key->size();
		_keyDBT.data = out_key->get();

		int ret = _db->get(_db, 0, &_keyDBT, &_valueDBT, 0);

		Reflect::ObjHolder* obj_value = 0;
		Serialization::InArchive* in_value = 0;
		ValueType* res = 0;
		if (ret == 0) {
			in_value = new Serialization::InArchive(_valueDBT.data);
			obj_value = Serialization::deserialize(in_value);
			res = &(obj_value->get<ValueType>());
		}

		delete obj_key;
		delete out_key;
		delete obj_value;
		delete in_value;
		return *res;
	}

	void remove(const KeyType& key_)
	{
		KeyType* key = const_cast<KeyType*>(&key_);
		Reflect::ObjHolder* obj_key = Reflect::ObjHolder::create(key);
		Serialization::OutArchive* out_key = Serialization::serialize(obj_key);
		
		clearKeyValue();

		_keyDBT.size = out_key->size();
		_keyDBT.data = out_key->get();

		_db->del(_db, 0, &_keyDBT, 0);
		delete obj_key;
		delete out_key;
	}

	void modify(const KeyType& key_, const ValueType& value_)
	{
		remove(key_);
		add(key_, value_);
	}

	struct Pair
	{
		Pair(KeyType& key_, ValueType& value_) 
			:	key(key_), value(value_)
		{
		
		}

		KeyType& key;
		ValueType& value;
	};

	class Iterator
	{
	public:
		Iterator(DB* db)
		{
			db->cursor(db, 0, &_cursor, 0);
		}

		bool hasMore()
		{
			clearKeyValue();
			return ((_cursor->c_get(_cursor, &_key, &_value, DB_NEXT)) == 0);
		}

		Pair current()
		{
			Serialization::InArchive* in_key = new Serialization::InArchive(_key.data);
			Reflect::ObjHolder* obj_key = Serialization::deserialize(in_key);
			Serialization::InArchive* in_value = new Serialization::InArchive(_value.data);
			Reflect::ObjHolder* obj_value = Serialization::deserialize(in_value);
			Pair res(obj_key->get<KeyType>(), obj_value->get<ValueType>());
			delete in_key;
			delete in_value;
			delete obj_key;
			delete obj_value;
			return res;
		}

	private:
		void clearKeyValue()
		{
			memset(&_key, 0, sizeof(DBT));
			memset(&_value, 0, sizeof(DBT));  
		}

		DBC* _cursor;
		DBT _key;
		DBT _value;
	};

	Iterator getIterator()
	{
		return Iterator(_db);
	}

private:
	void clearKeyValue()
	{
		memset(&_keyDBT, 0, sizeof(DBT));
		memset(&_valueDBT, 0, sizeof(DBT));  
	}

	const char* _file;
	DB* _db;
	DBT _keyDBT;
	DBT _valueDBT;
};

class ObjPersistentHashtable
{
public:
	ObjPersistentHashtable(const char* file)
		:	_file(file)
	{

	}

	void open()
	{
		db_create(&_db, 0, 0);
		_db->open(_db, 0, _file, 0, DB_BTREE, DB_CREATE, 0);
	}

	void close()
	{
		_db->close(_db, 0);
	}

	bool add(Reflect::ObjHolder* obj_key, Reflect::ObjHolder* obj_value)
	{
		Serialization::OutArchive* out_key = Serialization::serialize(obj_key);
		Serialization::OutArchive* out_value = Serialization::serialize(obj_value);

		clearKeyValue();

		_keyDBT.size = out_key->size();
		_keyDBT.data = out_key->get();
		_valueDBT.size = out_value->size();
		_valueDBT.data = out_value->get();
		
		int ret = _db->put(_db, 0, &_keyDBT, &_valueDBT, DB_NOOVERWRITE);

		delete out_key;
		delete out_value;
		return (ret == 0) ? true : false;
	}

	Reflect::ObjHolder* operator[](Reflect::ObjHolder* obj_key)
	{
		Serialization::OutArchive* out_key = Serialization::serialize(obj_key);
		
		clearKeyValue();

		_keyDBT.size = out_key->size();
		_keyDBT.data = out_key->get();

		int ret = _db->get(_db, 0, &_keyDBT, &_valueDBT, 0);

		Reflect::ObjHolder* obj_value = 0;
		Serialization::InArchive* in_value = 0;

		if (ret == 0) {
			in_value = new Serialization::InArchive(_valueDBT.data);
			obj_value = Serialization::deserialize(in_value);
		}

		delete obj_key;
		delete out_key;
		delete in_value;
		return obj_value;
	}

	void remove(Reflect::ObjHolder* obj_key)
	{
		Serialization::OutArchive* out_key = Serialization::serialize(obj_key);
		
		clearKeyValue();

		_keyDBT.size = out_key->size();
		_keyDBT.data = out_key->get();

		_db->del(_db, 0, &_keyDBT, 0);
		delete out_key;
	}

	void modify(Reflect::ObjHolder* obj_key, Reflect::ObjHolder* obj_value)
	{
		remove(obj_key);
		add(obj_key, obj_value);
	}

	struct Pair
	{
		Pair(Reflect::ObjHolder* key_, Reflect::ObjHolder* value_) 
			:	key(key_), value(value_)
		{
		
		}

		void dispose()
		{
			delete key;
			delete value;
		}

		Reflect::ObjHolder* key;
		Reflect::ObjHolder* value;
	};

	class Iterator
	{
	public:
		Iterator(DB* db)
		{
			db->cursor(db, 0, &_cursor, 0);
		}

		bool hasMore()
		{
			clearKeyValue();
			return ((_cursor->c_get(_cursor, &_key, &_value, DB_NEXT)) == 0);
		}

		Pair current()
		{
			Serialization::InArchive* in_key = new Serialization::InArchive(_key.data);
			Reflect::ObjHolder* obj_key = Serialization::deserialize(in_key);
			Serialization::InArchive* in_value = new Serialization::InArchive(_value.data);
			Reflect::ObjHolder* obj_value = Serialization::deserialize(in_value);
			Pair res(obj_key, obj_value);
			delete in_key;
			delete in_value;
			return res;
		}

	private:
		void clearKeyValue()
		{
			memset(&_key, 0, sizeof(DBT));
			memset(&_value, 0, sizeof(DBT));  
		}

		DBC* _cursor;
		DBT _key;
		DBT _value;
	};

	Iterator getIterator()
	{
		return Iterator(_db);
	}

private:
	void clearKeyValue()
	{
		memset(&_keyDBT, 0, sizeof(DBT));
		memset(&_valueDBT, 0, sizeof(DBT));  
	}

	const char* _file;
	DB* _db;
	DBT _keyDBT;
	DBT _valueDBT;
};

#endif
