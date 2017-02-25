#include "matrix.h"

bool SparseMatrix::transpose(SparseMatrix &t)
{
	t.mu=nu;
	t.nu=mu;
	t.tu=tu;
	if(t.tu)
	{
		//��������˳��洢
		int q=1;
		for(int col=1;col<=nu;col++)
		for(int p=1;p<=tu;p++)
		{
			if(data[p].j==col)
			{
				t.data[q].i=data[p].j;
				t.data[q].j=data[p].i;
				t.data[q].e=data[p].e;
				q++;
			}
		}
	}
	return true;
}

bool SparseMatrix::transpose_fast(SparseMatrix &t)
{
	int col=0;
	int p=0;
	int q=0;
	t.mu=nu;
	t.nu=mu;
	t.tu=tu;
	int *num=new int[nu+1];
	int *cpot=new int[nu+1];
	for(col=1;col<=nu;col++)
	{
		num[col]=0;
		cpot[col]=0;
	}
	if(t.tu)
	{
		//Ԥ�ȼ����ÿ��Ԫ��Ӧ�ñ����õ�λ��
		for(p=1;p<=tu;p++)
		{
			++num[data[p].j];
		}
		//ÿ�е���Ԫ��λ�ü���ú����cpot��
		cpot[1]=1;
		for(col=2;col<=nu;col++)
		{
			cpot[col]=cpot[col-1]+num[col-1];
		}
		for(p=1;p<=tu;p++)
		{
			col=data[p].j;
			q=cpot[col];
			t.data[q].e=data[p].e;
			t.data[q].i=data[p].j;
			t.data[q].j=data[p].i;
			cpot[col]++; //׼�����е��¸�Ԫ��λ��
		}
	}
	return true;
}

void SparseMatrix::print()
{
	int p=1;
	for(;p<=tu;p++)
	{
		cout<<"("<<data[p].i<<","<<data[p].j
			<<","<<data[p].e<<")"<<endl;
	}
	cout<<endl;
}

bool SparseMatrix::multiple(SparseMatrix n, SparseMatrix &t)
{
	int row;
	int col;
	int p;
	int tp;
	int brow;
	int btp;
	int q;
	if(nu!=n.mu)
		return false;
	t.mu=mu;
	t.nu=n.nu;
	t.tu=0;
	int *ctemp=new int[t.nu+1];//ctemp[0]δ��
	if(tu*n.tu!=0)
	{
		for(row=1;row<=mu;row++)//����ÿһ��
		{
			for(int i=1;i<=t.nu;i++)
				ctemp[i]=0;
			t.rpos[row]=t.tu+1;
			if(row<mu) tp=rpos[row+1];
			else tp=tu+1;
			for(p=rpos[row];p<tp;p++)//����ǰ��
			{
				brow=data[p].j;
				if(brow<n.mu) btp=n.rpos[brow+1];
				else btp=n.tu+1;
				for(q=n.rpos[brow];q<btp;q++)
				{
					col=n.data[q].j;
					ctemp[col]+=data[p].e*n.data[q].e;
				}
			}
			for(col=1;col<=t.nu;col++)
			{
				if(ctemp[col])
				{
					if(++t.tu>MAXSIZE) 
						return false;
					t.data[t.tu].e=ctemp[col];
					t.data[t.tu].i=row;
					t.data[t.tu].j=col;
				}
			}
		}//for row
	}
	return true;
}
