/*
   Test program for TinyXML.
   ref from tiny xml
*/

#ifdef TIXML_USE_STL
	#include <iostream>
	#include <sstream>
	using namespace std;
#else
	#include <stdio.h>
#endif

#if defined( WIN32 ) && defined( TUNE )
	#include <crtdbg.h>
	_CrtMemState startMemState;
	_CrtMemState endMemState;
#endif

#include "tinyxml.h"

bool XmlTest (const char* testString, const char* expected, const char* found, bool noEcho = false);
bool XmlTest( const char* testString, int expected, int found, bool noEcho = false );
void NullLineEndings( char* p );
TiXmlNode* getSuccessor(TiXmlNode* node, const char * val);

//
// This file demonstrates some basic functionality of TinyXml.
// Note that the example is very contrived. It presumes you know
// what is in the XML file. But it does test the basic operations,
// and show how to add and remove nodes.
//

int test_XmlTest();
int test_XmlTest2();
