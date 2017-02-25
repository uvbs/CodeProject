////////////////////////////////////////////////////////////////////////////////////
// 
// @file: strategy.h
// @author: by Mr.Chen
// @date: 2015/4/19	23:15
// @brief: 策略模式
//        策略模式定义了算法族，分别封装起来，让他们之间可以相互替换。
//        此模式让算法的变化独立于使用算法的客户。
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef _STRATEGY_H
#define _STRATEGY_H

#include <stdio.h>

namespace strategy_pattern
{
    /*
    * 动作行为（“飞行”）基类
    * 策略虚基类
    */
    class FlyBehavior
    {
        friend class Duck;

        virtual void fly() = 0;
    };

    //策略派生类1
    class FlyWithWings : public FlyBehavior
    {
        void fly()
        {
            printf("I am flying !\n");
        }
    };

    //策略派生类2
    class FlyNoWay : public FlyBehavior
    {
        void fly()
        {
            printf("I can not fly !\n");
        }
    };

	//策略派生类3
	class FlyRocketPowered : public FlyBehavior
	{
		void fly()
		{
			printf("I am flying with a rocket !\n");
		}
	};

    /*
    * 动作行为（“呱呱叫”）基类
    * 策略虚基类
    */
    class QuackBehavior
    {
        friend class Duck;

        virtual void quack() = 0;
    };

    //策略派生类1
    class Quack : public QuackBehavior
    {
        void quack()
        {
            printf("Quack !\n");
        }
    };

    //策略派生类2
    class MuteQuack : public QuackBehavior
    {
        void quack()
        {
            printf("<<Silent, I can not quack !>>\n");
        }
    };

	//策略派生类3
	class Squeak : public QuackBehavior
	{
		void quack()
		{
			printf("Squeak !\n");
		}
	};

    /*
    * 角色基类
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

    //角色派生类1
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

	//角色派生类2
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
