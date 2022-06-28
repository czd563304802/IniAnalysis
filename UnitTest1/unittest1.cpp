#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include "E:/LIB_And_DLL/IniAnalysis/include/IniAnalysis.h"
#define PATHNAME "E:/work/VisualStudio/.dll/IniAnalysis/test.ini"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			FILE * fd = fopen(PATHNAME, "r");
			int i = 0;
			char str[512] = {"\0"};
			i = getLine(fd,i,str);
			Assert::AreEqual(7, i);
			i = getLine(fd, i, str);
			int n =strlen( Key(str, "Name"));
			Assert::AreEqual(17,n);
			Assert::AreEqual(16, MyGetInt("FF16", "Version", 123, PATHNAME));
			MyGetString("FF16", "Name", "FF15", str, 512, PATHNAME);
			cout << str << endl;
			Assert::AreEqual(17, MyGetString("FF16", "Name", "FF15", str, 512, PATHNAME));
			Assert::AreEqual(21, MyGetString("WOLONG", "Name", "FF15", str, 512, PATHNAME));
			Assert::AreEqual(4, MyGetString("FF16", "1", "FF15", str, 512, PATHNAME));
			Assert::AreEqual(3,GetAllSection(fd, str));
			// TODO:  在此输入测试代码
		}
	};
}