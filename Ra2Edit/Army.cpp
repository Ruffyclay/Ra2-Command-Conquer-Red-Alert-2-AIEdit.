// Army.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Ra2Edit.h"
#include "Army.h"
#include <afxwin.h>
#include "afxdialogex.h"
#include <afxcmn.h>
#include "OtherTools.h"
#include "AIT.h"
#include "TaskForces.h"
#include "CScripts.h"

using namespace std;
bool flag_add_team = false;
// ȫ�ּĴ�main_team_num���������, ��һ�����������, ʼ���������
int main_team_num_index = 0;
CString flag_fresh_sc_title, flag_fresh_ta_title;
size_t team_head_pos = 0, team_end_pos = 0;
int num_pos = -1;
std::string Priority = "None";
std::string Max = "None";
std::string name_content = "None";
std::string TaskForce_content = "None";
std::string Script_content = "None";
std::string team_title = "None";




// ��ǰѡ��������
std::string content_head = "None";

void ReadTeam(){
	int poss, pose, len, len_temp_txt, psN = 0;
	char* temp, *temp_txt, *Line_num_txt = NULL;
	CString num, title, txt, note;
	while (psPos[psN].pos != NULL) {
		poss = TxtMemFind(psPos[psN].size, 0, psPos[psN].pos, "[TeamTypes]", C_END);
		if (poss == -1) {
			psN += 1; continue;
		}
		pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "[", C_HEAD);
		temp = ReadMemContent(psPos[psN].pos, psPos[psN].size, poss, pose);
		len = strlen(temp);
		temp_txt = GetMemLines(temp, len); // ����ؽ�����Դ�ļ��޹�
		len_temp_txt = GetMemLinesNum(temp);
		//main_team_num_index = len_temp_txt; // ��ȡȫ������������
		// ��ʱ�ṹ��,std::vectorģ��ר��
		struct TeamTypes temp_t;
		for (int i = 0; i < len_temp_txt; i++) {
			Line_num_txt = GetMemLine(temp_txt, i);
			if (CheckRegedit(Line_num_txt)) {
				num = GetTargetString(Line_num_txt, strlen(Line_num_txt), "=", R_BF);
				if (flag_auto_check)start_regnum = numtoi(num);
				flag_auto_check = false;
				title = GetTargetString(Line_num_txt, strlen(Line_num_txt), "=", R_BH);
				/*main_team_num_index += 1;
				max_index += 1;*/
				// ��¼��������ƣ�����Ѱ�Ҷ���Name=���Ե�ֵ
				note.Format(_T("[%s]"), title);
				poss = TxtMemFind(psPos[psN].size, 0, psPos[psN].pos, CStringToChar(note), C_END);
				int poss_1 = poss;
				if (poss != -1) { // ������ݴ���
					poss = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "Name", C_END);
					pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "\n", C_HEAD);
					note = (CString)ReadMemContent(psPos[psN].pos, psPos[psN].size, poss, pose);
					// ��������
					poss = poss_1;
					pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "[", C_HEAD);
					if (pose == -1) {
						pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "\0", C_HEAD);
					}
					txt = (CString)ReadMemContent(psPos[psN].pos, psPos[psN].size, poss, pose);
				}

				// �������е�����,��̬����
				temp_t.num = atoi(CStringToChar(num));
				temp_t.title = CStringToChar(title);
				temp_t.note = CStringToChar(note);
				temp_t.txt = CStringToChar(txt);
				teamtypes.push_back(temp_t);
			}
		}
		psN += 1;
	}
	//max_on_index = max_index;
	TeamSortByNum(teamtypes);
	main_team_num.reserve(strlen("[TeamTypes]\n"));
	main_team_num.insert(main_team_num.end(), "[TeamTypes]\n", "[TeamTypes]\n" + std::strlen("[TeamTypes]\n"));
	int lenCount = 0;
	for (int i = 0; i < teamtypes.size(); i++) {
		std::stringstream ss;
		ss << i+start_regnum << "=" << teamtypes[i].title;
		std::string temp_str = ss.str();
		lenCount += temp_str.length();
		main_team_num.reserve(lenCount + 1); // Ϊ���з�Ԥ��һ���ֽ�
		main_team_num.insert(main_team_num.end(), temp_str.c_str(), temp_str.c_str() + temp_str.length());
		main_team_num.push_back('\n');
		teamtypes[i].num = -1;
	}
	CString temp_C;
	char* temp_c;
	main_team.push_back('\n');
	for (int i = 0; i < teamtypes.size(); i++) {
		temp_C.Format(_T("[%s]"), (CString)teamtypes[i].title);
		temp_c = CStringToChar(temp_C);
		lenCount += strlen(temp_c);
		main_team.reserve(lenCount + 1);
		main_team.insert(main_team.end(), temp_c, temp_c + strlen(temp_c));
		main_team.push_back('\n');

		temp_c = teamtypes[i].txt;
		temp_c = GetMemLines(temp_c, strlen(temp_c));
		lenCount += strlen(temp_c);
		main_team.reserve(lenCount + 1);
		main_team.insert(main_team.end(), temp_c, temp_c + strlen(temp_c));
		main_team.push_back('\n');
		teamtypes[i].title = NULL;
		teamtypes[i].note = NULL;
		teamtypes[i].txt = NULL;
		flag_updata_te = true;
	}
}

// CArmy �Ի���
CArmy ArmyWinD;

IMPLEMENT_DYNAMIC(CArmy, CDialogEx)

CArmy::CArmy(CWnd* pParent /*=NULL*/)
	: CDialogEx(CArmy::IDD, pParent)
{
	pRa2EditDlg = dynamic_cast<CRa2EditDlg*>(pParent);
}

CArmy::~CArmy()
{
}

void CArmy::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_ARMY_LIST, m_Army_list);
	DDX_Control(pDX, IDC_CHECK5, m_Aggressive_check);
	DDX_Control(pDX, IDC_CHECK10, m_Suicide_check);
	DDX_Control(pDX, IDC_EDIT4, m_Priority_edit);
	DDX_Control(pDX, IDC_EDIT5, m_Max_edit);
	DDX_Control(pDX, IDC_EDIT1, m_Name_edit);
	DDX_Control(pDX, IDC_CHECK11, m_Prebuild_check);
	DDX_Control(pDX, IDC_CHECK8, m_AreTeamMembersRecruitable_check);
	DDX_Control(pDX, IDC_CHECK23, m_Autocreate_check);
	DDX_Control(pDX, IDC_CHECK6, m_AvoidThreats_check);
	DDX_Control(pDX, IDC_CHECK12, m_LooseRecruit_check);
	DDX_Control(pDX, IDC_CHECK4, m_Whiner_check);
	DDX_Control(pDX, IDC_CHECK15, m_Droppod_check);
	DDX_Control(pDX, IDC_CHECK19, m_Annoyance_check);
	DDX_Control(pDX, IDC_CHECK7, m_UseTransportOrigin_check);
	DDX_Control(pDX, IDC_CHECK3, m_Reinforce_check);
	DDX_Control(pDX, IDC_CHECK9, m_IonImmune_check);
	DDX_Control(pDX, IDC_CHECK18, m_OnlyTargetHouseEnemy_check);
	DDX_Control(pDX, IDC_CHECK13, m_Full_check);
	DDX_Control(pDX, IDC_CHECK2, m_GuardSlower_check);
	DDX_Control(pDX, IDC_CHECK1, m_Loadable_check);
	DDX_Control(pDX, IDC_CHECK16, m_TransportsReturnOnUnload_check);
	DDX_Control(pDX, IDC_CHECK17, m_IsBaseDefense_check);
	DDX_Control(pDX, IDC_COMBO1, m_task_ccb);
	DDX_Control(pDX, IDC_COMBO2, m_script_ccb);
	//DDX_Control(pDX, IDC_EDIT2, m_task_rd);
	//DDX_Control(pDX, IDC_EDIT3, m_script_rd);
	DDX_Control(pDX, IDC_CHECK14, m_Recruiter_check);
	DDX_Control(pDX, IDC_TIP, Ctip_txt);
	DDX_Control(pDX, IDC_CHECK20, m_autoturn_check);
	DDX_Control(pDX, IDC_BUTTON4, m_turn_bt);
	//DDX_Control(pDX, IDC_CHECK21, m_autosave_ck);
}


BEGIN_MESSAGE_MAP(CArmy, CDialogEx)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CArmy::OnBnClickedButton1)
	ON_NOTIFY(LVN_COLUMNCLICK, IDD_ARMY_LIST, &CArmy::OnLvnColumnclickArmyList)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_SETFOCUS, IDD_ARMY_LIST, &CArmy::OnNMSetfocusArmyList)
	ON_NOTIFY(NM_KILLFOCUS, IDD_ARMY_LIST, &CArmy::OnNMKillfocusArmyList)
	ON_NOTIFY(NM_CLICK, IDD_ARMY_LIST, &CArmy::OnNMClickArmyList)
	ON_NOTIFY(LVN_KEYDOWN, IDD_ARMY_LIST, &CArmy::OnLvnKeydownArmyList)
	ON_BN_CLICKED(IDC_BUTTON2, &CArmy::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CArmy::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CArmy::OnBnClickedButton5)
	//ON_BN_CLICKED(IDC_BUTTON4, &CArmy::OnBnClickedButton4)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON4, &CArmy::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK20, &CArmy::OnBnClickedCheck20)
END_MESSAGE_MAP()


// CArmy ��Ϣ�������
bool Army_List_init = true;
bool Once_ArmyInit = true;
bool Clist_Updata_init = true;

// ��¼�
void CArmy::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO:  �ڴ˴������Ϣ����������
	if (Once_ArmyInit){

		m_Army_list.SetExtendedStyle(m_Army_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		//�������
		CString head[] = { TEXT("ע�����"), TEXT("ע������"), TEXT("ע��") };
		
		//������
		for (int i = 0; i < sizeof(head) / sizeof(head[0]);i++)
		m_Army_list.InsertColumn(i, head[i], LVCFMT_LEFT, 100);


		Once_ArmyInit = false;
	}
	return;
}


// �ر�Army����
void CArmy::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ArmyWinD.OnCancel();
}


void CArmy::OnLvnColumnclickArmyList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

// Edit�س��¼�
void CArmy::OnOK()
{
	// TODO:  �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}

// ��ʾ���ڵ�ʱ��ѭ����������
void CArmy::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	
	bool flag = false;
	if (FilePath_main != "None"&& FilePath_main != "Nones")flag = true;
	// �����������
	if (FilePath_main != "None"&& FilePath_main != "Nones" && Clist_Updata_init){
		std::string str_temp;
		int linenum = GetMemLinesNum(main_script_num.data());
		for (int i = 1; i < linenum+1; i++)
		{
			str_temp = GetMemLine(main_script_num.data(), i);
			m_script_ccb.AddString((CString)FindMemSTDS(str_temp, "=").data());
		}
		linenum = GetMemLinesNum(main_task_num.data());
		for (int i = 1; i < linenum+1; i++)
		{
			str_temp = GetMemLine(main_task_num.data(), i);
			m_task_ccb.AddString((CString)FindMemSTDS(str_temp, "=").data());
		}
		Clist_Updata_init = false;
	}
	if (FilePath_main != "None"&& FilePath_main != "Nones")flag = true;
	if (Army_List_init&&flag){
		main_team_num_index = 0;
		m_Army_list.DeleteAllItems();
		CString num, title, note; // ע�����
		std::istringstream num_stream(main_team_num);
		std::string line, reg_num, reg_title;
		int team_idx = 0;
		while (std::getline(num_stream, line)) {
			// ����ע���ź�ע�����
			main_team_num_index += 1; // ע��������
			size_t pos = line.find('=');
			if (pos != std::string::npos) {
				reg_num = line.substr(0, pos);
				reg_title = line.substr(pos + 1);
			}
			else {
				continue;
			}

			// ����ע������Ӧ������
			pos = main_team.find("[" + reg_title + "]");
			if (pos == std::string::npos) {
				continue;
			}
			pos = main_team.find("Name=", pos);
			if (pos == std::string::npos) {
				continue;
			}
			pos += 5;
			size_t end_pos = main_team.find('\n', pos);
			if (end_pos == std::string::npos) {
				end_pos = main_team.size();
			}

			// �����б�
			num.Format(_T("%s"), CString(reg_num.c_str()));
			title.Format(_T("%s"), CString(reg_title.c_str()));
			note.Format(_T("%s"), CString(main_team.substr(pos, end_pos - pos).c_str()));
			m_Army_list.InsertItem(team_idx, num);
			m_Army_list.SetItemText(team_idx, 1, title);
			m_Army_list.SetItemText(team_idx, 2, note);
			++team_idx;
		}
		m_Army_list.RedrawWindow();
		ArmyWinD.Invalidate();
		ArmyWinD.UpdateWindow();
		Army_List_init = false;
	}
	if (flag_fresh_te){
		int count = m_Army_list.GetItemCount();
		CString temp;
		for (int i = 0; i < count; i++){
			temp = m_Army_list.GetItemText(i,1);
			if (temp == flag_fresh_te_title){
				m_Army_list.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
				CString title = m_Army_list.GetItemText(i, 1);
				CString num = m_Army_list.GetItemText(i, 0);
				CString note = m_Army_list.GetItemText(i, 2);
				size_t pos = main_team.find("[" + std::string(CT2A(title)) + "]");
				if (pos != std::string::npos) {
					size_t start_pos = main_team.rfind("\n", pos) + 1;
					size_t end_pos = main_team.find("[", start_pos + 1) - 1;
					size_t lentcontent = end_pos - start_pos;
					content_head = main_team.substr(start_pos, lentcontent);
					// ��¼�����������
					team_head_pos = start_pos + strlen(CStringToChar(title)) + 3; // ȫ�ּĴ浱ǰteam�޸ĵ�λ��
					team_end_pos = end_pos;
					// ��ʼ���Ҹ�����־λ
					name_content = FindMemSTDS(content_head, "Name=");
					Priority = FindMemSTDS(content_head, "Priority=");
					Max = FindMemSTDS(content_head, "Max=");
					TaskForce_content = FindMemSTDS(content_head, "TaskForce=");
					Script_content = FindMemSTDS(content_head, "Script=");
					team_title = CStringToChar(title);

					m_Name_edit.SetWindowTextW((CString)name_content.data());
					m_Priority_edit.SetWindowTextW((CString)Priority.data());
					m_Max_edit.SetWindowTextW((CString)Max.data());

					m_script_ccb.SetWindowTextW((CString)Script_content.data());
					m_task_ccb.SetWindowTextW((CString)TaskForce_content.data());

					CString temp;
					temp.Format(_T("ע����ţ�%s,���⣺%s"), num, title);
					num_pos = atoi(CStringToChar(num)); // ��ȡ��ǰѡ������

					Ctip_txt.SetWindowTextW(temp);


					if (Sac(FindMemSTDS(content_head, "Full="), "yes")){
						m_Full_check.SetCheck(BST_CHECKED);
					}
					else{
						m_Full_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "Annoyance="), "yes")){
						m_Annoyance_check.SetCheck(BST_CHECKED);
					}
					else{
						m_Annoyance_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "GuardSlower="), "yes")){
						m_GuardSlower_check.SetCheck(BST_CHECKED);
					}
					else{
						m_GuardSlower_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "Recruiter="), "yes")){
						m_Recruiter_check.SetCheck(BST_CHECKED);
					}
					else{
						m_Recruiter_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "Autocreate="), "yes")){
						m_Autocreate_check.SetCheck(BST_CHECKED);
					}
					else{
						m_Autocreate_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "Prebuild="), "yes")){
						m_Prebuild_check.SetCheck(BST_CHECKED);
					}
					else{
						m_Prebuild_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "Reinforce="), "yes")){
						m_Reinforce_check.SetCheck(BST_CHECKED);
					}
					else{
						m_Reinforce_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "Droppod="), "yes")){
						m_Droppod_check.SetCheck(BST_CHECKED);
					}
					else{
						m_Droppod_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "UseTransportOrigin="), "yes")){
						m_UseTransportOrigin_check.SetCheck(BST_CHECKED);
					}
					else{
						m_UseTransportOrigin_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "Whiner="), "yes")){
						m_Whiner_check.SetCheck(BST_CHECKED);
					}
					else{
						m_Whiner_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "LooseRecruit="), "yes")){
						m_LooseRecruit_check.SetCheck(BST_CHECKED);
					}
					else{
						m_LooseRecruit_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "Aggressive="), "yes")){
						m_Aggressive_check.SetCheck(BST_CHECKED);
					}
					else{
						m_Aggressive_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "Suicide="), "yes")){
						m_Suicide_check.SetCheck(BST_CHECKED);
					}
					else{
						m_Suicide_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "AvoidThreats="), "yes")){
						m_AvoidThreats_check.SetCheck(BST_CHECKED);
					}
					else{
						m_AvoidThreats_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "IonImmune="), "yes")){
						m_IonImmune_check.SetCheck(BST_CHECKED);
					}
					else{
						m_IonImmune_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "TransportsReturnOnUnload="), "yes")){
						m_TransportsReturnOnUnload_check.SetCheck(BST_CHECKED);
					}
					else{
						m_TransportsReturnOnUnload_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "AreTeamMembersRecruitable="), "yes")){
						m_AreTeamMembersRecruitable_check.SetCheck(BST_CHECKED);
					}
					else{
						m_AreTeamMembersRecruitable_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "IsBaseDefense="), "yes")){
						m_IsBaseDefense_check.SetCheck(BST_CHECKED);
					}
					else{
						m_IsBaseDefense_check.SetCheck(BST_UNCHECKED);
					}
					if (Sac(FindMemSTDS(content_head, "OnlyTargetHouseEnemy="), "yes")){
						m_OnlyTargetHouseEnemy_check.SetCheck(BST_CHECKED);
					}
					else{
						m_OnlyTargetHouseEnemy_check.SetCheck(BST_UNCHECKED);
					}

					flag_fresh_sc_title = (CString)Script_content.data();
					flag_fresh_ta_title = (CString)TaskForce_content.data();
					if (m_autoturn_check.GetCheck() == BST_CHECKED){
						flag_fresh_sc = true;
						flag_fresh_ta = true;
					}
					if (flag_fresh_ta){
						TaskForceWind.ShowWindow(SW_HIDE);
						TaskForceWind.ShowWindow(SW_NORMAL);
						ScriptsWind.ShowWindow(SW_HIDE);
						ScriptsWind.ShowWindow(SW_NORMAL);
					}

				}
				else{
					Ctip_txt.SetWindowTextW((CString)"�˶����ѱ�ɾ��");
				}
			}
			else{
				m_Army_list.SetItemState(i, 0, LVIS_FOCUSED | LVIS_SELECTED);
			}
		}
		flag_fresh_te = false;
	}

	return;
}

int m_nPrevSelItem = -1;
// �ı��б��۽�ʱ�Ĵ����
void CArmy::OnNMSetfocusArmyList(NMHDR* pNMHDR, LRESULT* pResult)
{
	
	*pResult = 0;
}

// ʧȥ�����¼�
void CArmy::OnNMKillfocusArmyList(NMHDR *pNMHDR, LRESULT *pResult)
{

	*pResult = 0;
}

// �����б��¼� �����޸�
void CArmy::OnNMClickArmyList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nPrevSelItem = m_Army_list.GetSelectionMark();

	if (flag_add_team) {
		// ��ȡע����ܳ���
		int count = GetMemLinesNum(main_team_num.data());
		std::string new_num = "[TeamTypes]\n", new_content;
		int posff = -1;
		for (int i = 0; i < count; i++) {
			// ��ȡi�е�����
			std::string temp_num = GetMemLine(main_team_num.data(), i);
			// ��i���ҳ�����
			std::string temp_title = FindMemSTDS(temp_num.data(), "=");
			if (Sac(temp_title, "None"))continue;
			posff += 1;
			// ��main_team���ҳ�ע�����

			std::string temp = FindiItem(main_team, temp_title.data());
			if (Sac(temp, "None"))temp_title.clear();

			// ��ע��
			std::string temp_name = FindMemSTDS(temp, "Name=");

			// �����б�
			CString num;
			num.Format(_T("%d"), posff + start_regnum);
			int rowCount = m_Army_list.GetItemCount();
			if (rowCount > posff) {
				m_Army_list.SetItemText(posff, 0, num);
			}
			else {
				m_Army_list.InsertItem(posff, num);

			}
			m_Army_list.SetItemText(posff, 1, CString(temp_title.c_str()));
			m_Army_list.SetItemText(posff, 2, CString(temp_name.c_str()));

			// �����µ�ע������
			new_num += CStringToChar(num);
			new_num += "=";
			new_num += GetMemLines(temp_title.data(), temp_title.size());
			new_num += "\n";

			// �����µ�ע�������
			new_content += GetMemLines(temp.data(), temp.size());
			new_content += "\n";
		}

		new_num += "\n";
		// ����main_team_num��main_team
		main_team_num = new_num;
		main_team = new_content;

		flag_add_team = false;
	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString title = m_Army_list.GetItemText(m_Army_list.GetSelectionMark(), 1);
	CString num = m_Army_list.GetItemText(m_Army_list.GetSelectionMark(), 0);
	CString note = m_Army_list.GetItemText(m_Army_list.GetSelectionMark(), 2);

	// ����ע������Ӧ������
	size_t pos = main_team.find("[" + std::string(CT2A(title)) + "]");
	if (pos != std::string::npos) {
		size_t start_pos = main_team.rfind("\n", pos) + 1;
		size_t end_pos = main_team.find("[", start_pos + 1) - 1;
		size_t lentcontent = end_pos - start_pos;
		content_head = main_team.substr(start_pos, lentcontent);
		// ��¼�����������
		team_head_pos = start_pos + strlen(CStringToChar(title)) + 3; // ȫ�ּĴ浱ǰteam�޸ĵ�λ��
		team_end_pos = end_pos;
		// ��ʼ���Ҹ�����־λ
		name_content = FindMemSTDS(content_head, "Name=");
		Priority = FindMemSTDS(content_head, "Priority=");
		Max = FindMemSTDS(content_head, "Max=");
		TaskForce_content = FindMemSTDS(content_head, "TaskForce=");
		Script_content = FindMemSTDS(content_head, "Script=");
		team_title = CStringToChar(title);

		m_Name_edit.SetWindowTextW((CString)name_content.data());
		m_Priority_edit.SetWindowTextW((CString)Priority.data());
		m_Max_edit.SetWindowTextW((CString)Max.data());

		m_script_ccb.SetWindowTextW((CString)Script_content.data());
		m_task_ccb.SetWindowTextW((CString)TaskForce_content.data());

		CString temp;
		temp.Format(_T("ע����ţ�%s,���⣺%s"), num, title);
		num_pos = atoi(CStringToChar(num)); // ��ȡ��ǰѡ������

		Ctip_txt.SetWindowTextW(temp);


		if (Sac(FindMemSTDS(content_head, "Full="), "yes")){
			m_Full_check.SetCheck(BST_CHECKED);
		}
		else{
			m_Full_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "Annoyance="), "yes")){
			m_Annoyance_check.SetCheck(BST_CHECKED);
		}
		else{
			m_Annoyance_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "GuardSlower="), "yes")){
			m_GuardSlower_check.SetCheck(BST_CHECKED);
		}
		else{
			m_GuardSlower_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "Recruiter="), "yes")){
			m_Recruiter_check.SetCheck(BST_CHECKED);
		}
		else{
			m_Recruiter_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "Autocreate="), "yes")){
			m_Autocreate_check.SetCheck(BST_CHECKED);
		}
		else{
			m_Autocreate_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "Prebuild="), "yes")){
			m_Prebuild_check.SetCheck(BST_CHECKED);
		}
		else{
			m_Prebuild_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "Reinforce="), "yes")){
			m_Reinforce_check.SetCheck(BST_CHECKED);
		}
		else{
			m_Reinforce_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "Droppod="), "yes")){
			m_Droppod_check.SetCheck(BST_CHECKED);
		}
		else{
			m_Droppod_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "UseTransportOrigin="), "yes")){
			m_UseTransportOrigin_check.SetCheck(BST_CHECKED);
		}
		else{
			m_UseTransportOrigin_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "Whiner="), "yes")){
			m_Whiner_check.SetCheck(BST_CHECKED);
		}
		else{
			m_Whiner_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "LooseRecruit="), "yes")){
			m_LooseRecruit_check.SetCheck(BST_CHECKED);
		}
		else{
			m_LooseRecruit_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "Aggressive="), "yes")){
			m_Aggressive_check.SetCheck(BST_CHECKED);
		}
		else{
			m_Aggressive_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "Suicide="), "yes")){
			m_Suicide_check.SetCheck(BST_CHECKED);
		}
		else{
			m_Suicide_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "AvoidThreats="), "yes")){
			m_AvoidThreats_check.SetCheck(BST_CHECKED);
		}
		else{
			m_AvoidThreats_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "IonImmune="), "yes")){
			m_IonImmune_check.SetCheck(BST_CHECKED);
		}
		else{
			m_IonImmune_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "TransportsReturnOnUnload="), "yes")){
			m_TransportsReturnOnUnload_check.SetCheck(BST_CHECKED);
		}
		else{
			m_TransportsReturnOnUnload_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "AreTeamMembersRecruitable="), "yes")){
			m_AreTeamMembersRecruitable_check.SetCheck(BST_CHECKED);
		}
		else{
			m_AreTeamMembersRecruitable_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "IsBaseDefense="), "yes")){
			m_IsBaseDefense_check.SetCheck(BST_CHECKED);
		}
		else{
			m_IsBaseDefense_check.SetCheck(BST_UNCHECKED);
		}
		if (Sac(FindMemSTDS(content_head, "OnlyTargetHouseEnemy="), "yes")){
			m_OnlyTargetHouseEnemy_check.SetCheck(BST_CHECKED);
		}
		else{
			m_OnlyTargetHouseEnemy_check.SetCheck(BST_UNCHECKED);
		}

		// �����������ڵ�ˢ��
		flag_fresh_sc_title = (CString)Script_content.data();
		flag_fresh_ta_title = (CString)TaskForce_content.data();
		if (m_autoturn_check.GetCheck() == BST_CHECKED){
			flag_fresh_sc = true;
			flag_fresh_ta = true;
			TaskForceWind.ShowWindow(SW_HIDE);
			TaskForceWind.ShowWindow(SW_NORMAL);
			ScriptsWind.ShowWindow(SW_HIDE);
			ScriptsWind.ShowWindow(SW_NORMAL);
		}
		//m_script_rd.SetWindowTextW((CString)FindiItem(main_script, Script_content.data()).data());
		//m_task_rd.SetWindowTextW((CString)FindiItem(main_task, TaskForce_content.data()).data());
	}
	else{
		Ctip_txt.SetWindowTextW((CString)"�˶����ѱ�ɾ��");
	}
	*pResult = 0;
}

// �����¼�
void CArmy::OnLvnKeydownArmyList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pKey = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	// �������¼�
	if (pKey->wVKey == VK_UP || pKey->wVKey == VK_DOWN) {
		// ��ȡ��ǰѡ�е��б���
		int nCurSel = m_Army_list.GetNextItem(-1, LVNI_SELECTED);
		

		// ������һ��ѡ�е��б���
		if (pKey->wVKey == VK_UP && nCurSel > 0) {
			nCurSel--;
		}
		else if (pKey->wVKey == VK_DOWN && nCurSel < m_Army_list.GetItemCount() - 1) {
			nCurSel++;
		}

		// ȡ��֮ǰ��ѡ��״̬
		m_Army_list.SetItemState(-1, 0, LVIS_SELECTED);

		// �����µ�ѡ��״̬
		m_Army_list.SetItemState(nCurSel, LVIS_SELECTED, LVIS_SELECTED);
		m_Army_list.EnsureVisible(nCurSel, FALSE);

		CString title = m_Army_list.GetItemText(m_Army_list.GetSelectionMark(), 1);
		CString num = m_Army_list.GetItemText(m_Army_list.GetSelectionMark(), 0);
		CString note = m_Army_list.GetItemText(m_Army_list.GetSelectionMark(), 2);

		// ����ע������Ӧ������
		size_t pos = main_team.find("[" + std::string(CT2A(title)) + "]");
		if (pos != std::string::npos) {
			size_t start_pos = main_team.rfind("\n", pos) + 1;
			size_t end_pos = main_team.find("[", start_pos + 1) - 1;
			size_t lentcontent = end_pos - start_pos;
			content_head = main_team.substr(start_pos, lentcontent);
			// ��¼�����������
			team_head_pos = start_pos + strlen(CStringToChar(title)) + 3; // ȫ�ּĴ浱ǰteam�޸ĵ�λ��
			team_end_pos = end_pos;
			// ��ʼ���Ҹ�����־λ
			name_content = FindMemSTDS(content_head, "Name=");
			Priority = FindMemSTDS(content_head, "Priority=");
			Max = FindMemSTDS(content_head, "Max=");
			TaskForce_content = FindMemSTDS(content_head, "TaskForce=");
			Script_content = FindMemSTDS(content_head, "Script=");
			team_title = CStringToChar(title);

			m_Name_edit.SetWindowTextW((CString)name_content.data());
			m_Priority_edit.SetWindowTextW((CString)Priority.data());
			m_Max_edit.SetWindowTextW((CString)Max.data());

			m_script_ccb.SetWindowTextW((CString)Script_content.data());
			m_task_ccb.SetWindowTextW((CString)TaskForce_content.data());

			CString temp;
			temp.Format(_T("ע����ţ�%s,���⣺%s"), num, title);
			num_pos = atoi(CStringToChar(num)); // ��ȡ��ǰѡ������

			Ctip_txt.SetWindowTextW(temp);


			if (Sac(FindMemSTDS(content_head, "Full="), "yes")){
				m_Full_check.SetCheck(BST_CHECKED);
			}
			else{
				m_Full_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "Annoyance="), "yes")){
				m_Annoyance_check.SetCheck(BST_CHECKED);
			}
			else{
				m_Annoyance_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "GuardSlower="), "yes")){
				m_GuardSlower_check.SetCheck(BST_CHECKED);
			}
			else{
				m_GuardSlower_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "Recruiter="), "yes")){
				m_Recruiter_check.SetCheck(BST_CHECKED);
			}
			else{
				m_Recruiter_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "Autocreate="), "yes")){
				m_Autocreate_check.SetCheck(BST_CHECKED);
			}
			else{
				m_Autocreate_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "Prebuild="), "yes")){
				m_Prebuild_check.SetCheck(BST_CHECKED);
			}
			else{
				m_Prebuild_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "Reinforce="), "yes")){
				m_Reinforce_check.SetCheck(BST_CHECKED);
			}
			else{
				m_Reinforce_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "Droppod="), "yes")){
				m_Droppod_check.SetCheck(BST_CHECKED);
			}
			else{
				m_Droppod_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "UseTransportOrigin="), "yes")){
				m_UseTransportOrigin_check.SetCheck(BST_CHECKED);
			}
			else{
				m_UseTransportOrigin_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "Whiner="), "yes")){
				m_Whiner_check.SetCheck(BST_CHECKED);
			}
			else{
				m_Whiner_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "LooseRecruit="), "yes")){
				m_LooseRecruit_check.SetCheck(BST_CHECKED);
			}
			else{
				m_LooseRecruit_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "Aggressive="), "yes")){
				m_Aggressive_check.SetCheck(BST_CHECKED);
			}
			else{
				m_Aggressive_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "Suicide="), "yes")){
				m_Suicide_check.SetCheck(BST_CHECKED);
			}
			else{
				m_Suicide_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "AvoidThreats="), "yes")){
				m_AvoidThreats_check.SetCheck(BST_CHECKED);
			}
			else{
				m_AvoidThreats_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "IonImmune="), "yes")){
				m_IonImmune_check.SetCheck(BST_CHECKED);
			}
			else{
				m_IonImmune_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "TransportsReturnOnUnload="), "yes")){
				m_TransportsReturnOnUnload_check.SetCheck(BST_CHECKED);
			}
			else{
				m_TransportsReturnOnUnload_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "AreTeamMembersRecruitable="), "yes")){
				m_AreTeamMembersRecruitable_check.SetCheck(BST_CHECKED);
			}
			else{
				m_AreTeamMembersRecruitable_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "IsBaseDefense="), "yes")){
				m_IsBaseDefense_check.SetCheck(BST_CHECKED);
			}
			else{
				m_IsBaseDefense_check.SetCheck(BST_UNCHECKED);
			}
			if (Sac(FindMemSTDS(content_head, "OnlyTargetHouseEnemy="), "yes")){
				m_OnlyTargetHouseEnemy_check.SetCheck(BST_CHECKED);
			}
			else{
				m_OnlyTargetHouseEnemy_check.SetCheck(BST_UNCHECKED);
			}

			//m_script_rd.SetWindowTextW((CString)FindiItem(main_script, Script_content.data()).data());
			//m_task_rd.SetWindowTextW((CString)FindiItem(main_task, TaskForce_content.data()).data());
		}
		else{
			Ctip_txt.SetWindowTextW((CString)"�˶����ѱ�ɾ��");
		}

	}

	*pResult = 0;
}

// ��������
void CArmy::OnBnClickedButton2()
{
	CString temp_str;
	std::string team_replace_content = "";
	m_Name_edit.GetWindowTextW(temp_str);
	team_replace_content += "Name=";
	if (temp_str == "\0"){
		temp_str = "<None>";
	}
	team_replace_content += CStringToChar(temp_str);
	team_replace_content += '\n';

	m_Priority_edit.GetWindowTextW(temp_str);
	team_replace_content += "Priority=";
	team_replace_content += CStringToChar(temp_str);
	if (temp_str == "\0"){
		temp_str = "1";
	}
	team_replace_content += '\n';

	m_Max_edit.GetWindowTextW(temp_str);
	team_replace_content += "Max=";
	if (temp_str == "\0"){
		temp_str = "1";
	}
	team_replace_content += CStringToChar(temp_str);
	team_replace_content += '\n';

	team_replace_content += "VeteranLevel=1\nMindControlDecision=2\nHouse=<none>\nLoadable=no\nTechLevel=0\nGroup=-1\nOnTransOnly=no\n";


	if (m_Full_check.GetCheck()){
		team_replace_content += "Full=yes\n";
	}
	else{
		team_replace_content += "Full=no\n";
	}

	if (m_Annoyance_check.GetCheck()){
		team_replace_content += "Annoyance=yes\n";
	}
	else{
		team_replace_content += "Annoyance=no\n";
	}
	if (m_GuardSlower_check.GetCheck()){
		team_replace_content += "GuardSlower=yes\n";
	}
	else{
		team_replace_content += "GuardSlower=no\n";
	}
	if (m_Recruiter_check.GetCheck()){
		team_replace_content += "Recruiter=yes\n";
	}
	else{
		team_replace_content += "Recruiter=no\n";
	}

	if (m_Autocreate_check.GetCheck()){
		team_replace_content += "Autocreate=yes\n";
	}
	else{
		team_replace_content += "Autocreate=no\n";
	}

	if (m_Prebuild_check.GetCheck()){
		team_replace_content += "Prebuild=yes\n";
	}
	else{
		team_replace_content += "Prebuild=no\n";
	}

	if (m_Reinforce_check.GetCheck()){
		team_replace_content += "Reinforce=yes\n";
	}
	else{
		team_replace_content += "Reinforce=no\n";
	}

	if (m_Droppod_check.GetCheck()){
		team_replace_content += "Droppod=yes\n";
	}
	else{
		team_replace_content += "Droppod=no\n";
	}

	if (m_UseTransportOrigin_check.GetCheck()){
		team_replace_content += "UseTransportOrigin=yes\n";
	}
	else{
		team_replace_content += "UseTransportOrigin=no\n";
	}

	if (m_Whiner_check.GetCheck()){
		team_replace_content += "Whiner=yes\n";
	}
	else{
		team_replace_content += "Whiner=no\n";
	}

	if (m_LooseRecruit_check.GetCheck()){
		team_replace_content += "LooseRecruit=yes\n";
	}
	else{
		team_replace_content += "LooseRecruit=no\n";
	}

	if (m_Aggressive_check.GetCheck()){
		team_replace_content += "Aggressive=yes\n";
	}
	else{
		team_replace_content += "Aggressive=no\n";
	}

	if (m_Suicide_check.GetCheck()){
		team_replace_content += "Suicide=yes\n";
	}
	else{
		team_replace_content += "Suicide=no\n";
	}

	if (m_AvoidThreats_check.GetCheck()){
		team_replace_content += "AvoidThreats=yes\n";
	}
	else{
		team_replace_content += "AvoidThreats=no\n";
	}

	if (m_IonImmune_check.GetCheck()){
		team_replace_content += "IonImmune=yes\n";
	}
	else{
		team_replace_content += "IonImmune=no\n";
	}

	if (m_TransportsReturnOnUnload_check.GetCheck()){
		team_replace_content += "TransportsReturnOnUnload=yes\n";
	}
	else{
		team_replace_content += "TransportsReturnOnUnload=no\n";
	}

	if (m_AreTeamMembersRecruitable_check.GetCheck()){
		team_replace_content += "AreTeamMembersRecruitable=yes\n";
	}
	else{
		team_replace_content += "AreTeamMembersRecruitable=no\n";
	}

	if (m_IsBaseDefense_check.GetCheck()){
		team_replace_content += "IsBaseDefense=yes\n";
	}
	else{
		team_replace_content += "IsBaseDefense=no\n";
	}
	if (m_OnlyTargetHouseEnemy_check.GetCheck()){
		team_replace_content += "OnlyTargetHouseEnemy=yes\n";
	}
	else{
		team_replace_content += "OnlyTargetHouseEnemy=no\n";
	}
	int index = m_script_ccb.GetCurSel();
	CString temp;
	m_script_ccb.GetWindowText(temp);
	team_replace_content += "Script=";
	team_replace_content += CStringToChar(temp);
	team_replace_content += "\n";

	m_task_ccb.GetWindowText(temp);
	team_replace_content += "TaskForce=";
	team_replace_content += CStringToChar(temp);
	team_replace_content += "\n";

	// ��ǰ���ݵ�ǰ��
	std::string replace_adsf(main_team, 0, team_head_pos);
	// ���������
	std::string replace_afsd = "";
	if (team_end_pos <= main_team.size())
		replace_afsd = main_team.substr(team_end_pos);

	// �޸Ĺ��������
	replace_adsf += team_replace_content;
	replace_adsf += replace_afsd;

	main_team.clear();
	main_team = replace_adsf;
	replace_adsf.clear();
	// �����б�����

	Army_List_init = true;
	if (Army_List_init){
		Army_List_init = false;
		CString  note; // ����
		std::istringstream num_stream(main_team_num);
		std::string line;
		int team_idx = 0;
		size_t pos = 0;
		pos = main_team.find("[" + team_title + "]");
		while (std::getline(num_stream, line)) {
			char* as = CStringToChar(m_Army_list.GetItemText(team_idx, 1));
			// �������
			if (Sac(team_title, as)){
				pos = main_team.find("Name=", pos);
				if (pos == std::string::npos) {
					continue;
				}
				pos += 5;
				size_t end_pos = main_team.find('\n', pos);
				if (end_pos == std::string::npos) {
					end_pos = main_team.size();
				}
				note.Format(_T("%s"), CString(main_team.substr(pos, end_pos - pos).c_str()));
				m_Army_list.SetItemText(team_idx, 2, note);
			}
			++team_idx;
		}
		m_Army_list.RedrawWindow();
	}
}

// ������ť
void CArmy::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	add_type = 1;
	Add_new_init = true;
	main_team_num_index += 1;
	add_wind.ShowWindow(SW_HIDE);
	add_wind.ShowWindow(SW_NORMAL);
}

// ɾ��ָ��������
void CArmy::OnBnClickedButton5()
{
	// ��ȡ��ǰѡ���е�����
	int nIndex = m_Army_list.GetNextItem(-1, LVNI_SELECTED);
	while (nIndex != -1){
		CString num, title, Content;
		title.Format(_T("%s=%s"), m_Army_list.GetItemText(nIndex, 0), m_Army_list.GetItemText(nIndex, 1));
		DelMemLine(main_team_num, CStringToChar(title)); // ɾ��������һ��
		main_team_num = GetMemLines(main_team_num.data(), main_team_num.size());
		main_team_num += "\n";
		m_Army_list.DeleteItem(nIndex);
		nIndex = m_Army_list.GetNextItem(-1, LVNI_SELECTED);
	}

	// �Զ�ˢ���б����˳����
	CString num,content,name,title;
	std::string temp_num = "[TeamTypes]\n", temp,cc;
	bool flag = false;
	if (FilePath_main != "None"&& FilePath_main != "Nones")flag = true;
	if (flag){
		int count_index = m_Army_list.GetItemCount();
		for (int i = 0; i < count_index; i++){
			num.Format(_T("%d"),i+start_regnum);
			title = m_Army_list.GetItemText(i, 1);
			content.Format(_T("%s=%s\n"), num, title); // ��ȡע������
			m_Army_list.SetItemText(i, 0, num); // �������
			temp_num += CStringToChar(content);
			cc = FindiItem(main_team, CStringToChar(m_Army_list.GetItemText(i, 1)));
			name.Format(_T("%s"), FindMemSTDS(cc, CStringToChar(title)));
			m_Army_list.SetItemText(i, 2, name); // ����ע��
			cc += "\n";
			temp += cc;
			cc.clear();
		}
		main_team_num.clear();
		main_team_num = temp_num;
		main_team_num += "\n";
		main_team.clear();
		main_team = temp;
		flag_updata_te = true;
	}
}

// ˢ���б�
//void CArmy::OnBnClickedButton4()
//{
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//
//	bool flag = false;
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	if (FilePath_main != "None"&& FilePath_main != "Nones")flag = true;
//	if (flag){
//		main_team_num_index = 0;
//		m_Army_list.DeleteAllItems();
//		CString num, title, note; // ע�����
//		std::istringstream num_stream(main_team_num);
//		std::string line, reg_num, reg_title;
//		int team_idx = 0;
//		while (std::getline(num_stream, line)) {
//			// ����ע���ź�ע�����
//			size_t pos = line.find('=');
//			if (pos != std::string::npos) {
//				reg_num = line.substr(0, pos);
//				reg_title = line.substr(pos + 1);
//			}
//			else {
//				continue;
//			}
//
//			// ����ע������Ӧ������
//			pos = main_team.find("[" + reg_title + "]");
//			if (pos == std::string::npos) {
//				continue;
//			}
//			pos = main_team.find("Name=", pos);
//			if (pos == std::string::npos) {
//				continue;
//			}
//			pos += 5;
//			size_t end_pos = main_team.find('\n', pos);
//			if (end_pos == std::string::npos) {
//				end_pos = main_team.size();
//			}
//
//			// �����б�
//			num.Format(_T("%d"), main_team_num_index);
//			title.Format(_T("%s"), CString(reg_title.c_str()));
//			note.Format(_T("%s"), CString(main_team.substr(pos, end_pos - pos).c_str()));
//			m_Army_list.InsertItem(team_idx, num);
//			m_Army_list.SetItemText(team_idx, 1, title);
//			m_Army_list.SetItemText(team_idx, 2, note);
//			++team_idx;
//			main_team_num_index += 1; // ע��������
//
//		}
//		m_Army_list.RedrawWindow();
//	}
//}

// ��������������
void CArmy::OnMouseMove(UINT nFlags, CPoint point)
{
	std::string str_temp;
	int linenum = 0,nIndex;

	if (flag_updata_sc){	// ���½ű�������
		m_script_ccb.ResetContent();
		char* pStr = GetMemLines(main_script_num.data(), main_script_num.size());
		linenum = GetMemLinesNum(pStr);
		for (int i = 0; i < linenum; i++)
		{
			str_temp = GetMemLine(main_script_num.data(), i);
			CString temp_all;
			temp_all = (CString)FindMemSTDS(str_temp, "=").data();
			if (temp_all != (CString)"None")
				m_script_ccb.AddString(temp_all);
		}
		nIndex = m_Army_list.GetNextItem(-1, LVNI_SELECTED);
		if (nIndex != -1){
			CString title,scripts_s;
			title = m_Army_list.GetItemText(nIndex, 1);
			scripts_s = (CString)FindiItem(main_team, CStringToChar(title)).data();
			scripts_s = (CString)FindMemSTDS((std::string)CStringToChar(scripts_s), "Script=").data();
			m_script_ccb.SetWindowTextW(scripts_s);
		}
		flag_updata_sc = false;
	}

	if (flag_updata_ta){	// ���¶���������
		m_task_ccb.ResetContent();
		char* pStr = GetMemLines(main_task_num.data(), main_task_num.size());
		linenum = GetMemLinesNum(pStr);
		for (int i = 0; i < linenum; i++)
		{
			str_temp = GetMemLine(main_task_num.data(), i);
			CString temp_all;
			temp_all = (CString)FindMemSTDS(str_temp, "=").data();
			if (temp_all != (CString)"None")
				m_task_ccb.AddString(temp_all);
		}
		nIndex = m_Army_list.GetNextItem(-1, LVNI_SELECTED);
		if (nIndex != -1){
			CString titles, task_s;
			titles = m_Army_list.GetItemText(nIndex, 1);
			task_s = (CString)FindiItem(main_team, CStringToChar(titles)).data();
			task_s = (CString)FindMemSTDS((std::string)CStringToChar(task_s), "TaskForce=").data();
			m_task_ccb.SetWindowTextW(task_s);
		}
		flag_updata_ta = false;
	}
	str_temp.clear();

	if (flag_add_team) {
		// ��ȡע����ܳ���
		int count = GetMemLinesNum(main_team_num.data());
		std::string new_num = "[TeamTypes]\n", new_content;
		int posff = -1;
		for (int i = 0; i < count; i++) {
			// ��ȡi�е�����
			std::string temp_num = GetMemLine(main_team_num.data(), i);
			// ��i���ҳ�����
			std::string temp_title = FindMemSTDS(temp_num.data(), "=");
			if (Sac(temp_title, "None"))continue;
			posff += 1;
			// ��main_team���ҳ�ע�����

			std::string temp = FindiItem(main_team, temp_title.data());
			if (Sac(temp, "None"))temp_title.clear();

			// ��ע��
			std::string temp_name = FindMemSTDS(temp, "Name=");

			// �����б�
			CString num;
			num.Format(_T("%d"), posff + start_regnum);
			int rowCount = m_Army_list.GetItemCount();
			if (rowCount > posff) {
				m_Army_list.SetItemText(posff, 0, num);
			}
			else {
				m_Army_list.InsertItem(posff, num);

			}
			m_Army_list.SetItemText(posff, 1, CString(temp_title.c_str()));
			m_Army_list.SetItemText(posff, 2, CString(temp_name.c_str()));

			// �����µ�ע������
			new_num += CStringToChar(num);
			new_num += "=";
			new_num += GetMemLines(temp_title.data(), temp_title.size());
			new_num += "\n";

			// �����µ�ע�������
			new_content += GetMemLines(temp.data(), temp.size());
			new_content += "\n";
		}

		new_num += "\n";
		// ����main_team_num��main_team
		main_team_num = new_num;
		main_team = new_content;

		flag_add_team = false;
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

// ��ת���ű�������
void CArmy::OnBnClickedButton4()
{
	flag_fresh_sc = true;
	flag_fresh_ta = true;
	TaskForceWind.ShowWindow(SW_HIDE);
	TaskForceWind.ShowWindow(SW_NORMAL);
	ScriptsWind.ShowWindow(SW_HIDE);
	ScriptsWind.ShowWindow(SW_NORMAL);
}


void CArmy::OnBnClickedCheck20()
{
	if (m_autoturn_check.GetCheck() == BST_CHECKED){
		m_turn_bt.EnableWindow(FALSE);
	}
	else m_turn_bt.EnableWindow(TRUE);
}
