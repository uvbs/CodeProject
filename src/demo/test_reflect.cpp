////////////////////////////////////////////////////////////////////////////////////
// 
// @file: test_reflect.cpp
// @author: by Mr.Chen
// @date: 2015/4/22/ 0:11
// @brief: 
//
////////////////////////////////////////////////////////////////////////////////////
#include "socketdef.h"
#include "test_reflect.h"

WX_IMPLEMENT_DYNAMIC_VIRTUAL(Shape, GETCLASS(tNode));
WX_IMPLEMENT_DYNAMIC(Rect, GETCLASS(Shape));
WX_IMPLEMENT_DYNAMIC(Square, GETCLASS(Rect));

void test_reflect()
{
	tKernel::getSinglePtr()->registerClass(GETCLASS(Shape));
	tKernel::getSinglePtr()->registerClass(GETCLASS(Rect));
	tKernel::getSinglePtr()->registerClass(GETCLASS(Square));

	Shape* pRect= (Shape*)tKernel::getSinglePtr()->newNode("Rect","root/","rect");
	pRect->draw();
	Square* pSquare = (Square*)(tKernel::getSinglePtr()->newNode("Square","root/rect/","square"));
	pSquare->draw();
}
