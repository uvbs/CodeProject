////////////////////////////////////////////////////////////////////////////////////
// 
// File: cbfunc.h
// Author: by Mr.Chen
// CreateTime: 2014/5/1	0:27
// Desc: �ص�������װ(֧�����Ա������ȫ�ֺ���), ref from CEGUI.
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __CB_FUNC_H_
#define __CB_FUNC_H_

#include "type.h"
#include <map>
using std::map;
using std::make_pair;

namespace sys_util {

	typedef int EventArgs;
	typedef int EventID;

	//�ص�����
	class SlotFunctorBase
	{
	public:
		virtual ~SlotFunctorBase() {}
		//ʹ�ò����������ú���
		virtual bool operator()(const EventArgs& args) = 0;
	};

	//ȫ�ֺ������ྲ̬��Ա�������ص�
	class FreeFunctionSlot : public SlotFunctorBase
	{
	public:
		//����ص������ĸ�ʽ
		typedef bool (SlotFunction)(const EventArgs&);  //����ָ��

	public:
		FreeFunctionSlot(SlotFunction* func) : _pFunction(func)
		{}
		virtual bool operator()(const EventArgs& args)
		{
			return _pFunction(args);
		}
	private:
		SlotFunction* _pFunction;
	};

	//����ͨ��Ա�������ص�
	template<typename T>
	class MemberFunctionSlot : public SlotFunctorBase
	{
	public:
		//��Ա�����Ļص���������
		typedef bool (T::*MemberFunctionType)(const EventArgs&);  //���Ա����ָ�������

	public:
		MemberFunctionSlot(MemberFunctionType func, T* obj) :
		_function(func), _pObj(obj)
		{}
		virtual bool operator()(const EventArgs& args)
		{
			return (_pObj->*_function)(args);  //������ĳ�Ա����
		}
	private:
		MemberFunctionType _function;
		T* _pObj;
	};

	//����������ʵ�ֵķ�װ
	class SubscriberSlot
	{
	public:
		//Ĭ�Ϲ��캯��
		SubscriberSlot() 
		{
			_pFunctorImpl = NULL;
		}
		//��׼��������
		~SubscriberSlot()
		{
			delete _pFunctorImpl;
			_pFunctorImpl = NULL;
		}
		//���ú�����()���أ������ɵ�һ��Functorʵ��
		bool operator()(const EventArgs& args) const
		{
			return (*_pFunctorImpl)(args);
		}
		//���س�Ա�Ƿ���Ч���Ƿ��Ѿ����ӵ�һ�������Functorʵ��
		bool connected() const
		{
			return _pFunctorImpl != NULL;
		}
		//FreeFunctionSlot�����ɺ����ķ�װ��
		SubscriberSlot(FreeFunctionSlot::SlotFunction* func) :
		_pFunctorImpl(new FreeFunctionSlot(func))
		{}
		// ģ�幹�캯�����Գ�Ա�����ķ�װΪ������MemberFunctionSlot��
		template<typename T>
		SubscriberSlot(bool (T::*function)(const EventArgs&), T* obj) :
		_pFunctorImpl(new MemberFunctionSlot<T>(function, obj))
		{}

	private:
		//�ڲ�����Functor��ָ�룬SlotFunctorBase����������������õ���
		SlotFunctorBase* _pFunctorImpl;
	};

	// �ص���װ�����
	class CbfuncHelper
	{
	public:
		typedef map<EventID, SubscriberSlot*> EventMap;
		
	public:
		CbfuncHelper() {_eventList.clear();}
		~CbfuncHelper() {_eventList.clear();}

		bool subscribe(EventID id, SubscriberSlot* subscriberSlot)
		{
			if (NULL == subscriberSlot)
			{
				return false;
			}
			EventMap::iterator iter = _eventList.find(id);
			if (iter != _eventList.end())
			{
				return false;
			}
			_eventList.insert(make_pair(id, subscriberSlot));
			return true;
		}

		bool evoke(EventID id)
		{
			EventMap::iterator iter = _eventList.find(id);
			if (iter == _eventList.end())
			{
				return false;
			}
			SubscriberSlot* subscribeSlot = _eventList[id];
			if (subscribeSlot)
			{
				EventArgs args = 0;
				(*subscribeSlot)(args);
				return true;
			}
			return false;
		}

    public:
        void registerEvent(int eventId, void* func)
        {
            subscribe(eventId, new SubscriberSlot((FreeFunctionSlot::SlotFunction*)func));
        }
        template<typename T>
        void registerEvent(int eventId, bool (T::*func)(const EventArgs&), T* obj)
        {
            subscribe(eventId, new SubscriberSlot(func, obj));
        }
        bool fireEvent(int eventId)
        {
            return evoke(eventId);
        }

	private:
		EventMap _eventList;
	};

	// ע�ắ����
	//////////////////////////////////////////////////////////////
	// ע��ȫ�ֺ������ྲ̬��Ա������Ϊ�ص�����
	//////////////////////////////////////////////////////////////
	//#define  REGISTER_FUNC(id, subscribeSlot, func, q) \
	//	SubscriberSlot* subscribeSlot = new SubscriberSlot((FreeFunctionSlot::SlotFunction*)&func);\
	//	q->subscribe(id, subscribeSlot);

	#define  REGISTER_FUNC(id, func, q) \
		q->subscribe(id, new SubscriberSlot((FreeFunctionSlot::SlotFunction*)&func));

	//////////////////////////////////////////////////////////////
	// ע�����Ա������Ϊ�ص�����
	//////////////////////////////////////////////////////////////
	// @param id			: �¼�id
	// @param subscribeSlot	: ���һ��������
	// @param func			: Ҫע��Ļص��������������ͨ��Ա����
	// @param p				: func�������һ��ʵ��ָ��
	// @param q				: ע����(����ע�ắ������)��һ��ʵ��ָ��

	//#define  REGISTER_OBJFUNC(id, subscribeSlot, func, p, q) \
	//	SubscriberSlot* subscribeSlot = new SubscriberSlot(&func, p);\
	//	q->subscribe(id, subscribeSlot);

	#define  REGISTER_OBJFUNC(id, func, _p, q) \
		q->subscribe(id, new SubscriberSlot(&func, _p));
	
} // namespace sys_util
using namespace sys_util;

#endif	// __CB_FUNC_H_
