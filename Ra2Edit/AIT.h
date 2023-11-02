#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CAIT �Ի���

class CAIT : public CDialogEx
{
	DECLARE_DYNAMIC(CAIT)

public:
	CAIT(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAIT();

// �Ի�������
	enum { IDD = IDD_AIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
private:
	CListCtrl m_AIT_list;
	// ����ע��
	CStatic m_tip_txt;
	// Name����
	CEdit m_name_edit;
	// �򵥵��˿���
	CButton m_EasyEnemy_check;
	// �еȵ��˿���
	CButton m_NormalEnemy_check;
	// ���ѵ��˿���
	CButton m_HardEnemy_check;
	// ָ������
	CComboBox m_Country_ccb;
	// �Ƽ��ȼ�
	CEdit m_Tech_edit;
	// ��������
	CComboBox m_atttype_ccb;
	// ������λ
	CComboBox m_aittunit_ccb;
	// ������ֵ
	CEdit m_value_edit;
	// ָ����Ӫ
	CComboBox m_Unity_ccb;
	// ���Ȩ��
	CComboBox m_lowaitt_ccb;
	// ����Ȩ��
	CComboBox m_normalaitt_ccb;
	// ���Ȩ��
	CComboBox m_heightaitt_ccb;
public:
	afx_msg void OnNMSetfocusAitList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickAitList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickAitList(NMHDR *pNMHDR, LRESULT *pResult);
private:
	// ��ս����A
	CComboBox m_teamA_ccb;
	// ����ģʽ
	CComboBox m_aittmod_ccb;
public:
	afx_msg void OnLvnKeydownAitList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnCbnSelchangeCombo9();
	afx_msg void OnCbnEditchangeCombo9();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnKillfocusCombo2();
	afx_msg void OnCbnDblclkCombo2();
	afx_msg void OnCbnEditchangeCombo2();
	afx_msg void OnCbnSetfocusCombo2();
	afx_msg void OnCbnDropdownCombo2();
	afx_msg void OnCbnSelchangeCombo8();
	afx_msg void RefreshControl(UINT uCtlID);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedCheck4();
	// �Զ���ת
	CButton m_autoturn_check;
private:
	// ��ת��ť
	CButton c_turn;
	// �Զ�����
	//CButton m_autosave_ck;
};
extern CAIT AITWind;
extern void ReadAIT();
extern bool flag_add_ait;
extern CString add_ait_title;
extern CString flag_fresh_te_title;