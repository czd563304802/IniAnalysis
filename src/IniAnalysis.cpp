#include <iostream>
#include <stdio.h>
#include "iniAnalysis.h"

using namespace std;
 
/**************************************************
���ܣ���ȡһ���е����ݣ�������һ�п�ͷ�Ĺ��λ��
������
corsor�����λ��
fd���ļ���io��
str����������������һ�е�����
****************************************************/
int getLine(FILE * fd, int corsor, char * str)
{
	fseek(fd, corsor, SEEK_SET);
	fread(str, 1, 512, fd);
	int i = 0;
	while (str[i] == '\r' || str[i] == '\n')    //������һ�н�βʣ���\r��\n
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
		if (str[i] == '\r' || str[i] == '\n')   //\r��\n����һ�еĽ�β
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
���ܣ�
��sectionName��allName��Ϊ�գ���ֻ�ж���һ���Ƿ�Ϊһ��
��sectionName��Ϊ�գ����ж���һ���Ƿ���sectionName��һ��
��sectionNameΪ�գ�allName��Ϊ�գ����ж��Ƿ�Ϊһ�ڣ����
�ǣ�����һ�ڵ����ִ���allName�С�

������
str��һ�е�����
sectionName����Ҫ�ȽϵĽڵ�����
allName����Ž����Ļ�����
**********************************************************/
bool Section(char * str, char * sectionName, char * allName)
{
	int i = 0;
	int j = 0;
	while (1)
	{
		while (str[i + j] == ' ')  //������ͷ�Ŀո�
		{
			j++;
		}
		if (str[i] == ';')       //���ո�����ĵ�һ���ַ���;���ʾ��һ����ע�ͣ�ֱ�ӷ���false
		{
			return false;
		}
		if (str[i + j] != '[')  //��һ���ǿ��ַ�Ϊ'[ '��ʾ�п���Ϊ��
			return false;
		else
		{
			while (str[i + j] != ']')   //'[ '������' ] '��ʾ��һ��
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
���ܣ��ж�key�Ƿ���ָ����key���Ƿ��ؼ���ֵ�����Ƿ��ؿ�

������
str��һ�е�����
keyName����Ҫ�Ƚϵļ�������
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
	while (str[i + j] != '=')   //�ҵ���һ�е�һ���ǿ��ַ���'= 'ǰ���һ���ǿ��ַ����м���һ�ξ��������
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
		while (str[k] == ' ')            //�ҵ�'= '���һ���ǿ��ַ��ͽ�βǰ���һ���ǿ��ַ���֮����һ�ξ��Ǽ���ֵ
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
���ܣ���ȡ���еĽڵ�����

������
fd���ļ�io��
allName����Ž����Ļ�����
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
���ܣ���ȡ��ָ���Ľ�����ָ���ļ���ֵ���ɹ�����ֵ�ĳ��ȣ�
����ֵ����returnString��ʧ�ܷ������趨��Ĭ��ֵ������Ĭ
ֵ����returnString

������
section��ָ���Ľ�
key��ָ���ļ�
defaultValue��Ĭ��ֵ
returnString�����������������ȡ��ֵ
returnSize���������ĳ���
fileName���ļ���·����
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
���ܣ���ȡ��ָ���Ľ��µ�ָ���������֣���ȡ��Ҫ��int����

������
section��ָ���Ľ�
key��ָ���ļ�
defaultValue��Ĭ��ֵ
fileName���ļ���·����
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
