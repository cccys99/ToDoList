#include "Task.h"
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
//��ȻTask.h�Ѿ������� <string>��<vector>����ͷ�ļ� ������д���Ը�����Ŀ�������ļ��õ�����Щͷ�ļ�

//�����������񴢴��ڸ����ļ������ļ���
void SaveTasksToFile(const std::vector<Task>& tasks, const std::string& fileName)
{
	std::ofstream ostream(fileName);
	ostream << tasks.size();

	//����tasks
	for (const Task& task : tasks)
	{
		std::string description = task.description;
		//���ｫ�ո��滻���»��� ��ʹ����������� ����descriptionʼ�ձ���ȡ��һ���ַ�������ʹ�����ո�
		std::replace(description.begin(), description.end(), ' ', '_');
		//����ǰ����д���ļ� 
		//д��һ�� ���з� ��������� �ո� �����Ƿ���� 1 or 0
		ostream << '\n' << description << ' ' << task.done;
	}

	//����������� �����м������� ���к� ����������� �� ��û����� 0 or 1
}

std::vector<Task> LoadTaskFromFile(const std::string& fileName)
{
	//���ȼ���ļ��Ƿ���� 
	//����filesystem�ᱨ�� ������ �������� ���� c++���Ա�׼ ��Ϊ ISO C++17 ��׼ (/std:c++17)
	if (!std::filesystem::exists(fileName))
	{
		return std::vector<Task>(); //����������ļ� �ͷ���һ����vector
	}

	std::vector<Task> tasks;
	std::ifstream istream(fileName);//����һ������ Ȼ����ļ��ж�ȡ

	int n; //��һ�������������
	istream >> n;

	for (int i = 0; i < n; i++)
	{
		std::string description;
		bool done;

		istream >> description >> done;
		std::replace(description.begin(), description.end(), '_', ' ');
		tasks.push_back(Task{ description,done }); //��������ӵ�vector��
	}

	return tasks;
}
