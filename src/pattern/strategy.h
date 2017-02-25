////////////////////////////////////////////////////////////////////////////////////
// 
// @file: strategy.h
// @author: by Mr.Chen
// @date: 2015/4/19	23:15
// @brief: ����ģʽ
//        ����ģʽ�������㷨�壬�ֱ��װ������������֮������໥�滻��
//        ��ģʽ���㷨�ı仯������ʹ���㷨�Ŀͻ���
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef _STRATEGY_H
#define _STRATEGY_H

#include <stdio.h>

namespace strategy_pattern
{
    /*
    * ������Ϊ�������С�������
    * ���������
    */
    class FlyBehavior
    {
        friend class Duck;

        virtual void fly() = 0;
    };

    //����������1
    class FlyWithWings : public FlyBehavior
    {
        void fly()
        {
            printf("I am flying !\n");
        }
    };

    //����������2
    class FlyNoWay : public FlyBehavior
    {
        void fly()
        {
            printf("I can not fly !\n");
        }
    };

	//����������3
	class FlyRocketPowered : public FlyBehavior
	{
		void fly()
		{
			printf("I am flying with a rocket !\n");
		}
	};

    /*
    * ������Ϊ�������ɽС�������
    * ���������
    */
    class QuackBehavior
    {
        friend class Duck;

        virtual void quack() = 0;
    };

    //����������1
    class Quack : public QuackBehavior
    {
        void quack()
        {
            printf("Quack !\n");
        }
    };

    //����������2
    class MuteQuack : public QuackBehavior
    {
        void quack()
        {
            printf("<<Silent, I can not quack !>>\n");
        }
    };

	//����������3
	class Squeak : public QuackBehavior
	{
		void quack()
		{
			printf("Squeak !\n");
		}
	};

    /*
    * ��ɫ����
    * 
    */
    class Duck
    {
    protected:
        FlyBehavior* _pFlyBehavior;
        QuackBehavior* _pQuackBehavior;

	public:
		void setFlyBehavior(FlyBehavior* fb)
		{
			_pFlyBehavior = fb;
		}
		void setQuackBehavior(QuackBehavior* qb)
		{
			_pQuackBehavior = qb;
		}

    public:
        virtual void display() = 0;
        void swim()
        {
            printf("All ducks can swim !\n");
        }
        void performFly()
        {
            if (_pFlyBehavior) 
			{
				_pFlyBehavior->fly();
			}
        }
        void performQuack()
        {
            if (_pQuackBehavior)
			{
				_pQuackBehavior->quack();
			}
        }
    };

    //��ɫ������1
    class MallardDuck : public Duck
    {
    public:
        void display()
        {
            printf("I am a real Mallard duck !\n");
        }
        MallardDuck()
        {
            _pFlyBehavior = new FlyWithWings();
            _pQuackBehavior = new Quack();
        }
    };

	//��ɫ������2
	class ModelDuck : public Duck
	{
	public:
		ModelDuck()
		{
			_pFlyBehavior = new FlyNoWay();
			_pQuackBehavior = new Quack();
		}
		void display()
		{
			printf("I am a model duck !\n");
		}
	};

	void test();

} //namespace strategy_pattern

using namespace strategy_pattern;

#endif
