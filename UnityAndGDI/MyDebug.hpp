#pragma once
#include<string>
#include<fstream>

using namespace std;

class FileDebug
{
public:
	static void Log(string message)
	{
		ofstream fout;

		//ios_base::app�����׷�����ݶ������ǵ�֮ǰ������,����Append����д��Ĭ���ǻḲ�ǵ�֮ǰ���ݵġ�
		//���һ��ʼû�������־�ļ��Ļ�����ô���ᴴ�����Ŀ¼�ļ���������ļ�·��������˵��
		fout.open("LogFile.txt", ios_base::app);
		fout << message << '\n';
		fout.close();
	}
};