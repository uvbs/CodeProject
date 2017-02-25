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

//矩阵元素
typedef struct 
{
	int i; //行
	int j; //列
	int e; //元素值
}Triple;

//稀疏矩阵
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
	Triple data[MAXSIZE+1];	//data[0]未用，非零三元组表
	int rpos[MAXROW+1];//rpos[0]未用
	int mu,nu,tu;	//行数，列数，非零元个数
};

#endif //_SMATRIX_H
