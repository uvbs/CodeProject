#include "array.h"
#include <string.h>

Array::Array(int dim, ...)
{
	int i;
	int elemTotal=1;
	if(dim<1 || dim>MAX_ARRAY_DIM)
	{
		return;
	}
	m_dim=dim;
	m_bounds=new int[m_dim];
	if(!m_bounds)
	{
		return;
	}
	//求出a的元素总数elemTotal
	va_list ap;
	va_start(ap,dim);
	for(i=0;i<dim;i++)
	{
		m_bounds[i]=va_arg(ap,int);
		if(m_bounds[i]<0)
		{
			return;
		}
		elemTotal*=m_bounds[i];
	}
	va_end(ap);
	//分配数组元素空间
	m_base=new int[elemTotal];
	if(!m_base)
	{
		return;
	}
	//cout<<elemTotal<<endl;
	for(i=0;i<elemTotal;i++)
	{
		m_base[i]=i;
	}
	//求出数组映像函数常量
	m_constants=new int[m_dim];
	if(!m_constants)
	{
		return;
	}
	m_constants[dim-1]=1;
	for(i=dim-2;i>=0;i--)
	{
		m_constants[i]=m_bounds[i+1]*m_constants[i+1];
	}
	return;
}

Array::~Array()
{
	delete m_base;
	m_base=NULL;

	delete m_bounds;
	m_bounds=NULL;

	delete m_constants;
	m_constants=NULL;
}

int Array::locate(va_list ap)
{
	int off=0;
	for(int i=m_dim-1;i>=0;i--)
	{
		int ind=va_arg(ap,int);
		if(ind<0 || ind>=m_bounds[i])
			return -1;
		off+=m_constants[i]*ind;
	}
	return off;
}

bool Array::get(int* e,...)
{
	va_list ap;
	va_start(ap,e);
	int off = 0;
	//if((off=locate(ap))<0)
	//	return false;
	for(int i=0;i<m_dim;++i)
	{
		int ind=va_arg(ap,int);
		if(ind<0 || ind>=m_bounds[i])
			return false;
		off+=m_constants[i]*ind;
	}
	*e=*(m_base+off);
	return true;
}

bool Array::set(int e,...)
{
	va_list ap;
	va_start(ap,e);
	int off=0;
	for(int i=0;i<m_dim;i++)
	{
		int ind=va_arg(ap,int);
		if(ind<0 || ind>=m_bounds[i])
		{
			return false;
		}
		off+=m_constants[i]*ind;
	}
	*(m_base+off)=e;
	return true;
}
