#pragma once
#include "afxcmn.h"
#include "Ra2EditDlg.h"
#include "afxwin.h"

// CTaskForces �Ի���

class CTaskForces : public CDialogEx
{
	DECLARE_DYNAMIC(CTaskForces)

public:
	CTaskForces(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTaskForces();

// �Ի�������
	enum { IDD = IDD_TASK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// ��ǲ�����б�
	CListCtrl m_TaskForces_list;
public:

	afx_msg void OnBnClickedButton2();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
	// �༭��
	CRichEditCtrl m_task_redit;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNMClickTaskList(NMHDR *pNMHDR, LRESULT *pResult);
private:
	// ����
	CStatic m_tip_txt;
public:
	afx_msg void OnLvnKeydownTaskList(NMHDR *pNMHDR, LRESULT *pResult);
	// �Զ�����
	//CButton m_autosave_ck;
};
extern CTaskForces TaskForceWind;
extern void ReadTask();
extern int Count_ta;
extern bool flag_add_task;
extern CString add_title_ta;