////////////////////////////////////////////////////////////////////////////////////
// 
// @file: hash_table.h
// @author: by Mr.Chen
// @date: 2014/5/14	16:06
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __HASH_TABLE_H_
#define __HASH_TABLE_H_

#include <iostream>
#include "assert.h"
#include "type.h"
#include "string_util.h"
#include "macrodef.h"
using std::cout;
using std::endl;

namespace container
{
	enum 
	{
		INVALID_VALUE = -1,
		PRIME_NUM = 28,
	};

	//������
	static uint s_primeList[PRIME_NUM] = 
	{
		53u,         97u,         193u,       389u,       769u,
		1543u,       3079u,       6151u,      12289u,     24593u,
		49157u,      98317u,      196613u,    393241u,    786433u,
		1572869u,    3145739u,    6291469u,   12582917u,  25165843u,
		50331653u,   100663319u,  201326611u, 402653189u, 805306457u, 
		1610612741u, 3221225473u, 4294967291u
	};

	//���ڻ�ȡ���ʵ���������ϣ������Ĵ�С
	static uint getPrime(uint size)
	{
		for (int i = 0; i < PRIME_NUM; ++i)
		{
			if (s_primeList[i] >= size)
			{
				return s_primeList[i];
			}
		}
		return INVALID_VALUE;
	}

	//��ϣ������, ���ڻ�ȡ��ϣ��
	class Hash
	{
	public:
		template<typename Key> 
		uint operator()(Key key) const {
			return key;
		}
		uint operator()(char* key)   const
		{
			uint hashCode = 0u;
			while (*key) {
				hashCode = hashCode * 31 + *(uchar*)(key);
				++key;
			}
			return hashCode;
		}
		uint operator()(const char* key) const 
		{
			uint hashCode = 0u;
			while (*key) {
				hashCode = hashCode * 31 + *(uchar*)(key);
				++key;
			}
			return hashCode;
		}
		uint operator()(uchar* key)	const
		{
			uint hashCode = 0u;
			while (*key) {
				hashCode = hashCode * 31 + *key;
				++key;
			}
			return hashCode;
		}
		uint operator()(const uchar* key)  const
		{
			uint hashCode = 0u;
			while (*key) {
				hashCode = hashCode * 31 + *key;
				++key;
			}
			return hashCode;
		}
		uint operator()(wchar_t* key) const
		{
			uint hashCode = 0u;
			while (*key) {
				hashCode = hashCode * 31 + *key;
				++key;
			}
			return hashCode;
		}
		uint operator()(const wchar_t* key) const
		{
			uint hashCode = 0u;
			while (*key) {
				hashCode = hashCode * 31 + *key;
				++key;
			}
			return hashCode;
		}
		uint operator()(uint64 key)	const
		{
			return (uint)(key ^ (key >> 32));
		}
	};

	class Compare
	{
	public:
		template<typename Key> 
		bool operator()(Key key1, Key key2) const
		{
			return key1 == key2;
		}
		bool operator()(char* key1, char* key2) const
		{
			return !str_util::strcmp(key1, key2);
		}
		bool operator()(const char* key1, const char* key2) const
		{
			return !str_util::strcmp(const_cast<char*>(key1), key2);
		}
		bool operator()(const char* &key1, const char* &key2) const
		{
			return !str_util::strcmp(const_cast<char*>(key1), key2);
		}
	};

	//�ù�ϣ�����ڿ��ٲ��ң�ͨ�������������ͻ��
	//ʵ���˲��롢���ҡ����¡�ɾ����
	template<typename Key, typename Value, typename HashFunc = Hash, typename CmpFunc = Compare>
	class HashTable
	{
	public:
		//��ϣ���еĽڵ���
		struct HashNode
		{
			HashNode* _pNext;
			Key _key;
			Value _value;
			HashNode(const Key& key, const Value& value) : _key(key), _value(value), _pNext(NULL)
			{}
		public:
			HashNode(const HashNode& rhs)
			{
				this->_pNext = rhs._pNext;
				this->_key = rhs._key;
				this->_value = rhs._value;
			}
			HashNode& operator=(const HashNode& rhs)
			{
				if(*this == rhs)
				{
					return *this;
				}
				this->_pNext = rhs._pNext;
				this->_key = rhs._key;
				this->_value = rhs._value;
				return *this;
			}
		};

	public:
		explicit HashTable(int capacity);
		~HashTable();

		//���Ԫ��
		bool insert(const Key& key, const Value& value);
		//����Ԫ��
		const Value* find(const Key& key) const;
		//����Ԫ��
		bool update(const Key& key, const Value& value);
		//ɾ��Ԫ��
		bool erase(const Key& key);

		//Ԫ�ظ���
		uint size() const {return _elementsCount;}
		//��ϣ�������С
		uint capacity() const {return _capacity;} 
		//���������С�� ͨ������һ���¹�ϣ��Ȼ�󽫾ɱ�����ݲ��뵽�±�����±���������ݳ�Ա��ֵ���ɱ�
		bool resize(uint size);

		//��ʹ��״��������־
		bool getStatus() const;
		//��������ϸʹ�����д����־
		void dump() const;
		//��չ�ϣ��
		bool clear();

	private:
		//��ֹ�������졢��ֵ
		HashTable(const HashTable& rhs);
		HashTable& operator = (const HashTable& rhs);

		//��ȡ������Ӧ�����е�����
		uint getHash(const Key& key, const uint size) const;
		//����֪������������ڵ�
		bool insertNode(const uint index, HashNode* hashNode);
		//����key����ڵ�
		bool insertNode(const Key& key, HashNode* hashNode);
		//�����ֵ�������ǹ�ϣ�����С
		bool insertNoResize(const Key& key, const Value& value);

	private:
		//��ϣ����
		HashFunc _hashFunc;
		//�ȽϺ���
		CmpFunc _cmpFunc;
		// ��ϣ������
		HashNode** _pBuckets;
		//��ϣ�������С
		uint _capacity;
		//Ԫ�ظ���
		uint _elementsCount;
		//������ʹ�õĸ���
		uint _usedCount;
	};

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	HashTable<Key, Value, HashFunc, CmpFunc>::HashTable(int capacity) : _capacity(capacity), _pBuckets(NULL), _elementsCount(0), _usedCount(0)
	{
		_capacity = (uint)getPrime(capacity); 
		Assert(_capacity);
		_pBuckets = new HashNode*[_capacity];
		for (uint i = 0; i < _capacity; ++i)
		{
			_pBuckets[i] = 0;
		}
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	HashTable<Key, Value, HashFunc, CmpFunc>::~HashTable()
	{
		bool ret = clear();
		if (!ret)
		{
			Assert(0);
			return;
		}
		SAFE_DELETE_ARRAY(_pBuckets);
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	uint HashTable<Key, Value, HashFunc, CmpFunc>::getHash(const Key& key, const uint capacity) const
	{
		uint hashCode = _hashFunc(key);
		Assert(hashCode != (uint)(INVALID_VALUE));
		return hashCode % capacity;
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	bool HashTable<Key, Value, HashFunc, CmpFunc>::insertNode(const uint index, HashNode* hashNode)
	{
		if (_pBuckets[index] == NULL)
		{
			_pBuckets[index] = hashNode;
			++_elementsCount;
			++_usedCount;
		}
		else
		{
			HashNode* curr = _pBuckets[index];
			HashNode* next = _pBuckets[index]->_pNext;
			while (next)
			{
				curr = next;
				next = next->_pNext;
			}
			curr->_pNext = hashNode;
			++_elementsCount;
		}
		return true;
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	bool HashTable<Key, Value, HashFunc, CmpFunc>::insertNode(const Key& key, HashNode* hashNode)
	{
		uint index = getHash(key, _capacity);
		return insertNode(index, hashNode);
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	bool HashTable<Key, Value, HashFunc, CmpFunc>::insertNoResize(const Key& key, const Value& value)
	{
		uint index = getHash(key, _capacity);
		Assert(index != (uint)(INVALID_VALUE));
		HashNode* hashNode = new HashNode(key, value);
		return insertNode(index, hashNode);
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	bool HashTable<Key, Value, HashFunc, CmpFunc>::resize(uint size)
	{
		if (size <= _capacity)
		{
			return true;
		}
		uint oldSize = _capacity;
		HashNode** pOld = _pBuckets;
		_capacity = getPrime(size);	
		_pBuckets = new HashNode*[_capacity]();
		_elementsCount = 0;
		_usedCount = 0;
		for (uint i = 0; i < oldSize; ++i)
		{
			HashNode* first = pOld[i];
			while (first)
			{
				HashNode* node = first;
				first = first->_pNext;
				node->_pNext = NULL;
				bool ret = insertNode(node->_key, node);
				if (!ret)
				{
					Assert(0);
					return ret;
				}
			}
		}
		SAFE_DELETE_ARRAY(pOld);
		return true;
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	bool HashTable<Key, Value, HashFunc, CmpFunc>::insert(const Key& key, const Value& value)
	{
		resize(_elementsCount + 1);
		return insertNoResize(key, value);
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	const Value* HashTable<Key, Value, HashFunc, CmpFunc>::find(const Key& key) const
	{
		uint index = getHash(key, _capacity);
		if (index == (uint)(INVALID_VALUE))
		{
			return NULL;
		}
		HashNode* node = _pBuckets[index];
		while (node)
		{
			if (_cmpFunc(node->_key, key))
			{
				return &(node->_value);
			}
			node = node->_pNext;
		}
		return NULL;
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	bool HashTable<Key, Value, HashFunc, CmpFunc>::update(const Key& key, const Value& value)
	{
		Value* oldValue = const_cast<Value*>(find(key));
		Assert(oldValue != NULL);
		*oldValue = value;
		return true;
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	bool HashTable<Key, Value, HashFunc, CmpFunc>::erase(const Key& key)
	{
		uint index = getHash(key, _capacity);
		HashNode* first = _pBuckets[index];
		HashNode* previous = NULL;
		while (first)
		{
			if(_cmpFunc(first->_key, key))
			{
				if (previous == NULL)
				{
					if (first->_pNext == NULL)
					{
						--_usedCount;
					}
					_pBuckets[index] = first->_pNext;
					SAFE_DELETE(first);
					--_elementsCount;
					return true;
				}
				else
				{
					previous->_pNext = first->_pNext;
					SAFE_DELETE(first);
					--_elementsCount;
					return true;
				}
			}
			previous = first;
			first = first->_pNext;
		}
		return false;
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	bool HashTable<Key, Value, HashFunc, CmpFunc>::getStatus() const
	{
		//std::cout << "HashTableʹ�õĸ���Ϊ��%d����ͻ�ĸ���Ϊ��%d��Ԫ�ظ���Ϊ��%d����ϣ�������С��%d", 
		//m_usedCount, (m_elementsNumber-m_usedCount), m_elementsNumber, m_capacity);
		if(_elementsCount != 0)
		{
			float rate = ((_elementsCount - _usedCount) * 1000 / _elementsCount) * 0.001f;
			//Log::GetInstance()->SaveSystemLog( "��ͻ��Ϊ��", rate );
		}
		else
		{
			//Log::GetInstance()->SaveSystemLog( "��ͻ��Ϊ��", 0 );
		}
		return true;
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	void HashTable<Key, Value, HashFunc, CmpFunc>::dump() const
	{
		for (int i = 0; i < _capacity; ++i)
		{
			std::cout << "index is " << i << ": " ;
			if (_pBuckets[i])
			{
				HashNode* first = _pBuckets[i];
				while (first)
				{
					//if (first->m_pNext)
					//	std::cout << 1 <<" -->";
					//else
					//	std::cout << " " << 1;
					cout << first->_key <<", ";
					first = first->_pNext;
				}
				cout << endl;
			}
			else
			{
				cout << 0 << endl;
			}
		}
	}

	template<typename Key, typename Value, typename HashFunc, typename CmpFunc>
	bool HashTable<Key, Value, HashFunc, CmpFunc>::clear()
	{
		for (uint i = 0; i < _capacity; ++i)
		{
			if (_pBuckets[i] != NULL)
			{
				HashNode* first = _pBuckets[i];
				while (first)
				{
					HashNode* tmp = first;
					first = first->_pNext;
					SAFE_DELETE(tmp);
					--_elementsCount;
				}
				_pBuckets[i] = NULL;
			}
		}
		_usedCount = 0;
		return (_elementsCount == 0);
	}

} // namespace container
using namespace container;

#endif	// __HASH_TABLE_H_
