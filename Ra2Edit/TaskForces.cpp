// TaskForces.cpp : 实现文件
//

#include "stdafx.h"
#include "Ra2Edit.h"
#include "TaskForces.h"
#include "afxdialogex.h"
#include "OtherTools.h"
#include "Army.h"

CString add_title_ta;
int Count_ta = 0;
bool flag_add_task = false;

void ReadTask(){
	int poss, pose, len, len_temp_txt, psN = 0;
	char* temp, *temp_txt, *Line_num_txt = NULL;
	CString num, title, content, note,txt;
	while (psPos[psN].pos != NULL){
		poss = TxtMemFind(psPos[psN].size, 0, psPos[psN].pos, "[TaskForces]", C_END);
		if (poss == -1){ psN += 1; continue; }
		pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "[", C_HEAD);
		temp = ReadMemContent(psPos[psN].pos, psPos[psN].size, poss, pose);
		len = strlen(temp);
		temp_txt = GetMemLines(temp, len); // 与加载进来的源文件无关
		len_temp_txt = GetMemLinesNum(temp);

		// 临时结构体,std::vector模块专用
		struct TaskForces temp_t;
		for (int i = 0; i < len_temp_txt; i++)
		{
			Line_num_txt = GetMemLine(temp_txt, i);
			if (CheckRegedit(Line_num_txt)){
				num = GetTargetString(Line_num_txt, strlen(Line_num_txt), "=", R_BF);
				title = GetTargetString(Line_num_txt, strlen(Line_num_txt), "=", R_BH);

				// 记录队伍的名称，尝试寻找队伍Name=属性的值
				note.Format(_T("[%s]"), title);
				poss = TxtMemFind(psPos[psN].size, 0, psPos[psN].pos, CStringToChar(note), C_END);
				int poss_1 = poss;
				if (poss != -1){
					poss = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "Name", C_END);
					pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "\n", C_HEAD);
					note = (CString)ReadMemContent(psPos[psN].pos, psPos[psN].size, poss, pose);

					// 查找内容
					poss = poss_1;
					pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "[", C_HEAD);
					if (pose == -1) {
						pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "\0", C_HEAD);
					}
					txt = (CString)ReadMemContent(psPos[psN].pos, psPos[psN].size, poss, pose);

				}

				// 保存所有的内容,动态变量
				temp_t.num = atoi(CStringToChar(num));
				temp_t.title = CStringToChar(title);
				temp_t.note = CStringToChar(note);
				temp_t.txt = CStringToChar(txt);
				taskforces.push_back(temp_t);
			}
		}
		psN += 1;
	}
	TaskSortByNum(taskforces);
	main_task_num.reserve(strlen("[TaskForces]\n"));
	main_task_num.insert(main_task_num.end(), "[TaskForces]\n", "[TaskForces]\n" + std::strlen("[TaskForces]\n"));
	int lenCount = 0;
	for (int i = 0; i < taskforces.size(); i++) {
		std::stringstream ss;
		ss << i+start_regnum << "=" << taskforces[i].title;
		std::string temp_str = ss.str();
		lenCount += temp_str.length();
		main_task_num.reserve(lenCount + 1); // 为换行符预留一个字节
		main_task_num.insert(main_task_num.end(), temp_str.c_str(), temp_str.c_str() + temp_str.length());
		main_task_num.push_back('\n');
		taskforces[i].num = -1;
	}
	CString temp_C;
	char* temp_c;
	main_task.push_back('\n');
	for (int i = 0; i < taskforces.size(); i++) {
		temp_C.Format(_T("[%s]"), (CString)taskforces[i].title);
		temp_c = CStringToChar(temp_C);
		lenCount += strlen(temp_c);
		main_task.reserve(lenCount + 1);
		main_task.insert(main_task.end(), temp_c, temp_c + strlen(temp_c));
		main_task.push_back('\n');

		temp_c = taskforces[i].txt;
		temp_c = GetMemLines(temp_c, strlen(temp_c));
		lenCount += strlen(temp_c);
		main_task.reserve(lenCount + 1);
		main_task.insert(main_task.end(), temp_c, temp_c + strlen(temp_c));
		main_task.push_back('\n');
		taskforces[i].title = NULL;
		taskforces[i].txt = NULL;
		taskforces[i].note = NULL;
	}
}

// CTaskForces 对话框
CTaskForces TaskForceWind;

IMPLEMENT_DYNAMIC(CTaskForces, CDialogEx)

CTaskForces::CTaskForces(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTaskForces::IDD, pParent)
{
}

CTaskForces::~CTaskForces()
{
}

void CTaskForces::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_TASK_LIST, m_TaskForces_list);
	DDX_Control(pDX, IDC_RICHEDIT21, m_task_redit);
	DDX_Control(pDX, IDC_TIP, m_tip_txt);
	//DDX_Control(pDX, IDC_CHECK1, m_autosave_ck);
}


BEGIN_MESSAGE_MAP(CTaskForces, CDialogEx)
	/*ON_BN_CLICKED(IDC_BUTTON1, &CTaskForces::OnBnClickedButton1)*/
	ON_BN_CLICKED(IDC_BUTTON2, &CTaskForces::OnBnClickedButton2)
	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, &CTaskForces::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &CTaskForces::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &CTaskForces::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CTaskForces::OnBnClickedButton8)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(NM_CLICK, IDD_TASK_LIST, &CTaskForces::OnNMClickTaskList)
	ON_NOTIFY(LVN_KEYDOWN, IDD_TASK_LIST, &CTaskForces::OnLvnKeydownTaskList)
END_MESSAGE_MAP()


// CTaskForces 消息处理程序




void CTaskForces::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	OnCancel();
}

bool Once_TaskForcesInit = true;
void CTaskForces::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	if (Once_TaskForcesInit){
		m_TaskForces_list.SetExtendedStyle(m_TaskForces_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		//插入标题
		CString head[] = { TEXT("注册序号"), TEXT("注册名称"), TEXT("注释") };

		//插入列
		for (int i = 0; i < sizeof(head) / sizeof(head[0]); i++)
			m_TaskForces_list.InsertColumn(i, head[i], LVCFMT_LEFT, 100);


		Once_TaskForcesInit = false;
	}

	CFont font;
	font.CreatePointFont(120, _T("微软雅黑"));
	m_task_redit.SetFont(&font);
	

	return;
}

bool Task_List_init = true;
void CTaskForces::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	bool flag = false;
	if (FilePath_main != "None"&& FilePath_main != "Nones")flag = true;
	// 解决载入问题
	if (Task_List_init&&flag){
		Task_List_init = false;
		CString num, title, note; // 注册序号
		std::istringstream num_stream(main_task_num);
		std::string line, reg_num, reg_title;
		int team_idx = 0;
		while (std::getline(num_stream, line)) {
			// 分离注册编号和注册标题
			size_t pos = line.find('=');
			if (pos != std::string::npos) {
				reg_num = line.substr(0, pos);
				reg_title = line.substr(pos + 1);
			}
			else {
				continue;
			}

			// 查找注册标题对应的内容
			pos = main_task.find("[" + reg_title + "]");
			if (pos == std::string::npos) {
				continue;
			}
			pos = main_task.find("Name=", pos);
			if (pos == std::string::npos) {
				continue;
			}
			pos += 5;
			size_t end_pos = main_task.find('\n', pos);
			if (end_pos == std::string::npos) {
				end_pos = main_task.size();
			}

			// 插入列表
			num.Format(_T("%s"), CString(reg_num.c_str()));
			title.Format(_T("%s"), CString(reg_title.c_str()));
			note.Format(_T("%s"), CString(main_task.substr(pos, end_pos - pos).c_str()));
			m_TaskForces_list.InsertItem(team_idx, num);
			m_TaskForces_list.SetItemText(team_idx, 1, title);
			m_TaskForces_list.SetItemText(team_idx, 2, note);
			++team_idx;
		}

		m_TaskForces_list.RedrawWindow();
		TaskForceWind.Invalidate();
		TaskForceWind.UpdateWindow();
	}

	if (flag_fresh_ta){
		int count = m_TaskForces_list.GetItemCount();
		CString temp;
		for (int i = 0; i < count; i++){
			temp = m_TaskForces_list.GetItemText(i, 1);
			if (temp == flag_fresh_ta_title){ // 如果找到了
				m_TaskForces_list.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
				m_task_redit.SetWindowTextW((CString)FindiItem(main_task, CStringToChar(flag_fresh_ta_title)).data());
				m_tip_txt.SetWindowTextW(flag_fresh_ta_title);
			}
			else m_TaskForces_list.SetItemState(i, 0, LVIS_FOCUSED | LVIS_SELECTED);
		}
		flag_fresh_ta = false;
	}
}

// 新增
void CTaskForces::OnBnClickedButton1()
{
	Count_ta = m_TaskForces_list.GetItemCount();
	Add_new_init = true;
	add_type = 3;
	add_wind.ShowWindow(SW_HIDE);
	add_wind.ShowWindow(SW_NORMAL);
}

// 理顺部队
void CTaskForces::OnBnClickedButton7()
{
	CString content, temp_c, txt, title;
	m_tip_txt.GetWindowTextW(title);
	txt.Format(_T("[%s]\n"), title);
	std::string stdt, line;
	m_task_redit.GetWindowTextW(content);
	stdt = CStringToChar(content);
	txt += (CString)"Name=";
	txt += FindMemSTDS(stdt, "Name=").data(); // 获取注释
	txt += (CString)"Group=";
	txt += FindMemSTDS(stdt, "Group=").data(); // 获取分组
	content = (CString)GetMemLines(CStringToChar(content), strlen(CStringToChar(content)));
	int count = GetMemLinesNum(CStringToChar(content));
	int nn = 0;
	for (int i = 1; i < count + 1; i++){
		line = GetMemLine(CStringToChar(content), i);
		bool yes = line.find(",") != std::string::npos && line.find("Name") == std::string::npos;
		if (yes){
			if (line.find("=") != std::string::npos) // 如果有=则找最后一个
				line = SerchTargetString(line.data(), "=", -2, -2);
			temp_c.Format(_T("%d=%s\n"), nn, (CString)line.data());
			txt += temp_c;
			nn++;
		}
	}
	txt = (CString)GetMemLines(CStringToChar(txt), strlen(CStringToChar(txt)));
	m_task_redit.SetWindowTextW(txt);
	return;
}

// 保存
void CTaskForces::OnBnClickedButton6()
{
	// 查找内容
	std::string temp;
	CString title, lenC, ll;
	title = (CString)"[";
	m_tip_txt.GetWindowTextW(ll);
	title += ll;
	title += (CString)"]";
	char* lenc;
	m_task_redit.GetWindowTextW(lenC);
	lenc = CStringToChar((CString)FindiItem(main_task, CStringToChar(ll)).data());
	int posEnd = TxtMemFind(main_task.size(), 0, main_task.data(), CStringToChar(title), R_BF) + strlen(lenc);
	int posStart = TxtMemFind(main_task.size(), 0, main_task.data(), CStringToChar(title), R_BF);

	// 开头 修改位置 结尾
	temp += main_task.substr(0, posStart);
	temp += removeExtraNewlines(GetMemLines(CStringToChar(lenC), strlen(CStringToChar(lenC))));
	temp += main_task.substr(posEnd);

	main_task.clear();
	main_task = temp;
	temp.clear();
	free(lenc);

	// 更新列表最新的注释
	CString tit, o_tit;
	std::string upnote;
	m_tip_txt.GetWindowTextW(tit);
	upnote = FindiItem(main_task, CStringToChar(tit));
	upnote = FindMemSTDS(upnote, "Name=");
	int count = m_TaskForces_list.GetItemCount();
	for (int i = 0; i < count; i++){
		o_tit = m_TaskForces_list.GetItemText(i, 1);
		if (o_tit == tit){
			m_TaskForces_list.SetItemText(i, 2, (CString)upnote.data());
		}
	}
}

// 删除
void CTaskForces::OnBnClickedButton8()
{
	int nIndex = m_TaskForces_list.GetNextItem(-1, LVNI_SELECTED);
	while (nIndex != -1){
		CString title;
		m_TaskForces_list.DeleteItem(nIndex);
		nIndex = m_TaskForces_list.GetNextItem(-1, LVNI_SELECTED);
	}
	int pouch = m_TaskForces_list.GetItemCount();
	CString temp_num = (CString)"[TaskForces]\n", txt, temp;
	for (int i = 0; i < pouch; i++){
		// 累加注册标题到内存
		txt.Format(_T("%d=%s\n"), i+start_regnum, m_TaskForces_list.GetItemText(i, 1));
		temp_num += txt;
	}

	for (int i = 0; i < pouch; i++){
		CString title = m_TaskForces_list.GetItemText(i, 1);
		CString dat = (CString)FindiItem(main_task, CStringToChar(title)).data();
		txt.Format(_T("%s\n"), dat);
		temp += txt;
	}
	for (int i = 0; i < pouch; i++){
		CString num;
		num.Format(_T("%d"), i+start_regnum);
		m_TaskForces_list.SetItemText(i, 0, num);
	}
	main_task_num.clear();
	main_task.clear();
	main_task = CStringToChar(temp);
	main_task += "\n";
	main_task_num = CStringToChar(temp_num);
	flag_updata_ta = true;
	return;
}


void CTaskForces::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (flag_add_task){
			CString num;
			num.Format(_T("%d"), Count_ta+start_regnum);
			m_TaskForces_list.InsertItem(Count_ta, num);
			m_TaskForces_list.SetItemText(Count_ta, 1, add_title_ta);
			m_TaskForces_list.SetItemText(Count_ta, 2, (CString)"Ruffy");
			Count_ta += 1;
			flag_add_task = false;
		}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CTaskForces::OnNMClickTaskList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nIndex = m_TaskForces_list.GetNextItem(-1, LVNI_SELECTED);
	if (nIndex != -1){ // 如果点击非空
		char* Item = CStringToChar(m_TaskForces_list.GetItemText(nIndex, 1));
		m_task_redit.SetWindowTextW((CString)FindiItem(main_task, Item).data());
		m_tip_txt.SetWindowTextW(m_TaskForces_list.GetItemText(nIndex, 1));
	}

	if (flag_add_task){
		CString num;
		num.Format(_T("%d"), Count_ta+start_regnum);
		m_TaskForces_list.InsertItem(Count_ta, num);
		m_TaskForces_list.SetItemText(Count_ta, 1, add_title_ta);
		m_TaskForces_list.SetItemText(Count_ta, 2, (CString)"Ruffy");
		Count_ta += 1;
		flag_add_task = false;
	}
	*pResult = 0;
}


void CTaskForces::OnLvnKeydownTaskList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	int nIndex = m_TaskForces_list.GetNextItem(-1, LVNI_SELECTED);
	if (nIndex != -1){ // 如果点击非空
		char* Item = CStringToChar(m_TaskForces_list.GetItemText(nIndex, 1));
		m_task_redit.SetWindowTextW((CString)FindiItem(main_task, Item).data());
		m_tip_txt.SetWindowTextW(m_TaskForces_list.GetItemText(nIndex, 1));
	}

	if (flag_add_task){
		CString num;
		num.Format(_T("%d"), Count_ta+start_regnum);
		m_TaskForces_list.InsertItem(Count_ta, num);
		m_TaskForces_list.SetItemText(Count_ta, 1, add_title_ta);
		m_TaskForces_list.SetItemText(Count_ta, 2, (CString)"Ruffy");
		Count_ta += 1;
		flag_add_task = false;
	}

	*pResult = 0;
}
