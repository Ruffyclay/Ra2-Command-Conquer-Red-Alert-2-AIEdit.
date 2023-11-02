#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHECK_HEAD 0
#define CHECK_END 1

/*
* 文本文件的操作主要是操作文件指针
* FILE *fp; // 结构体
* fopen_s(&fp, filename, "r+"); 打开文件
* fseek(文件指针,偏移量,偏移方式)
* 文件指针的有三种偏移方式SEEK_CUR,SEEK_SET,SEEK_END分别是当前，开头，结尾
* fwrite(内容, 每次写入的大小(一般为1)，写入次数(一般用strlen()或sizeof())
* , 文件指针);
* fread(存放内容的数组,每次写入的大小(一般为sizeof(数组[0])
* ，读取的次数(一般为sizeof(数组)),文件指针))
* fputs和fgetc还有fputc和fgetc分别是在当前指针的后面写入或读取字符串
* ，或者是1个字符，一般读取都是一行字符串。
* ftell()函数可以获取文件的大小从而可以动态分配内存,malloc()，使用完过后，要用free(变量)释放被分配的内存
*/

/*
* str2在str1中最后的位置 CHECK_HEAD 在前 CHECK_END 在后
*/
inline int strinstr(const char* str1, const char* str2, int mode) {
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if (len1 < len2) {
		return -1;
	}
	for (int i = 0; i <= len1 - len2; i++) {
		int j = 0;
		while (j < len2 && str1[i + j] == str2[j]) {
			j++;
		}
		if ((j == len2)&&(mode == 1)) {
			return i + strlen(str2) + 1;
		}
		else if ((j == len2) && (mode == 0)){
			return i;
		}
	}
	return -1;
}
inline int count_valid_elements(char *arr, int len, char null) { // 统计数组有效位的个数,null是省略值
	int count = 0;
	CString message;
	for (int i = 0; i < len; i++) {
		if (arr[i] != null) {  // 判断
			count++;
		}
	}
	message.Format(_T("%c,%d"),arr,count);
	AfxMessageBox(message);
	return count;
}

// 改成内存读取方式
// 读取指定区间的内容 fp,posStart posEnd
inline CString GetFileContent(const char* text, long int posStart, long int posEnd) {
	if (text=="" || posStart > posEnd) {
		return (CStringW)"";
	}

	// 获取文件大小
	long fileSize = strlen(text);

	// 校验文件指针起始点和结束点的有效性
	if (posStart >= fileSize || posEnd >= fileSize) {
		return (CString)"";
	}

	// 计算需要读取的字节数
	int numBytes = posEnd - posStart;

	// 分配足够的内存存储文件内容
	char* content = (char*)malloc(numBytes + 1);
	if (!content) {
		return (CString)"";
	}

	// 读取文件内容
	for (long i = posStart; i < posEnd; i++)
	{
		content[i] = text[posStart + i];
	}

	// 添加字符串结束符
	content[numBytes] = '\0';

	// 转换为CString类型
	CString str(content);

	// 释放内存
	//free(content);

	return str;
}



// 打开文件，在指定内容下面进行追加内容 打开的文件指针 , 指定的内容，追加的内容
inline void file_append_below(FILE* fp, long int pos, const char data[]) {
	char line[1024] = { 0 };
	char* buffer = NULL;
	long file_size;
	long countpos = 0; // 统计总偏移次数

	fseek(fp, 0L, SEEK_END); // 将光标定位到末尾
	file_size = ftell(fp); // 获取文件的大小
	rewind(fp); // 重置文件指针
	if (pos != -1) { // 如果找到了指定的内容
		// 将光标移动到指定位置，然后将这剩下的内容保存到缓冲区
		fseek(fp, pos, SEEK_SET);
		buffer = (char*)malloc(file_size - pos + 1); // 动态分配内存
		int n = fread(buffer, sizeof(char), file_size - pos, fp);
		buffer[n] = '\0'; // 在末尾添加'\0'作为字符串结尾

		// 重新调整文件指针，写入数据到文件
		fseek(fp, pos, SEEK_SET);
		fwrite(data, sizeof(char), strlen(data), fp);

		// 插入完过后，将消失的内容补到末尾
		fwrite(buffer, 1, n, fp);
	}
	free(buffer);
}

// 查找指定字符串并返回它的指针位置
// 指定文件指针(已打开), 指定目标, 指定开始位置,返回模式CHECK_HEAD|CHECK_END
inline long file_find_target(FILE* fp, const char* target, long int pos, int mode){
	bool flag = false;
	long countpos = pos; // 指针开始的位置
	if (fp != NULL){ // 如果文件指针打开了
		long file_size;
		int len = strlen(target); // 目标长度
		fseek(fp, 0L, SEEK_END);
		file_size = ftell(fp); // 整个文件的长度
		rewind(fp);
		fseek(fp, 0L, SEEK_SET);

		CString mes;

		// 将文件内容加载到内存进行读写加速
		char* buffer = NULL;
		buffer = (char*)malloc(file_size + 1);
		int n = fread(buffer, sizeof(char), file_size, fp);
		buffer[n] = '\0'; // 末尾结束符号,此时buffer大小是整个文件大小且为空的数组
		fwrite(buffer, 1, n, fp); // buffer存入指针全部数据，此时指针位置为0

		//testCheck(get_last_dir(CStringToChar(FilePath_main)));

		if (strlen(target) > file_size) return -1;
		while (len + countpos <= file_size){ // 满足条件才能开始
			if (buffer[countpos] == target[0]){ // 第一个字符正确
				flag = true;
				for (int i = 0; i < len; i++){ // 检查剩下的字符
					// 在此期间，指针不变化，只偏移
					mes.Format(_T("%d,%c,%c"), countpos,buffer[countpos + i],target[i]);
					if (buffer[countpos + i] != target[i]){
						flag = false;
						break; // 跳出循环
					}
					AfxMessageBox(mes);
				}
			}
			if (flag){
				break;
			}
			countpos += 1;
		}
		free(buffer);

		if ((flag) && (mode == 1)){ // 返回尾巴
			return (long int)countpos + strlen(target) + 1;
		}
		else if ((flag) && (mode == 0)){ // 返回头部
			return (long int)countpos - 1;
		}
		else{ // 返回错误
			return -1;
		}
	}
}

inline CString file_checkinclude_con(FILE* fp){
	long poss, pose,file_size;
	char* txt = NULL;
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp); // 整个文件的长度
	txt = (char*)malloc(file_size + 1);
	rewind(fp);
	fread(txt, sizeof(char), file_size, fp);
	txt[file_size] = '\0';
	poss = file_find_target(fp, "[#include]", 0, CHECK_END);
	pose = file_find_target(fp, "[", poss, CHECK_HEAD);
	return GetFileContent(txt,poss,pose);
}

