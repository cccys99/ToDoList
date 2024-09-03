#include "MainFrame.h"
#include <wx/wx.h>
#include <vector>
#include <string>
#include "Task.h"

//构造函数
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	CreatControls();
	BindEventHandlers();
	AddSaveTasks(); //加载保存的txt文件
}

//添加控件
void MainFrame::CreatControls()
{
	//为标题创建一种字体 高度为36 粗体 0是选择一个合适的宽度
	wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold()); 
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	headlineText = new wxStaticText(panel, wxID_ANY, "To-Do List", 
		wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL); // 最后一个参数是style 水平居中
	headlineText->SetFont(headlineFont);
	//headlineText->SetBackgroundColour(*wxRED);

	inputField = new wxTextCtrl(panel, wxID_ANY, "",
		wxPoint(100, 80), wxSize(495, 35),wxTE_PROCESS_ENTER);
	addButton = new wxButton(panel, wxID_ANY, "Add", wxPoint(600, 80), wxSize(100, 35));
	checkListBox = new wxCheckListBox(panel, wxID_ANY, wxPoint(100, 120), wxSize(600, 400));
	clearButton = new wxButton(panel, wxID_ANY, "clear", wxPoint(100, 525), wxSize(100, 35));
}

//绑定事件处理
void MainFrame::BindEventHandlers()
{
	addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
	checkListBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
	clearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClosed, this);
}

//加载保存的txt文件
void MainFrame::AddSaveTasks()
{
	std::vector<Task> tasks = LoadTaskFromFile("tasks.txt");

	for (const Task& task : tasks)
	{
		int index = checkListBox->GetCount();
		checkListBox->Insert(task.description, index);
		checkListBox->Check(index, task.done);
	}
}

void MainFrame::OnAddButtonClicked(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void MainFrame::OnInputEnter(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void MainFrame::OnListKeyDown(wxKeyEvent& evt)
{
	//先检测哪个键被按下了
	switch (evt.GetKeyCode())
	{
	case WXK_DELETE:
		DeleteSelectedTask();
		break;
	case WXK_UP:
		MoveSelectedTask(-1);
		break;
	case WXK_DOWN:
		MoveSelectedTask(1);
		break;
	}
}

void MainFrame::OnClearButtonClicked(wxCommandEvent& evt)
{
	if (checkListBox->IsEmpty())
	{
		return;
	}

	//消息对话框 询问用户是否确认
	wxMessageDialog dialog(this, "Are you sure you want to clear all tasks?",
		"Clear", wxYES_NO | wxCANCEL);
	int result = dialog.ShowModal(); //如果点击yes result就是wxID_YES

	if (result == wxID_YES)
	{
		checkListBox->Clear();
	}
}

//关闭窗口时 将任务保存在文件中
void MainFrame::OnWindowClosed(wxCloseEvent& evt)
{
	std::vector<Task> tasks;

	for (int i = 0; i < checkListBox->GetCount(); i++)
	{
		Task task;
		task.description = checkListBox->GetString(i);
		task.done = checkListBox->IsChecked(i);
		tasks.push_back(task);
	}

	SaveTasksToFile(tasks, "tasks.txt");
	evt.Skip(); //否则关闭不了
}

void MainFrame::AddTaskFromInput()
{
	wxString description = inputField->GetValue();

	//如果非空
	if (!description.IsEmpty())
	{
		checkListBox->Insert(description, checkListBox->GetCount());
		inputField->Clear(); //点击add后输入框清除
	}

	inputField->SetFocus(); //光标仍然在输入框 不用单击输入框可以接着输入
}

void MainFrame::DeleteSelectedTask()
{
	int selectedIndex = checkListBox->GetSelection();

	if (selectedIndex == wxNOT_FOUND)
	{
		return; //如果这里的索引是wxNOT_FOUND 就意味着没有选择任何项目 此时只需返回
	}

	checkListBox->Delete(selectedIndex); //删除索引处的项目
}

void MainFrame::MoveSelectedTask(int offset) //偏移量为 -1 or 1 上移或下移
{
	int selectedIndex = checkListBox->GetSelection();

	if (selectedIndex == wxNOT_FOUND)
	{
		return;
	}

	//如果索引有效 则计算所选任务的新索引
	int newIndex = selectedIndex + offset;

	//交换所选索引处和新索引处的任务
	if (newIndex >= 0 && newIndex < checkListBox->GetCount())
	{
		SwapTasks(selectedIndex, newIndex); //交换任务
		checkListBox->SetSelection(newIndex, true);
	}
}

//交换任务函数 将索引处的两个任务交换
void MainFrame::SwapTasks(int i, int j) 
{
	//创建任务i 和任务j
	//这里getstring返回的是标准字符串 转换成stdstring
	Task taskI{ checkListBox->GetString(i).ToStdString(),checkListBox->IsChecked(i) };
	Task taskJ{ checkListBox->GetString(j).ToStdString(),checkListBox->IsChecked(j) };

	//更新清单框的内容
	checkListBox->SetString(i, taskJ.description);
	checkListBox->Check(i, taskJ.done);

	checkListBox->SetString(j, taskI.description);
	checkListBox->Check(j, taskI.done);
}
