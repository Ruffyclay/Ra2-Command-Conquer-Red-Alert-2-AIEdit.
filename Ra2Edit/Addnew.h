#pragma once
#include "afxwin.h"


// CAddnew �Ի���

class CAddnew : public CDialogEx
{
	DECLARE_DYNAMIC(CAddnew)

public:
	CAddnew(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddnew();

// �Ի�������
	enum { IDD = IDD_NEWADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// �������ݵ�����
	CEdit m_add_name_edit;
	// �������ݵ�����
	CComboBox m_add_type_ccb;
	virtual void OnOK();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
extern CAddnew add_wind;
extern char add_type;
extern bool Add_new_init;
extern CString add_new_title;
