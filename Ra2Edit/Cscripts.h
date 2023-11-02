#pragma once


// Cscripts �Ի���
#include "Ra2EditDlg.h"
#include "afxcmn.h"
#include "afxwin.h"

class Cscripts : public CDialogEx
{
	DECLARE_DYNAMIC(Cscripts)

public:
	Cscripts(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Cscripts();

private:
	CRa2EditDlg* pRa2EditDlg;

// �Ի�������
	enum { IDD = IDD_SCRIPTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
private:
	CListCtrl m_Script_list;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnNMClickScriptList(NMHDR *pNMHDR, LRESULT *pResult);
private:
	// ���ݱ༭��
	CRichEditCtrl m_script_redit;
	// ���ⱸע
	CStatic m_tip_txt;
public:
	afx_msg void OnBnClickedButton5();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnLvnKeydownScriptList(NMHDR *pNMHDR, LRESULT *pResult);
private:
	// �Զ�����
	//CButton m_autosave_ck;
};
extern Cscripts ScriptsWind;
extern void ReadScript();
extern int Count_sc;
extern CString add_title_sc;
extern bool flag_add_sc;