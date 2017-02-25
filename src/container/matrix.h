////////////////////////////////////////////////////////////////////////////////////
// 
// @file: 6-sMatrix.h
// @author: by Mr.Chen
// @date: 2015/4/22/ 1:21
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMATRIX_H
#define _SMATRIX_H

#include <iostream>
#include <time.h>
using namespace std;

//����Ԫ��
typedef struct 
{
	int i; //��
	int j; //��
	int e; //Ԫ��ֵ
}Triple;

//ϡ�����
class SparseMatrix
{
public:
	static const int MAXSIZE=1024;
	static const int MAXROW=10;

public:
	bool transpose(SparseMatrix &t);
	bool transpose_fast(SparseMatrix &t);
	bool multiple(SparseMatrix n, SparseMatrix &t);
	void print();

public:
	Triple data[MAXSIZE+1];	//data[0]δ�ã�������Ԫ���
	int rpos[MAXROW+1];//rpos[0]δ��
	int mu,nu,tu;	//����������������Ԫ����
};

#endif //_SMATRIX_H
