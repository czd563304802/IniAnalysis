#ifndef __INIANALYSIS_H__
#define __INIANALYSIS_H__



//��ȡһ���е����ݣ�������һ�й�꿪ʼ��λ��
int getLine(FILE * fd, int corsor, char * str);

//�жϽ����Ƿ�һ������ȷ���true������ȷ���false
bool Section(char * str, char * sectionName, char *allName);

//�жϼ���������ȷ��ؼ���ֵ������ȷ���nullptr
char *  Key(char * str, char * keyName);

//��ȡ���еĽڣ����ؽڵ��������������еĽ�������allName��
int GetAllSection(FILE * fd, char * allName);

//��ȡ�ַ������͵�ֵ����������ȡ���ַ����ĳ��ȣ������ַ�������returnString�У�ʧ���򷵻�defaultValue�ĳ��ȣ�������returnString
int MyGetString(char* section, char* key, char* defaultValue, char* returnString, int returnSize, char* fileName);

//��ȡ���͵�ֵ�������������ݣ�ʧ���򷵻�defaultValue������
int MyGetInt(char* section, char* key, int defaultValue, char* fileName);

#endif