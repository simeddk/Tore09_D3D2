#include "stdafx.h"
#include "ThreadDemo.h"
#include <iostream>

#ifndef Loop
void Loop1()
{
	for (int i = 0; i < 10; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4);
		printf("%s : %d\n", __FUNCTION__, i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);
	}

	printf("%s is completed\n", __FUNCTION__);
}

void Loop2()
{
	for (int i = 0; i < 10; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x2);
		printf("%s : %d\n", __FUNCTION__, i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);
	}

	printf("%s is completed\n", __FUNCTION__);
}

void Loop3()
{
	for (int i = 0; i < 10; i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x1);
		printf("%s : %d\n", __FUNCTION__, i);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);
	}

	printf("%s is completed\n", __FUNCTION__);
}
void LoopOneParam(int count)
{
	for (int i = 0; i < count; i++)
	{
		thread::id ThreadID = this_thread::get_id();
		printf("ThreadID : %x, Iterator : %d\n", ThreadID, i);
	}
}
#endif

#ifndef Devide&Conquer
void EashSum(vector<int>::iterator Start, vector<int>::iterator End, int* Sum)
{
	vector<int>::iterator It = Start;
	for (It; It != End; ++It)
	{
		*Sum += *It;
	}
}
#endif

void ThreadDemo::Initialize()
{
	/*thread thread1(Loop1);
	thread thread2(Loop2);
	thread thread3(Loop3);

	thread1.detach();
	printf("Thread1 is aborted\n");

	thread2.detach();
	printf("Thread2 is aborted\n");

	thread3.detach();
	printf("Thread3 is aborted\n");*/

	/*thread t1(LoopOneParam, 10);
	thread t2(LoopOneParam, 10);
	t1.detach();
	t2.detach();*/

	/*vector<int> Datas;
	for (int i = 0; i <= 100; i++)
	{
		Datas.push_back(i);
	}

	vector<int> EashResults(4);
	vector<thread> Threads;
	for (int i = 0; i < 4; i++)
	{
		Threads.push_back(thread(
			EashSum, 
			Datas.begin() + i * 25,
			Datas.begin() + (i + 1) * 25,
			&EashResults[i]
		));
	}

	for (int i = 0; i < 4; i++)
	{
		Threads[i].join();
	}

	int Result = 0;
	for (int i = 0; i < 4; i++)
	{
		Result += EashResults[i];
	}

	printf("Result : %d\n", Result);*/

	//SharedResource();
	
	//RaceCondition();
	
	//Deadlock();

	ProviderAndComsumer();
}


void ThreadDemo::Update()
{
	progress += Time::Delta() / 3;

	ImGui::ProgressBar(progress);
}

void ThreadDemo::SharedResource()
{
	thread t([=]() 
	{ 
		while (true)
		{
			Sleep(Time::Delta());
			printf("Progress : %f\n", progress);

			if (progress >= 1.f)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x2);
				printf("Done\n");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);
				break;
			}
		}
	});

	t.detach();
}

void ThreadDemo::RaceCondition()
{
	int Count = 0;

	vector<thread> Threads;
	for (int i = 0; i < 4; i++)
	{
		function<void(int&)> Delegate;
		Delegate = bind(&ThreadDemo::IncreaseCount, this, placeholders::_1);
		Threads.push_back(thread(Delegate, ref(Count)));
	}

	for (int i = 0; i < 4; i++)
	{
		Threads[i].join();
	}

	printf("Count : %d\n", Count);
}

void ThreadDemo::IncreaseCount(int& count)
{
	lock_guard<mutex> lock(m);
	for (int i = 0; i < 1000000; i++)
	{
		count++;
	}
}

void ThreadDemo::Deadlock()
{
	int Count = 0;
	mutex m1, m2;

	function<void(mutex&, mutex&)> Delegate1;
	Delegate1 = bind(&ThreadDemo::Thread1, this, placeholders::_1, placeholders::_2);
	thread t1(Delegate1, ref(m1), ref(m2));

	function<void(mutex&, mutex&)> Delegate2;
	Delegate2 = bind(&ThreadDemo::Thread2, this, placeholders::_1, placeholders::_2);
	thread t2(Delegate2, ref(m1), ref(m2));

	t1.join();
	t2.join();

	cout << "메인 루틴 끝" << endl;
}

void ThreadDemo::Thread1(mutex& m1, mutex& m2)
{
	for (int i = 0; i < 10000; i++)
	{
		lock_guard<mutex> Lock1(m1);
		lock_guard<mutex> Lock2(m2);

		thread::id ThreadID = this_thread::get_id();
		cout << "ThreadID : " << ThreadID << ", Iteraotr : " << i << endl;
	}
}

void ThreadDemo::Thread2(mutex& m1, mutex& m2)
{
	for (int i = 0; i < 10000; i++)
	{
		/*lock_guard<mutex> Lock2(m2);
		lock_guard<mutex> Lock1(m1);

		thread::id ThreadID = this_thread::get_id();
		cout << "ThreadID : " << ThreadID << ", Iteraotr : " << i << endl;*/

		while (true)
		{
			m2.lock();

			if (m1.try_lock() == false)
			{
				m2.unlock();
				continue;
			}

			thread::id ThreadID = this_thread::get_id();
			cout << "ThreadID : " << ThreadID << ", Iteraotr : " << i << endl;

			m1.unlock();
			m2.unlock();
			break;
		}
	}
}


string ThreadDemo::Datas[5] = { "C++", "JAVA", "C#", "Phyton", "Perl" };

void Provider(queue<string>* queue, mutex* m, int index)
{
	string* Datas = ThreadDemo::GetDatas();

	for (int i = 0; i < 5; i++)
	{
		this_thread::sleep_for(chrono::microseconds(100 * index));
		string Contents = "ThreadIndex[" + to_string(index) + "], Value = " + Datas[i];
		
		m->lock();
		queue->push(Contents);
		m->unlock();
	}
}

void Comsumer(queue<string>* queue, mutex* m, int* count)
{
	while (*count < 25)
	{
		m->lock();

		string Poped = queue->front();
		queue->pop();

		//Todo.

		(*count)++;
		m->unlock();

		printf("Count : %d, Poped : %s\n", *count, Poped.c_str());
		this_thread::sleep_for(chrono::milliseconds(1));
	}
}

void ThreadDemo::ProviderAndComsumer()
{
	vector<thread> Sender;
	mutex m;

	for (int i = 0; i < 5; i++)
	{
		Sender.push_back(thread(Provider, &Queue, &m, i));
	}

	int ProcessCount;
	thread Receiver(Comsumer, &Queue, &m, &ProcessCount);


}



