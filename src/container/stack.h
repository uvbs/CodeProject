///////////////////////////////////////////////////////////////////////////////////
// 
// @file: stack.h
// @author: by Mr.Chen
// @date: 2014/9/15	14:10
// @brief: ջ
//
////////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef __STACK_H_
#define  __STACK_H_

#include "myassert.h"
#include <stdio.h>

namespace container {

template<class ElemType>
class Stack
{
public:
    enum 
    {
        INIT_STACK_SIZE = 100,
    };

public:
    Stack(int ms)
    {
        if (ms <= 0 || ms > INIT_STACK_SIZE)
        {
            //cerr << "Stack InitSize Error!" << endl;
            ms = INIT_STACK_SIZE;
        }
        _base = new ElemType[ms];
        if (!_base)
        {
            //cerr << "Memory allocation failure!" << endl;
            MyAssert(0);
        }
        _top = -1;
        _size = ms;
    }

    void push(ElemType item)
    {
        if (_top == _size-1)
        {
            alloc_stack();
        }
        _top++;
        _base[_top] = item;
    }

    ElemType pop()
    {
        if (_top == -1)
        {
            //cerr << "Stack is empty!" << endl;
            MyAssert(0);
        }
        _top--;
        return _base[_top + 1];
    }

    ElemType peek()
    {
        if (_top == -1)
        {
            //cerr << "Stack is empty!" << endl;
            MyAssert(0);
        }
        return _base[_top];
    }

    bool empty() {return _top == -1;}
    int length() {return _top + 1; }

    bool find(ElemType e)
    {
        for (int i = 0; i <= _top; ++i)
        {
            if (_base[i] == e)
            {
                return true;
            }
        }
        return false;
    }

    void put()
    {
        if (_top == -1)
        {
            //cerr << "Stack is empty!" << endl;
            MyAssert(0);
            return;
        }
        for (int i = 0; i <= _top; ++i)
        {
            //cout << _base[i] << " ";
            //printf("%d, "_base[i]);
        }
        //cout << endl;
        printf("\n");
    }

    void put(ElemType *array)
    {
        int i = -1;
        if (_top == -1)
        {
            //cerr << "Stack is empty!" << endl;
            MyAssert(0);
            return;
        }
        for (int i = 0; i <= _top; ++i)
        {
            array[i] = _base[i];
        }
    }

    ~Stack()
    {
        delete []_base;
        _base = 0;
        _top = -1;
        _size = 0;
    }

protected:
    void alloc_stack()
    {
        _size *= 2;
        ElemType *p = new ElemType[_size];
        for (int i = 0; i <= _top; i++)
        {
            p[i] = _base[i];
        }
        delete []_base;
        _base = p;
    }

private:
    ElemType *_base; //ջ��ָ��
    int _top; //ջ��
    int _size; //ջ�ռ�size
};

/////////////////////////////////////////////////////////////////////////////////////
//��׺ת��׺
void change(char* s1, char* s2);
//���ݺ�׺���ʽ��ֵ
double compute_express(char* str);
//��׺���ʽ��ֵ
void compute();
//����ת��
void convert(int n, int r);
//����ƥ��
int match();

/////////////////////////////////////////////////////////////////////////////////////
//��������
/*
T=10
�������Ϊ:{1,8,4,3,5,2}
4���:(1,4,3,2)
(1,4,5)
(8,2)
(3,5,2)
*/
typedef struct Weight
{
    int i;
    int wi;
}WEIGHT;

//�ݹ�
bool knapsack(int w[], int T, int n);
//�ǵݹ�
void knapsack_v2(int w[], int T, int n);

/////////////////////////////////////////////////////////////////////////////////////
//akm����
typedef struct AkmNode
{
    unsigned int vm;
    unsigned int vn;
}AkmNode;

//�ݹ�
unsigned int akm(unsigned int m, unsigned int n);
//�ǵݹ�
unsigned int akm_v2(unsigned int m, unsigned int n);


/////////////////////////////////////////////////////////////////////////////////////
//Ackerman����
typedef struct AckerNode
{
    int nval;
    int xval;
    int yval;
}AckerNode;

//�ݹ�
int ackerman(int n, int x, int y);
//�ǵݹ�
int ackerman_v2(int n, int x, int y);

/////////////////////////////////////////////////////////////////////////////////////
//ȫ��������
//�ݹ�
void permute(int a[], int s, int n);
//�ǵݹ�
void permute_v2(int a[],  int n);

} //namespace container
using namespace container;

#endif //__STACK_H_

