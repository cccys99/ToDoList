#include "Task.h"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
//虽然Task.h已经包含了 <string>和<vector>两个头文件 但这样写可以更清楚的看到这个文件用到了哪些头文件

//将给定的任务储存在给定文件名的文件中
void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& fileName)
{
	std::ofstream ostream(fileName);
	ostream << tasks.size();

	//遍历tasks
	for (const Task& task : tasks)
	{
		std::string description = task.description;
		//这里将空格替换成下划线 会使加载任务更简单 这样description始终被读取成一个字符串（即使包含空格）
		std::replace(description.begin(), description.end(), ' ', '_');
		//将当前任务写入文件 
		//写入一个 换行符 任务的描述 空格 任务是否完成 1 or 0
		ostream << '\n' << description << ' ' << task.done;
	}

	//所以整体就是 先是有几个任务 换行后 是任务的描述 加 有没有完成 0 or 1
}

std::vector<Task> LoadTaskFromFile(const std::string& fileName)
{
	//首先检查文件是否存在 
	//这里filesystem会报错 将属性 配置属性 常规 c++语言标准 改为 ISO C++17 标准 (/std:c++17)
	if (!std::filesystem::exists(fileName))
	{
		return std::vector<Task>(); //如果不存在文件 就返回一个空vector
	}

	std::vector<Task> tasks;
	std::ifstream istream(fileName);//创建一个向量 然后从文件中读取

	int n; //第一行是任务的数量
	istream >> n;

	for (int i = 0; i < n; i++)
	{
		std::string description;
		bool done;

		istream >> description >> done;
		std::replace(description.begin(), description.end(), '_', ' ');
		tasks.push_back(Task{ description,done }); //将任务添加到vector中
	}

	return tasks;
}
