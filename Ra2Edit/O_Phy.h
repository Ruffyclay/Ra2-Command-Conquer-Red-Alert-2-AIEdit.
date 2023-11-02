#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char a;

// 打开文件，返回路径（CString）
inline CString FileFuction_Open(){
	// 打开AI.ini文件
	CString filePath = TEXT("None");
	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("AI配置 文件(AI*.ini)|AI*.ini|所有文件(*.*)|*.*||"));
	if (fileDialog.DoModal() != IDOK) {
		return filePath;
	}
	// 获取所选文件的路径
	filePath = fileDialog.GetPathName();
	// 将所选文件的路径存储到成员变量中，方便保存时使用
	return filePath;
}

// 保存文件
inline void FileFunction_Save(const char* resfile, const char* Bckfile) {
	FILE* SAVE_OK = NULL;
	ReadAndBackup(Bckfile, resfile); // 接收的文件
	fopen_s(&SAVE_OK, resfile, "r"); // 检查是否完成保存
	if (SAVE_OK != NULL){
		AfxMessageBox(TEXT("保存成功!"));
	}
	else{
		AfxMessageBox(TEXT("保存失败!"));
	}
	fclose(SAVE_OK);
	remove(Bckfile); // 删除备份文件
}