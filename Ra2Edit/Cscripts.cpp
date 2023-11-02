// Cscripts.cpp : 实现文件
//

#include "stdafx.h"
#include "Ra2Edit.h"
#include "Cscripts.h"
#include "afxdialogex.h"
#include "OtherTools.h"
#include "Army.h"

int Count_sc = 0;
CString add_title_sc;

Cscripts ScriptsWind;

void ReadScript(){
	int poss, pose, len, len_temp_txt, psN = 0;
	char* temp, *temp_txt, *Line_num_txt = NULL;
	CString num, title, content, note, txt;
	while (psPos[psN].pos != NULL){
		poss = TxtMemFind(psPos[psN].size, 0, psPos[psN].pos, "[ScriptTypes]", C_END);
		if (poss == -1){ psN += 1; continue; }
		pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "[", C_HEAD);
		if (pose == -1){
			pose = TxtMemFind(psPos[psN].size, poss, psPos[psN].pos, "\0", C_HEAD);
		}
		temp = ReadMemContent(psPos[psN].pos, psPos[psN].size, poss, pose);
		len = strlen(temp);
		temp_txt = GetMemLines(temp, len); // 与加载进来的源文件无关
		len_temp_txt = GetMemLinesNum(temp);

		// 临时结构体,std::vector模块专用
		struct Scripts temp_t;
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
				scripts.push_back(temp_t);
			}
		}
		psN += 1;
	}
	ScriptSortByNum(scripts);

	main_script_num.reserve(strlen("[ScriptTypes]\n"));
	main_script_num.insert(main_script_num.end(), "[ScriptTypes]\n", "[ScriptTypes]\n" + std::strlen("[ScriptTypes]\n"));
	int lenCount = 0;
	for (int i = 0; i < scripts.size(); i++) {
		std::stringstream ss;
		ss << i+start_regnum << "=" << scripts[i].title;
		std::string temp_str = ss.str();
		lenCount += temp_str.length();
		main_script_num.reserve(lenCount + 1); // 为换行符预留一个字节
		main_script_num.insert(main_script_num.end(), temp_str.c_str(), temp_str.c_str() + temp_str.length());
		main_script_num.push_back('\n');
		scripts[i].num = -1; // 清空嵌套指针
	}
	CString temp_C;
	char* temp_c;
	main_script.push_back('\n');
	for (int i = 0; i < scripts.size(); i++) {
		temp_C.Format(_T("[%s]"), (CString)scripts[i].title);
		temp_c = CStringToChar(temp_C);
		lenCount += strlen(temp_c);
		main_script.reserve(lenCount + 1);
		main_script.insert(main_script.end(), temp_c, temp_c + strlen(temp_c));
		main_script.push_back('\n');

		temp_c = scripts[i].txt;
		temp_c = GetMemLines(temp_c, strlen(temp_c));
		lenCount += strlen(temp_c);
		main_script.reserve(lenCount + 1);
		main_script.insert(main_script.end(), temp_c, temp_c + strlen(temp_c));
		main_script.push_back('\n');
		scripts[i].title = NULL; // 清空嵌套指针
		scripts[i].txt = NULL; // 清空嵌套指针
		scripts[i].note = NULL; // 清空嵌套指针
	}
}

// Cscripts 对话框

IMPLEMENT_DYNAMIC(Cscripts, CDialogEx)

Cscripts::Cscripts(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cscripts::IDD, pParent)
{
	pRa2EditDlg = dynamic_cast<CRa2EditDlg*>(pParent);
}

Cscripts::~Cscripts()
{
}

void Cscripts::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_SCRIPT_LIST, m_Script_list);
	DDX_Control(pDX, IDC_RICHEDIT21, m_script_redit);
	DDX_Control(pDX, IDC_TIPS, m_tip_txt);
	//DDX_Control(pDX, IDC_CHECK1, m_autosave_ck);
}


BEGIN_MESSAGE_MAP(Cscripts, CDialogEx)
	/*ON_BN_CLICKED(IDC_BUTTON1, &Cscripts::OnBnClickedButton1)*/
	ON_BN_CLICKED(IDC_BUTTON2, &Cscripts::OnBnClickedButton2)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, &Cscripts::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &Cscripts::OnBnClickedButton3)
	ON_NOTIFY(NM_CLICK, IDD_SCRIPT_LIST, &Cscripts::OnNMClickScriptList)
	ON_BN_CLICKED(IDC_BUTTON5, &Cscripts::OnBnClickedButton5)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON4, &Cscripts::OnBnClickedButton4)
	ON_NOTIFY(LVN_KEYDOWN, IDD_SCRIPT_LIST, &Cscripts::OnLvnKeydownScriptList)
END_MESSAGE_MAP()


// Cscripts 消息处理程序
int Cscripts::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
}


// 关闭窗口
void Cscripts::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	ScriptsWind.OnCancel();
}

bool Once_ScriptInit = true;
void Cscripts::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
	if (Once_ScriptInit){
		m_Script_list.SetExtendedStyle(m_Script_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		//插入标题
		CString head[] = { TEXT("注册序号"), TEXT("注册名称"), TEXT("注释") };

		//插入列
		for (int i = 0; i < sizeof(head) / sizeof(head[0]); i++){
				m_Script_list.InsertColumn(i, head[i], LVCFMT_LEFT, 100);
		}

		CFont font;
		font.CreatePointFont(120, _T("微软雅黑"));
		m_script_redit.SetFont(&font);


		Once_ScriptInit = false;
	}

	return ;
}


bool Script_List_init = true;
void Cscripts::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO:  在此处添加消息处理程序代码
	// 解决载入问题
	bool flag = false;
	if (FilePath_main != "None"&& FilePath_main != "Nones")flag = true;
	if (Script_List_init&&flag){
		CString num, title, note; // 注册序号
		std::istringstream num_stream(main_script_num);
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
			pos = main_script.find("[" + reg_title + "]");
			if (pos == std::string::npos) {
				continue;
			}
			pos = main_script.find("Name=", pos);
			if (pos == std::string::npos) {
				continue;
			}
			pos += 5;
			size_t end_pos = main_script.find('\n', pos);
			if (end_pos == std::string::npos) {
				end_pos = main_script.size();
			}

			// 插入列表
			num.Format(_T("%s"), CString(reg_num.c_str()));
			title.Format(_T("%s"), CString(reg_title.c_str()));
			note.Format(_T("%s"), CString(main_script.substr(pos, end_pos - pos).c_str()));
			m_Script_list.InsertItem(team_idx, num);
			m_Script_list.SetItemText(team_idx, 1, title);
			m_Script_list.SetItemText(team_idx, 2, note);
			++team_idx;
		}

		m_Script_list.RedrawWindow();
		ScriptsWind.Invalidate();
		ScriptsWind.UpdateWindow();
		Script_List_init = false;
	}

	if (flag_fresh_sc){
		int count = m_Script_list.GetItemCount();
		CString temp;
		for (int i = 0; i < count; i++){
			temp = m_Script_list.GetItemText(i, 1);
			if (temp == flag_fresh_sc_title){ // 如果找到了
				m_Script_list.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
				m_script_redit.SetWindowTextW((CString)FindiItem(main_script, CStringToChar(flag_fresh_sc_title)).data());
				m_tip_txt.SetWindowTextW(flag_fresh_sc_title);
			}
			else m_Script_list.SetItemState(i, 0, LVIS_FOCUSED | LVIS_SELECTED);
		}
		flag_fresh_sc = false;
	}
}

// 保存内容
void Cscripts::OnBnClickedButton1()
{
	// 查找内容
	std::string temp;
	CString title,lenC,ll;
	title = (CString)"[";
	m_tip_txt.GetWindowTextW(ll);
	title += ll;
	title += (CString)"]";

	char* lenc;
	m_script_redit.GetWindowTextW(lenC);
	lenc = CStringToChar((CString)FindiItem(main_script, CStringToChar(ll)).data());
	int posEnd = TxtMemFind(main_script.size(), 0, main_script.data(), CStringToChar(title), R_BF) + strlen(lenc);
	int posStart = TxtMemFind(main_script.size(), 0, main_script.data(), CStringToChar(title), R_BF);

	// 开头 修改位置 结尾
	temp += main_script.substr(0, posStart);
	temp += removeExtraNewlines(GetMemLines(CStringToChar(lenC), strlen(CStringToChar(lenC))));
	temp += main_script.substr(posEnd);

	main_script.clear();
	main_script = temp;
	temp.clear();
	free(lenc);

	// 更新列表最新的注释
	CString tit,o_tit;
	std::string upnote;
	m_tip_txt.GetWindowTextW(tit);
	upnote = FindiItem(main_script,CStringToChar(tit));
	upnote = FindMemSTDS(upnote,"Name=");
	int count = m_Script_list.GetItemCount();
	for (int i = 0; i < count; i++){
		o_tit = m_Script_list.GetItemText(i, 1);
		if (o_tit == tit){
			m_Script_list.SetItemText(i, 2, (CString)upnote.data());
		}
	}
	flag_updata_sc = true;
}

// 删除内容
void Cscripts::OnBnClickedButton3()
{
	int nIndex = m_Script_list.GetNextItem(-1, LVNI_SELECTED);
	while (nIndex != -1){
		CString title;
		m_Script_list.DeleteItem(nIndex);
		nIndex = m_Script_list.GetNextItem(-1, LVNI_SELECTED);
	}
	int pouch = m_Script_list.GetItemCount();
	CString temp_num = (CString)"[ScriptTypes]\n", txt,temp;
	for (int i = 0; i < pouch; i++){
		// 将标题序号加载到内存中保存
		txt.Format(_T("%d=%s\n"), i+start_regnum, m_Script_list.GetItemText(i, 1));
		temp_num += txt;
	}
	
	for (int i = 0; i < pouch; i++){
		CString title = m_Script_list.GetItemText(i, 1);
		CString dat = (CString)FindiItem(main_script, CStringToChar(title)).data();
		txt.Format(_T("%s\n"), dat);
		temp += txt;
	}
	for (int i = 0; i < pouch; i++){
		// 将标题序号输出到表格中
		CString num;
		num.Format(_T("%d"), i+start_regnum);
		m_Script_list.SetItemText(i, 0, num);
	}
	main_script_num.clear();
	main_script.clear();
	main_script = CStringToChar(temp);
	main_script_num = CStringToChar(temp_num);
	main_script_num += "\n";
	flag_updata_sc = true;

	return;
}
bool flag_add_sc = false;
// 单击查看事件
void Cscripts::OnNMClickScriptList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// 获取点击位置
	int nIndex = m_Script_list.GetNextItem(-1, LVNI_SELECTED);
	if (nIndex != -1){ // 如果点击非空
		char* Item = CStringToChar(m_Script_list.GetItemText(nIndex, 1));
		m_script_redit.SetWindowTextW((CString)FindiItem(main_script, Item).data());
		m_tip_txt.SetWindowTextW(m_Script_list.GetItemText(nIndex, 1));
	}

	if (flag_add_sc){
		CString num;
		num.Format(_T("%d"), Count_sc+start_regnum);
		m_Script_list.InsertItem(Count_sc, num);
		m_Script_list.SetItemText(Count_sc,1,add_title_sc);
		m_Script_list.SetItemText(Count_sc, 2, (CString)"Ruffy");
		Count_sc += 1;
		flag_add_sc = false;
	}

	*pResult = 0;
}

// 新增脚本
void Cscripts::OnBnClickedButton5()
{
	Count_sc = m_Script_list.GetItemCount();// 原因不明为何不用加1
	Add_new_init = true;
	add_type = 2;
	add_wind.ShowWindow(SW_HIDE);
	add_wind.ShowWindow(SW_NORMAL);
}


void Cscripts::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (flag_add_sc){
		CString num;
		num.Format(_T("%d"), Count_sc+start_regnum);
		m_Script_list.InsertItem(Count_sc, num);
		m_Script_list.SetItemText(Count_sc, 1, add_title_sc);
		m_Script_list.SetItemText(Count_sc, 2, (CString)"Ruffy");
		Count_sc += 1;
		flag_add_sc = false;
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

// 理顺内容
void Cscripts::OnBnClickedButton4()
{
	CString content,temp_c,txt,title;
	m_tip_txt.GetWindowTextW(title);
	txt.Format(_T("[%s]\n"), title);
	std::string stdt,line;
	m_script_redit.GetWindowTextW(content);
	stdt = CStringToChar(content);
	txt += (CString)"Name=";
	txt += FindMemSTDS(stdt, "Name=").data(); // 获取注释
	content = (CString)GetMemLines(CStringToChar(content), strlen(CStringToChar(content)));
	int count = GetMemLinesNum(CStringToChar(content));
	int nn = 0;
	for (int i = 1; i < count+1; i++){
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
	m_script_redit.SetWindowTextW(txt);
	return;
}


void Cscripts::OnLvnKeydownScriptList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nIndex = m_Script_list.GetNextItem(-1, LVNI_SELECTED);
	if (nIndex != -1){ // 如果点击非空
		char* Item = CStringToChar(m_Script_list.GetItemText(nIndex, 1));
		m_script_redit.SetWindowTextW((CString)FindiItem(main_script, Item).data());
		m_tip_txt.SetWindowTextW(m_Script_list.GetItemText(nIndex, 1));
	}

	if (flag_add_sc){
		CString num;
		num.Format(_T("%d"), Count_sc+start_regnum);
		m_Script_list.InsertItem(Count_sc, num);
		m_Script_list.SetItemText(Count_sc, 1, add_title_sc);
		m_Script_list.SetItemText(Count_sc, 2, (CString)"Ruffy");
		Count_sc += 1;
		flag_add_sc = false;
	}
	*pResult = 0;
}
