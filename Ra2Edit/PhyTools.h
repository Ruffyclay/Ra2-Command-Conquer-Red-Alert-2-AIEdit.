#pragma once
#include "stdafx.h"

inline long GetPhySize(FILE* fp){
	if (fp != NULL){
		long file_size;
		fseek(fp, 0L, SEEK_END);
		file_size = ftell(fp); // 整个文件的长度
		rewind(fp);
		fseek(fp, 0L, SEEK_SET);
		return file_size;
	}
	return -1;
}

// 创建备份文件 源文件名称, 备份文件名称
inline void ReadAndBackup(const char* resule, const char* filebackup){
	FILE* fp = NULL;
	FILE* bck = NULL;
	char* buffer = NULL;
	long file_size;

	fopen_s(&fp, resule, "r"); // 源文件
	fopen_s(&bck, filebackup, "w+"); // 备份的文件
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);

	buffer = (char*)malloc(file_size + 1);
	int n = fread(buffer, sizeof(char), file_size, fp);
	buffer[n] = '\0';

	fwrite("\0", sizeof(char), strlen("\0"), bck);
	fwrite(buffer, 1, n, bck);

	fclose(fp);
	fclose(bck);
	free(buffer);
	return;

}