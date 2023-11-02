// AIT.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ra2Edit.h"
#include "AIT.h"
#include "afxdialogex.h"
#include "OtherTools.h"
#include "Army.h"

CAIT AITWind;
CString flag_fresh_te_title;
// CAIT �Ի���
// �Ƿ�ִ���������ݲ���
bool flag_add_ait = false;
CString add_ait_title;
int count_ait_num;
bool fresh_ait_list = false;

// ��ȡֵ�ù�ϣ��
const std::unordered_map<std::string, int> chineseToNumber{
	{ "������", -1 },
	{ "�з�ӵ��(ǰ��)ָ����λ", 0 },
	{ "�ҷ�ӵ��(ǰ��)ָ����λ", 1 },
	{ "�з�����Ϊ��", 2 },
	{ "�з�����Ϊ��", 3 },
	{ "�з���Ǯ���ڻ����", 4 },
	{ "������Ļװ�þ���", 5 },
	{ "������ʱ�մ��;���", 6 },
	{ "������ӵ��(ǰ��)ָ����λ", 7 }
};

int getmode(const std::string& chinese) {
	auto iter = chineseToNumber.find(chinese);
	if (iter != chineseToNumber.end()) {
		return iter->second;
	}
	else {
		return -1; // �����ָ�� Ĭ��������
	}
}

std::unordered_map<std::string, int> Ruffylistmod{
	{ "С��", 0 },
	{ "С�ڵ���", 1 },
	{ "����", 2 },
	{ "���ڵ���", 3 },
	{ "����", 4 },
	{ "������", 5 }
};

int Ruffyfind(const std::string& key) {
	if (Ruffylistmod.count(key) > 0) {
		return Ruffylistmod[key];
	}
	return 5; // û���ҵ���Ӧ��ֵ������Ĭ��ֵ
}


// ��ȡAITT����
void ReadAIT(){
	int poss, pose, len, len_temp_txt, psN = 0;
	char* temp, *temp_txt, *Line_num_txt = NULL;
	CString num, title, content, note, txt;
	while (psPos[psN].pos != NULL){
		poss = TxtMemFind(psPos[psN].size, 0, psPos[psN].pos, "[AITriggerTypes]", C_END);
		if (poss == -1){ psN += 1; continue; }
		pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "[", C_HEAD);
		if (pose == -1){ // ����Ҳ�������,ֱ�ӳ�����ĩβ
			pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, '\0', C_HEAD);
		}
		if (poss == pose){ psN += 1; continue; }
		// ��ȡ�����м��ֵ
		temp = ReadMemContent(psPos[psN].pos, psPos[psN].size, poss, pose);
		len = strlen(temp);
		// ��ȥ�м�ո񲢱���
		temp_txt = GetMemLines(temp, len); // ����ؽ�����Դ�ļ��޹�
		len_temp_txt = GetMemLinesNum(temp);
		
		// ��ʼ���봥������
		struct AITriggerTypes temp_t;
		for (int i = 0; i < len_temp_txt; i++)
		{
			// ѭ����ȡÿһ������Ȼ����뵽�ڴ�
			Line_num_txt = GetMemLine(temp_txt, i);
			// ���������Ƿ��д���
			if (CheckAITedit(Line_num_txt)){
				title = GetTargetString(Line_num_txt, strlen(Line_num_txt), "=", R_BF);
				txt = GetTargetString(Line_num_txt, strlen(Line_num_txt), "=", R_BH);

				// �������е�����,��̬����
				temp_t.title = CStringToChar(title);
				temp_t.txt = CStringToChar(txt);
				aitriggertypes.push_back(temp_t);
			}
		}
	psN += 1;
	}
	main_aitt.reserve(strlen("[AITriggerTypes]\n"));
	main_aitt.insert(main_aitt.end(), "[AITriggerTypes]\n", "[AITriggerTypes]\n" + std::strlen("[AITriggerTypes]\n"));
	int lenCount = 0;
	count_ait_num = aitriggertypes.size() - 1;
	for (int i = 0; i < aitriggertypes.size(); i++) {
		std::stringstream ss;
		ss << aitriggertypes[i].title << "=" << aitriggertypes[i].txt;
		std::string temp_str = ss.str();
		lenCount += temp_str.length();
		main_aitt.reserve(lenCount + 1); // Ϊ���з�Ԥ��һ���ֽ�
		main_aitt.insert(main_aitt.end(), temp_str.c_str(), temp_str.c_str() + temp_str.length());
		main_aitt.push_back('\n');
		aitriggertypes[i].title = NULL;
		aitriggertypes[i].txt = NULL;
	}
	main_aitt.push_back('\n');
}

void CAIT::RefreshControl(UINT uCtlID)
{
	CRect rc;
	GetDlgItem(uCtlID)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	InvalidateRect(rc);
	UpdateWindow();
}

IMPLEMENT_DYNAMIC(CAIT, CDialogEx)

CAIT::CAIT(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAIT::IDD, pParent)
{

}

CAIT::~CAIT()
{
}

void CAIT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AIT_LIST, m_AIT_list);
	DDX_Control(pDX, IDC_STIP, m_tip_txt);
	DDX_Control(pDX, IDC_EDIT1, m_name_edit);
	DDX_Control(pDX, IDC_CHECK1, m_EasyEnemy_check);
	DDX_Control(pDX, IDC_CHECK2, m_NormalEnemy_check);
	DDX_Control(pDX, IDC_CHECK3, m_HardEnemy_check);
	DDX_Control(pDX, IDC_COMBO1, m_Country_ccb);
	DDX_Control(pDX, IDC_EDIT2, m_Tech_edit);
	DDX_Control(pDX, IDC_COMBO2, m_atttype_ccb);
	DDX_Control(pDX, IDC_COMBO7, m_aittunit_ccb);
	DDX_Control(pDX, IDC_EDIT6, m_value_edit);
	DDX_Control(pDX, IDC_COMBO6, m_Unity_ccb);
	DDX_Control(pDX, IDC_COMBO3, m_lowaitt_ccb);
	DDX_Control(pDX, IDC_COMBO4, m_normalaitt_ccb);
	DDX_Control(pDX, IDC_COMBO5, m_heightaitt_ccb);
	DDX_Control(pDX, IDC_COMBO8, m_teamA_ccb);
	DDX_Control(pDX, IDC_COMBO9, m_aittmod_ccb);
	DDX_Control(pDX, IDC_CHECK4, m_autoturn_check);
	DDX_Control(pDX, IDC_BUTTON5, c_turn);
	//DDX_Control(pDX, IDC_CHECK5, m_autosave_ck);
}


BEGIN_MESSAGE_MAP(CAIT, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CAIT::OnBnClickedButton2)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_SETFOCUS, IDC_AIT_LIST, &CAIT::OnNMSetfocusAitList)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CAIT::OnHdnItemclickAitList)
	ON_NOTIFY(NM_CLICK, IDC_AIT_LIST, &CAIT::OnNMClickAitList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_AIT_LIST, &CAIT::OnLvnKeydownAitList)
	ON_BN_CLICKED(IDC_BUTTON3, &CAIT::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CAIT::OnBnClickedButton1)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON4, &CAIT::OnBnClickedButton4)
	ON_CBN_SELCHANGE(IDC_COMBO9, &CAIT::OnCbnSelchangeCombo9)
	ON_CBN_EDITCHANGE(IDC_COMBO9, &CAIT::OnCbnEditchangeCombo9)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CAIT::OnCbnSelchangeCombo2)
	ON_CBN_KILLFOCUS(IDC_COMBO2, &CAIT::OnCbnKillfocusCombo2)
	ON_CBN_DBLCLK(IDC_COMBO2, &CAIT::OnCbnDblclkCombo2)
	ON_CBN_EDITCHANGE(IDC_COMBO2, &CAIT::OnCbnEditchangeCombo2)
	ON_CBN_SETFOCUS(IDC_COMBO2, &CAIT::OnCbnSetfocusCombo2)
	ON_CBN_DROPDOWN(IDC_COMBO2, &CAIT::OnCbnDropdownCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO8, &CAIT::OnCbnSelchangeCombo8)
	ON_BN_CLICKED(IDC_BUTTON5, &CAIT::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_CHECK4, &CAIT::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CAIT ��Ϣ�������


void CAIT::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

bool AIT_List_init = true;
void CAIT::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	bool flag = false;
	if (FilePath_main != "None"&& FilePath_main != "Nones")flag = true;
	// �����������
	if (AIT_List_init&&flag){

		m_AIT_list.SetExtendedStyle(m_AIT_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		CString head[] = { TEXT("ע������"), TEXT("ע��") };
		for (int i = 0; i < sizeof(head) / sizeof(head[0]); i++)
			m_AIT_list.InsertColumn(i, head[i], LVCFMT_LEFT, 100);

		AIT_List_init = false;
		CString title, note; // ע�����ƣ���ע
		std::istringstream num_stream(main_aitt);
		std::string line, reg_title, reg_note;
		int team_idx = 0;
		while (std::getline(num_stream, line)) {
			// ����ע���ź�ע�����
			size_t pos = line.find('=');
			if (pos != std::string::npos) {
				reg_title = line.substr(0, pos);
				int posend = line.find(',', pos + 1); // ��pos+1λ�ÿ�ʼ������һ������
				reg_note = line.substr(pos + 1, posend - (pos + 1));
			}
			else {
				continue;
			}

			// �����б�
			title.Format(_T("%s"), CString(reg_title.c_str()));
			note.Format(_T("%s"), CString(reg_note.c_str()));
			m_AIT_list.InsertItem(team_idx, title);
			m_AIT_list.SetItemText(team_idx, 1, note);
			++team_idx;
		}

		m_AIT_list.RedrawWindow();
		AITWind.Invalidate();
		AITWind.UpdateWindow();
	}
}


void CAIT::OnNMSetfocusAitList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}

// �ò���
void CAIT::OnHdnItemclickAitList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	*pResult = 0;
}

// ����ѯ���ݲ���
void CAIT::OnNMClickAitList(NMHDR *pNMHDR, LRESULT *pResult)
{

	if (flag_add_ait){
		count_ait_num += 1;
		m_AIT_list.InsertItem(count_ait_num, add_ait_title);
		m_AIT_list.SetItemText(count_ait_num, 1, (CString)"NoName");
		flag_add_ait = false;
		fresh_ait_list = true; // ˢ���б�
	}

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������


	POSITION pos = m_AIT_list.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		char* posf = NULL;
		int nIndex = m_AIT_list.GetNextSelectedItem(pos);
		// ��ȡ��������
		CString title;
		std::string content;
		title = m_AIT_list.GetItemText(nIndex, 0);
		content = FindMemSTDS(main_aitt, CStringToChar(title));
		m_tip_txt.SetWindowTextW(title);

		// ����ע��
		CString name;
		posf = ExtractText(content.data(), "=",",");
		name = (CString)posf;
		m_name_edit.SetWindowTextW(name);
		free(posf);

		// ������սС��A
		CString teamA;
		posf = SerchTargetString(content.data(), ",", 1, 2);
		teamA = (CString)posf;
		m_teamA_ccb.SetWindowTextW(teamA);
		free(posf);

		// ����ָ������
		CString country;
		posf = SerchTargetString(content.data(), ",", 2, 3);
		country = (CString)posf;
		m_Country_ccb.SetWindowTextW(country);
		free(posf);

		// ������������
		CString tjtype;
		posf = SerchTargetString(content.data(), ",", 4, 5);
		tjtype = (CString)posf;
		switch (numtoi(tjtype)){
		case -1:m_atttype_ccb.SetWindowTextW((CString)"������"); break;
		case 0:m_atttype_ccb.SetWindowTextW((CString)"�з�ӵ��(ǰ��)ָ����λ"); break;
		case 1:m_atttype_ccb.SetWindowTextW((CString)"�ҷ�ӵ��(ǰ��)ָ����λ"); break;
		case 2:m_atttype_ccb.SetWindowTextW((CString)"�з�����Ϊ��"); break;
		case 3:m_atttype_ccb.SetWindowTextW((CString)"�з�����Ϊ��"); break;
		case 4:m_atttype_ccb.SetWindowTextW((CString)"�з���Ǯ���ڻ����"); break;
		case 5:m_atttype_ccb.SetWindowTextW((CString)"������Ļװ�þ���"); break;
		case 6:m_atttype_ccb.SetWindowTextW((CString)"������ʱ�մ��;���"); break;
		case 7:m_atttype_ccb.SetWindowTextW((CString)"������ӵ��(ǰ��)ָ����λ"); break;
		}
		free(posf);

		// ��������ģʽ
		CString aittmod;
		std::string tjmod, tjval,test;
		test = SerchTargetString(content.data(), ",", 6, 7);
		if (strlen(test.data()) != 64){
			CString temp;
			temp.Format(_T("%s,%s"), title, (CString)"��ע����д������Զ�����");
			m_tip_txt.SetWindowTextW(temp);
			test = "0000000000000000000000000000000000000000000000000000000000000000";
		}
		tjmod = FindHexData((char*)test.data(), false); // �����������ģʽ
		tjval = FindHexData((char*)test.data(), true); // �������������ֵ
		
		char* temp = "Ruffy";
		switch (atoi(tjmod.data())){
		case 0: temp = "С��"; break;
		case 1: temp = "С�ڵ���"; break;
		case 2: temp = "����"; break;
		case 3: temp = "���ڵ���"; break;
		case 4: temp = "����"; break;
		case 5: temp = "������"; break;
		}
		m_aittmod_ccb.SetWindowTextW((CString)temp);
		m_value_edit.SetWindowTextW((CString)tjval.data());
		test.clear();
		
		// ����������λ
		CString tjunit;
		posf = SerchTargetString(content.data(), ",", 5, 6);
		m_aittunit_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// ����Ȩ�� 7 8
		posf = inta(SerchTargetString(content.data(), ",", 7, 8));
		m_normalaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// ���Ȩ�� 8 9
		posf = inta(SerchTargetString(content.data(), ",", 8, 9));
		m_lowaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// ���Ȩ�� 9 10
		posf = inta(SerchTargetString(content.data(), ",", 9, 10));
		m_heightaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// �����Ӫʹ��
		posf = SerchTargetString(content.data(), ",", 12, 13);
		m_Unity_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// ���Ƽ��ȼ�
		posf = SerchTargetString(content.data(), ",", 4, 5);
		m_Tech_edit.SetWindowTextW((CString)posf);
		free(posf);

		// ���ȼ�
		posf = SerchTargetString(content.data(), ",", 15, 16);
		if ((CString)posf == (CString)"1"){
			m_EasyEnemy_check.SetCheck(BST_CHECKED);
		}
		else m_EasyEnemy_check.SetCheck(BST_UNCHECKED);
		free(posf);
		posf = SerchTargetString(content.data(), ",", 16, 17);
		if ((CString)posf == (CString)"1"){
			m_NormalEnemy_check.SetCheck(BST_CHECKED);
		}
		else m_NormalEnemy_check.SetCheck(BST_UNCHECKED);
		free(posf);
		posf = SerchTargetString(content.data(), ",", 17, 0);
		if ((CString)posf == (CString)"1"){
			m_HardEnemy_check.SetCheck(BST_CHECKED);
		}else m_HardEnemy_check.SetCheck(BST_UNCHECKED);

		// �Զ������б�
		m_teamA_ccb.GetWindowTextW(flag_fresh_te_title);
		if (m_autoturn_check.GetCheck() == BST_CHECKED)
			flag_fresh_te = true;
		if (flag_fresh_te){
			ArmyWinD.ShowWindow(SW_HIDE);
			ArmyWinD.ShowWindow(SW_NORMAL);
		}
		free(posf);

	}
	*pResult = 0;
}

// ���̲����¼�
void CAIT::OnLvnKeydownAitList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_AIT_list.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		char* posf = NULL;
		int nIndex = m_AIT_list.GetNextSelectedItem(pos);
		// ��ȡ��������
		CString title;
		std::string content;
		title = m_AIT_list.GetItemText(nIndex, 0);
		content = FindMemSTDS(main_aitt, CStringToChar(title));
		m_tip_txt.SetWindowTextW(title);

		// ����ע��
		CString name;
		posf = ExtractText(content.data(), "=", ",");
		name = (CString)posf;
		m_name_edit.SetWindowTextW(name);
		free(posf);

		// ������սС��A
		CString teamA;
		posf = SerchTargetString(content.data(), ",", 1, 2);
		teamA = (CString)posf;
		m_teamA_ccb.SetWindowTextW(teamA);
		free(posf);

		// ����ָ������
		CString country;
		posf = SerchTargetString(content.data(), ",", 2, 3);
		country = (CString)posf;
		m_Country_ccb.SetWindowTextW(country);
		free(posf);

		// ������������
		CString tjtype;
		posf = SerchTargetString(content.data(), ",", 4, 5);
		tjtype = (CString)posf;
		switch (numtoi(tjtype)){
		case -1:m_atttype_ccb.SetWindowTextW((CString)"������"); break;
		case 0:m_atttype_ccb.SetWindowTextW((CString)"�з�ӵ��(ǰ��)ָ����λ"); break;
		case 1:m_atttype_ccb.SetWindowTextW((CString)"�ҷ�ӵ��(ǰ��)ָ����λ"); break;
		case 2:m_atttype_ccb.SetWindowTextW((CString)"�з�����Ϊ��"); break;
		case 3:m_atttype_ccb.SetWindowTextW((CString)"�з�����Ϊ��"); break;
		case 4:m_atttype_ccb.SetWindowTextW((CString)"�з���Ǯ���ڻ����"); break;
		case 5:m_atttype_ccb.SetWindowTextW((CString)"������Ļװ�þ���"); break;
		case 6:m_atttype_ccb.SetWindowTextW((CString)"������ʱ�մ��;���"); break;
		case 7:m_atttype_ccb.SetWindowTextW((CString)"������ӵ��(ǰ��)ָ����λ"); break;
		}
		free(posf);

		// ��������ģʽ
		CString aittmod;
		std::string tjmod, tjval, test;
		test = SerchTargetString(content.data(), ",", 6, 7);
		if (strlen(test.data()) != 64){
			CString temp;
			temp.Format(_T("%s,%s"), title, (CString)"��ע����д������Զ�����");
			m_tip_txt.SetWindowTextW(temp);
			test = "0000000000000000000000000000000000000000000000000000000000000000";
		}
		tjmod = FindHexData((char*)test.data(), false); // �����������ģʽ
		tjval = FindHexData((char*)test.data(), true); // �������������ֵ

		char* temp = "Ruffy";
		switch (atoi(tjmod.data())){
		case 0: temp = "С��"; break;
		case 1: temp = "С�ڵ���"; break;
		case 2: temp = "����"; break;
		case 3: temp = "���ڵ���"; break;
		case 4: temp = "����"; break;
		case 5: temp = "������"; break;
		}
		m_aittmod_ccb.SetWindowTextW((CString)temp);
		m_value_edit.SetWindowTextW((CString)tjval.data());
		test.clear();

		// ����������λ
		CString tjunit;
		posf = SerchTargetString(content.data(), ",", 5, 6);
		m_aittunit_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// ����Ȩ�� 7 8
		posf = inta(SerchTargetString(content.data(), ",", 7, 8));
		m_normalaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// ���Ȩ�� 8 9
		posf = inta(SerchTargetString(content.data(), ",", 8, 9));
		m_lowaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// ���Ȩ�� 9 10
		posf = inta(SerchTargetString(content.data(), ",", 9, 10));
		m_heightaitt_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// �����Ӫʹ��
		posf = SerchTargetString(content.data(), ",", 12, 13);
		m_Unity_ccb.SetWindowTextW((CString)posf);
		free(posf);

		// ���Ƽ��ȼ�
		posf = SerchTargetString(content.data(), ",", 4, 5);
		m_Tech_edit.SetWindowTextW((CString)posf);
		free(posf);

		// ���ȼ�
		posf = SerchTargetString(content.data(), ",", 15, 16);
		if ((CString)posf == (CString)"1"){
			m_EasyEnemy_check.SetCheck(BST_CHECKED);
		}
		else m_EasyEnemy_check.SetCheck(BST_UNCHECKED);
		free(posf);
		posf = SerchTargetString(content.data(), ",", 16, 17);
		if ((CString)posf == (CString)"1"){
			m_NormalEnemy_check.SetCheck(BST_CHECKED);
		}
		else m_NormalEnemy_check.SetCheck(BST_UNCHECKED);
		free(posf);
		posf = SerchTargetString(content.data(), ",", 17, 0);
		if ((CString)posf == (CString)"1"){
			m_HardEnemy_check.SetCheck(BST_CHECKED);
		}
		else m_HardEnemy_check.SetCheck(BST_UNCHECKED);
		free(posf);

	}
	*pResult = 0;
}

// ���浱ǰ�޸�
void CAIT::OnBnClickedButton3()
{
	int EasyEnemy, NormalEnemy, HardEnemy;
	CString title, aitt_txt,name,country,attmod,tech,teamA,atttype,aittunit,value,unity,lowaitt,normalaitt,heightaitt;
	int nIndex = m_AIT_list.GetNextItem(-1, LVNI_SELECTED);
	if (nIndex == -1)return;
	// ��ȡ����
	title = m_AIT_list.GetItemText(nIndex, 0);
	// ��ȡע��
	m_name_edit.GetWindowTextW(name);
	// ��ȡ�򵥵��˿���
	EasyEnemy = m_EasyEnemy_check.GetCheck();
	// ��ȡ�еȵ��˿���
	NormalEnemy = m_NormalEnemy_check.GetCheck();
	// ��ȡ���ѵ��˿���
	HardEnemy = m_HardEnemy_check.GetCheck();
	// ��ȡ����״��
	m_Country_ccb.GetWindowTextW(country);
	// ��ȡ�Ƽ�״̬
	m_Tech_edit.GetWindowTextW(tech);
	// ��ȡ����ģʽ
	m_aittmod_ccb.GetWindowTextW(attmod);
	// ��ȡ��������
	m_atttype_ccb.GetWindowTextW(atttype);
	// ��ȡ������λ
	m_aittunit_ccb.GetWindowTextW(aittunit);
	// ��ȡ������ֵ
	m_value_edit.GetWindowTextW(value);
	// ��ȡָ����Ӫ
	m_Unity_ccb.GetWindowTextW(unity);
	// ��ȡ���Ȩ��
	m_lowaitt_ccb.GetWindowTextW(lowaitt);
	// ��ȡ����Ȩ��
	m_normalaitt_ccb.GetWindowTextW(normalaitt);
	// ��ȡ���Ȩ��
	m_heightaitt_ccb.GetWindowTextW(heightaitt);
	// ��ȡ����A
	m_teamA_ccb.GetWindowTextW(teamA);

	// ��ȡ����ǰ�沿��
	std::string head;
	int headpos;
	headpos = TxtMemFind(main_aitt.size(),0,main_aitt.data(),CStringToChar(title),R_BF);
	head = main_aitt.substr(0,headpos);
	
	// ��ȡ��������ĩβ������
	std::string content_end;
	content_end += CStringToChar(title);
	if (!Sac(FindMemSTDS(main_aitt, CStringToChar(title)), "None")){
		content_end += FindMemSTDS(main_aitt, CStringToChar(title));
	}
	else content_end = "";
	std::string end;
	int endpos;
	endpos = content_end.size() + headpos;
	end = main_aitt.substr(endpos);

	// ��ȡ�޸Ĳ���
	std::string replace_txt;
	replace_txt += CStringToChar(title);
	replace_txt += "=";
	replace_txt += CStringToChar(name);
	replace_txt += ",";
	replace_txt += CStringToChar(teamA);
	replace_txt += ",";
	replace_txt += CStringToChar(country);
	replace_txt += ",";
	replace_txt += CStringToChar(tech);
	replace_txt += ",";
	// ��ϣֵѰ��
	char temp_value[20];
	sprintf_s(temp_value, sizeof(temp_value), "%d", getmode(CStringToChar(atttype)));
	replace_txt += temp_value;
	replace_txt += ",";

	// ��ӵ�λ
	replace_txt += CStringToChar(aittunit);
	replace_txt += ",";

	// ��Ӵ�����ֵ
	replace_txt += decimalToHexadecimal(CStringToChar(value));
	// ��������
	replace_txt += "0";

	char temp_tj[20];
	sprintf_s(temp_tj, sizeof(temp_tj), "%d", Ruffyfind(CStringToChar(attmod)));

	replace_txt += temp_tj;
	replace_txt += "000000000000000000000000000000000000000000000000000000,";
	// ���������ߴ���
	replace_txt += CStringToChar(normalaitt);
	replace_txt += ".000000,";
	replace_txt += CStringToChar(lowaitt);
	replace_txt += ".000000,";
	replace_txt += CStringToChar(heightaitt);
	replace_txt += ".000000,1,0,";
	replace_txt += CStringToChar(unity);
	replace_txt += ",0,";
	replace_txt += "<none>,";

	// �е� �� �����Ƿ����
	if (EasyEnemy == BST_CHECKED)
		replace_txt += "1,";
	else replace_txt += "0,";
	if (NormalEnemy == BST_CHECKED)
		replace_txt += "1,";
	else replace_txt += "0,";
	if (HardEnemy == BST_CHECKED)
		replace_txt += "1";
	else replace_txt += "0";
	head += replace_txt;
	head += GetMemLines(end.data(), end.size());
	head += "\n";
	main_aitt.clear();
	main_aitt = head;
	head.clear();
	fresh_ait_list = true;

	std::string upnote;
	CString o_tit;
	upnote = CStringToChar(name);
	int count = m_AIT_list.GetItemCount();
	for (int i = 0; i < count; i++){
		o_tit = m_AIT_list.GetItemText(i, 0);
		if (o_tit == title){
			m_AIT_list.SetItemText(i, 1, (CString)upnote.data());
		}
	}

	return;
}

// ��������
void CAIT::OnBnClickedButton1()
{
	add_type = 0;
	Add_new_init = true;
	add_wind.ShowWindow(SW_HIDE);
	add_wind.ShowWindow(SW_NORMAL);
}


void CAIT::OnMouseMove(UINT nFlags, CPoint point)
{

	if (flag_add_ait){
		count_ait_num += 1;
		m_AIT_list.InsertItem(count_ait_num, add_ait_title);
		m_AIT_list.SetItemText(count_ait_num,1, (CString)"NoName");
		flag_add_ait = false;
		fresh_ait_list = true; // ˢ���б�
	}
	
	if (fresh_ait_list){
		fresh_ait_list = false;
		CString title, name,namet;
		std::string content, nameStr;
		int nCount = m_AIT_list.GetItemCount();
		for (int i = 0; i < nCount; i++) {
			title = m_AIT_list.GetItemText(i, 0); // ��ȡ����
			namet = m_AIT_list.GetItemText(i, 1); // ��ȡ�ڶ��е�name
			content = FindMemSTDS(main_aitt, CStringToChar(title));
			// ��������ע��
			char* posf = ExtractText(content.data(), "=", ",");
			name = (CString)posf;
			free(posf);
			if (name!=namet){
				m_AIT_list.SetItemText(i, 1,name);
			}
		}
	}

	// ��ȡ����ģʽ������
	CString tjmode;
	m_atttype_ccb.GetWindowTextW(tjmode);
	m_aittmod_ccb.EnableWindow(TRUE);
	m_aittunit_ccb.EnableWindow(TRUE);
	m_value_edit.EnableWindow(TRUE);
	// ��ȡ����ģʽ������
	m_atttype_ccb.GetWindowTextW(tjmode);
	if (tjmode == (CString)"�з���Ǯ���ڻ����"){
		m_aittmod_ccb.SetWindowTextW((CString)"���ڻ����");
		if (m_aittmod_ccb.IsWindowEnabled())
			m_aittmod_ccb.EnableWindow(FALSE);
		m_aittunit_ccb.SetWindowTextW((CString)"<none>");
		if (m_aittunit_ccb.IsWindowEnabled())
			m_aittunit_ccb.EnableWindow(FALSE);
	}
	else if ((tjmode == (CString)"������Ļװ�þ���") || (tjmode == (CString)"������ʱ�մ��;���") || (tjmode == (CString)"�з�����Ϊ��") || (tjmode == (CString)"�з�����Ϊ��") || (tjmode == (CString)"������")){
		m_aittmod_ccb.SetWindowTextW((CString)"С��");
		m_aittunit_ccb.SetWindowTextW((CString)"<none>");
		if (m_aittmod_ccb.IsWindowEnabled())
			m_aittmod_ccb.EnableWindow(FALSE);
		if (m_aittunit_ccb.IsWindowEnabled())
			m_aittunit_ccb.EnableWindow(FALSE);
		m_value_edit.SetWindowTextW((CString)"0");
		if (m_value_edit.IsWindowEnabled())
			m_value_edit.EnableWindow(FALSE);
	}

	if (flag_updata_te){	// ���½ű�������
		std::string str_temp;
		int linenum = 0,nIndex;
		m_teamA_ccb.ResetContent();
		char* pStr = GetMemLines(main_team_num.data(), main_team_num.size());
		linenum = GetMemLinesNum(pStr);
		for (int i = 0; i < linenum; i++)
		{
			str_temp = GetMemLine(main_team_num.data(), i);
			CString temp_all;
			temp_all = (CString)FindMemSTDS(str_temp, "=").data();
			if (temp_all != (CString)"None")
			m_teamA_ccb.AddString(temp_all);
		}
		nIndex = m_AIT_list.GetNextItem(-1, LVNI_SELECTED);
		if (nIndex != -1){
			CString title, team_s;
			m_tip_txt.GetWindowTextW(title);
			m_teamA_ccb.SetWindowTextW(title);
		}
		flag_updata_te = false;
	}


	CDialogEx::OnMouseMove(nFlags, point);
}

// ɾ��
void CAIT::OnBnClickedButton4()
{
	int nIndex = m_AIT_list.GetNextItem(-1, LVNI_SELECTED); // ��ȡ��ǰ�����е�����
	while (nIndex != -1){
		nIndex = m_AIT_list.GetNextItem(-1, LVNI_SELECTED); // ��ȡ��ǰ�����е�����
		if (nIndex != -1) {
			CString del_title;
			count_ait_num -= 1;
			del_title = m_AIT_list.GetItemText(nIndex, 0);
			m_AIT_list.DeleteItem(nIndex);
			DelMemLine(main_aitt, CStringToChar(del_title));
			fresh_ait_list = true;
		}
	}
	main_aitt = GetMemLines(main_aitt.data(), main_aitt.size());
}

// ��Ч�¼�
void CAIT::OnCbnSelchangeCombo9()
{
	
}


void CAIT::OnCbnEditchangeCombo9()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

// ʹ�ü����޸�����ʱ����
void CAIT::OnCbnSelchangeCombo2()
{
}

// ʧȥ����ʱ����
void CAIT::OnCbnKillfocusCombo2()
{
}

// ������������ ����
void CAIT::OnCbnDblclkCombo2()
{
}

// ������������¼�
void CAIT::OnCbnEditchangeCombo2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}

// ���ô���
void CAIT::OnCbnSetfocusCombo2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CAIT::OnCbnDropdownCombo2()
{
	// ��ȡ����ģʽ������
}


void CAIT::OnCbnSelchangeCombo8()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

// �����תʱ
void CAIT::OnBnClickedButton5()
{
	flag_fresh_te = true;
	ArmyWinD.ShowWindow(SW_HIDE);
	ArmyWinD.ShowWindow(SW_NORMAL);
}


// ��ת��ť�Ƿ�����ж�
void CAIT::OnBnClickedCheck4()
{
	if (m_autoturn_check.GetCheck() == BST_CHECKED){
		c_turn.EnableWindow(FALSE);
	}
	else c_turn.EnableWindow(TRUE);
}
