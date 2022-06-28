#include <iostream>
#include <stdio.h>
#include "iniAnalysis.h"

using namespace std;
 
/**************************************************
功能：获取一整行的数据，返回下一行开头的光标位置
参数：
corsor：光标位置
fd：文件的io流
str：缓存区，缓存这一行的数据
****************************************************/
int getLine(FILE * fd, int corsor, char * str)
{
	fseek(fd, corsor, SEEK_SET);
	fread(str, 1, 512, fd);
	int i = 0;
	while (str[i] == '\r' || str[i] == '\n')    //消除上一行结尾剩余的\r和\n
	{
		i++;
	}
	if (i != 0)
	{
		corsor += i;
		fseek(fd, corsor, SEEK_SET);
		fread(str, 1, 512, fd);
		i = 0;
	}
	while (1)
	{
		if (str[i] == '\r' || str[i] == '\n')   //\r和\n当作一行的结尾
		{
			str[i] = '\0';
			break;
		}
		if (str[i] == '\0')
		{
			return 0;
		}
		i++;
	}
	return corsor + i + 1;
}


/*********************************************************
功能：
若sectionName和allName都为空，则只判断至一行是否为一节
若sectionName不为空，则判断这一行是否是sectionName这一节
若sectionName为空，allName不为空，则判断是否为一节，如果
是，则将这一节的名字存入allName中。

参数：
str：一行的数据
sectionName：需要比较的节的名字
allName：存放节名的缓冲区
**********************************************************/
bool Section(char * str, char * sectionName, char * allName)
{
	int i = 0;
	int j = 0;
	while (1)
	{
		while (str[i + j] == ' ')  //消除开头的空格
		{
			j++;
		}
		if (str[i] == ';')       //除空格以外的第一个字符是;则表示这一行是注释，直接返回false
		{
			return false;
		}
		if (str[i + j] != '[')  //第一个非空字符为'[ '表示有可能为节
			return false;
		else
		{
			while (str[i + j] != ']')   //'[ '后有是' ] '表示是一节
			{
				i++;
				if (str[i + j] == '\0')
				{
					return false;
				}
			}
			if (sectionName == nullptr)
			{
				if (allName == nullptr)
				{
					return true;
				}
				str[i + j] = '\0';
				strcat(allName, str + j + 1);
				strcat(allName, "\n");
				return true;
			}
			while (str[i + j] != ']')
			{
				i++;
			}
			str[i + j] = '\0';
			if (strcmp(str + j + 1, sectionName) == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}


/*********************************************************
功能：判断key是否是指定的key，是返回键的值，不是返回空

参数：
str：一行的数据
keyName：需要比较的键的名字
**********************************************************/
char *  Key(char * str, char * keyName)
{
	int i = 0;
	int j = 0;
	while (str[i] == ' ')
	{
		i++;
	}
	if (str[i] == ';')
	{
		return nullptr;
	}
	while (str[i + j] != '=')   //找到这一行第一个非空字符和'= '前最后一个非空字符，中间这一段就是这个键
	{
		j++;
	}
	int k = i + j + 1;
	j -= 1;
	while (str[i + j] == ' ')
	{
		j--;
	}
	str[i + j + 1] = '\0';
	if (strcmp(str + i, keyName) == 0)
	{
		char * ptr = new char[512];
		while (str[k] == ' ')            //找到'= '后第一个非空字符和结尾前最后一个非空字符，之间这一段就是键的值
		{
			k++;
		}
		int n = 0;
		while (str[k + n] != '\0')
		{
			n++;
		}
		while (str[k + n] == ' ')
		{
			n--;
		}
		str[k + n + 1] = '\0';
		strcpy(ptr, str + k);
		return ptr;
	}
	else
	{
		return nullptr;
	}
}


/*********************************************************
功能：获取所有的节的名字

参数：
fd：文件io流
allName：存放节名的缓冲区
**********************************************************/
int GetAllSection(FILE * fd, char * allName)
{
	int corsor = 0;
	memset(allName, '\0', strlen(allName));
	int i = 0;
	char str[512] = { '\0' };
	while (1)
	{
		corsor = getLine(fd, corsor, str);
		if (Section(str, nullptr, allName) == true)
			i++;
		if (corsor == 0)
		{
			return i;
		}
	}
	return i;
}


/*********************************************************
功能：获取所指定的节下面指定的键的值，成功返回值的长度，
并将值存入returnString，失败返回所设定的默认值，并将默
值存入returnString

参数：
section：指定的节
key：指定的键
defaultValue：默认值
returnString：缓冲区，存放所获取的值
returnSize：缓冲区的长度
fileName：文件的路径名
**********************************************************/
int MyGetString(char* section, char* key, char* defaultValue, char* returnString, int returnSize, char* fileName)
{
	FILE * fd = fopen(fileName, "r");
	char str[512] = { '\0' };
	int corsor = 0;
	while (1)
	{
		corsor = getLine(fd, corsor, str);
		if (Section(str, section, nullptr) == true)
		{
			break;
		}
		if (corsor == 0)
		{
			strcpy(returnString, defaultValue);
			return strlen(returnString);
		}
	}
	while (1)
	{
		corsor = getLine(fd, corsor, str);
		if (corsor == 0)
		{
			strcpy(returnString, defaultValue);
			return strlen(returnString);
		}
		if (Section(str, nullptr, nullptr) == true)
		{
			strcpy(returnString, defaultValue);
			return strlen(returnString);
		}
		int i = 0;
		int j = 0;
		while (str[i] == ' ')
		{
			i++;
		}
		if (str[i] == ';')
		{
			continue;
		}
		while (str[i + j] != '=')
		{
			j++;
		}
		int k = i + j + 1;
		j -= 1;
		while (str[i + j] == ' ')
		{
			j--;
		}
		str[i + j + 1] = '\0';
		if (strcmp(str + i, key) == 0)
		{
			while (str[k] == ' ')
			{
				k++;
			}
			int n = 0;
			while (str[k + n] != '\0')
			{
				n++;
			}
			while (str[k + n] == ' ')
			{
				n--;
			}
			str[k + n + 1] = '\0';
			strcpy(returnString, str + k);
			return strlen(returnString);
		}
	}
}


/*********************************************************
功能：获取所指定的节下的指定键的名字，获取的要是int类型

参数：
section：指定的节
key：指定的键
defaultValue：默认值
fileName：文件的路径名
**********************************************************/
int MyGetInt(char* section, char* key, int defaultValue, char* fileName)
{
	char ptr[512] = { "\0" };
	MyGetString(section, key, "123", ptr, 512, fileName);
	if (strcmp(ptr, "123") == 0)
	{
		return defaultValue;
	}
	else
	{
		if (strspn(ptr, "0123456789") == strlen(ptr))
		{
			return atoi(ptr);
		}
		else
		{
			return defaultValue;
		}
	}
}
