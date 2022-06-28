#ifndef __INIANALYSIS_H__
#define __INIANALYSIS_H__



//获取一整行的数据，返回下一行光标开始的位置
int getLine(FILE * fd, int corsor, char * str);

//判断节名是否一样，相等反回true，不相等返回false
bool Section(char * str, char * sectionName, char *allName);

//判断键名，若相等返回键的值，不相等返回nullptr
char *  Key(char * str, char * keyName);

//获取所有的节，返回节的数量，并包所有的节名存入allName中
int GetAllSection(FILE * fd, char * allName);

//获取字符串类型的值，返回所获取的字符串的长度，并把字符串存入returnString中，失败则返回defaultValue的长度，并存入returnString
int MyGetString(char* section, char* key, char* defaultValue, char* returnString, int returnSize, char* fileName);

//获取整型的值，返回整型数据，失败则返回defaultValue的数据
int MyGetInt(char* section, char* key, int defaultValue, char* fileName);

#endif