#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char a;

// ���ļ�������·����CString��
inline CString FileFuction_Open(){
	// ��AI.ini�ļ�
	CString filePath = TEXT("None");
	CFileDialog fileDialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("AI���� �ļ�(AI*.ini)|AI*.ini|�����ļ�(*.*)|*.*||"));
	if (fileDialog.DoModal() != IDOK) {
		return filePath;
	}
	// ��ȡ��ѡ�ļ���·��
	filePath = fileDialog.GetPathName();
	// ����ѡ�ļ���·���洢����Ա�����У����㱣��ʱʹ��
	return filePath;
}

// �����ļ�
inline void FileFunction_Save(const char* resfile, const char* Bckfile) {
	FILE* SAVE_OK = NULL;
	ReadAndBackup(Bckfile, resfile); // ���յ��ļ�
	fopen_s(&SAVE_OK, resfile, "r"); // ����Ƿ���ɱ���
	if (SAVE_OK != NULL){
		AfxMessageBox(TEXT("����ɹ�!"));
	}
	else{
		AfxMessageBox(TEXT("����ʧ��!"));
	}
	fclose(SAVE_OK);
	remove(Bckfile); // ɾ�������ļ�
}