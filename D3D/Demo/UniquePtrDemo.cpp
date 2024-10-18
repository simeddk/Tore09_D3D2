#include "stdafx.h"
#include "UniquePtrDemo.h"
#include <iostream>
using namespace std;

void Exception()
{
	throw -999;
}

void SetBlabla(unique_ptr<Object>& param)
{
	cout << param->GetZero() << endl;
}

void Create()
{
	unique_ptr<Object> o1(new Object());
		
	o1->SetZero(100);
	printf("%d\n", o1->GetZero());

	unique_ptr<Object> o2;
	o2 = move(o1);

	printf("0x%x\n", o1.get());
	printf("0x%x\n", o2.get());
	
	SetBlabla(o1);

	//unique_ptr<Object> o3(new Object());
	//unique_ptr<Object> o3 = make_unique<Object>();

	vector<unique_ptr<Object>> objects;
	//objects.push_back(new Object());
	objects.emplace_back(new Object());

	Exception();
}

void UniquePtrDemo::Initialize()
{
	try
	{
		Create();
	}
	catch (int Message)
	{
		printf("예외 발생 : %d\n", Message);
	}
}
