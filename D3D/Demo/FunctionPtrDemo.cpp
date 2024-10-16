#include "stdafx.h"
#include "FunctionPtrDemo.h"
#include <iostream>

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

void Idle()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	printf("%s\n", __FUNCTION__);
}

void Move()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
	printf("%s\n", __FUNCTION__);
}

void Attack()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE);
	printf("%s\n", __FUNCTION__);
}

void Delay(void (*InDelegate)())
{
	printf("3\n");
	Sleep(1000);

	printf("2\n");
	Sleep(1000);

	printf("1\n");
	Sleep(1000);

	InDelegate();
}

int PrintString(string InStr)
{
	printf("InStr : %s\n", InStr.c_str());
	return (int)InStr.length();
}

string MakeStringFromInt(int Lhs, int Rhs)
{
	string result = to_string(Lhs);
	result += " + ";
	result += to_string(Rhs);
	result += " = ";
	result +=  to_string(Lhs + Rhs);

	return result;
}

void FunctionPtrDemo::Initialize()
{
#pragma region GlobalFunction
	//Delegate
	void (*OnBehavior)();	
	OnBehavior = Attack;	//Binding
	OnBehavior();			//Execute

	//Multicast
	void (*OnMulticastBehavior[3])();	
	OnMulticastBehavior[0] = Idle;
	OnMulticastBehavior[1] = Move;
	OnMulticastBehavior[2] = Attack;

	printf("%s\n", "---");
	for (auto It : OnMulticastBehavior)
	{
		It();
	}

	//CallBack
	void (*Delegate)() = Attack;
	//Delay(Delegate);

	int (*RetValOneParam)(string InString);
	RetValOneParam = PrintString;

	int Return = RetValOneParam("Hell World");
	printf("Length : %d\n", Return);
#pragma endregion

#pragma region MemberFunc
	void (*StaticEvent)() = &MyClass::StaticFunc;
	StaticEvent();

	MyClass Object;
	void (MyClass::*MemeberEvent)() = &MyClass::MemberFunc;
	(Object.*MemeberEvent)();
#pragma endregion

#pragma region Functional
	function<void()> FunctionalDelegate = bind(Idle);
	FunctionalDelegate();

	FunctionalDelegate = bind(&MyClass::MemberFunc, Object);
	FunctionalDelegate();

	FunctionalDelegate = bind(&MyClass::StaticFunc);
	FunctionalDelegate();

	function<string(int, int)> RetavlTwoParamDelegate;
	RetavlTwoParamDelegate = bind(MakeStringFromInt, placeholders::_1, placeholders::_2);
	printf("%s\n", RetavlTwoParamDelegate(10, 20).c_str());

	//vector<function<string(int)>> FunctionPtrs;
#pragma endregion
}

void FunctionPtrDemo::Destroy()
{

}

void FunctionPtrDemo::Update()
{
	if (ImGui::Button("Save as csv"))
	{
		function<void(wstring)> OnSaveCompletedDelegate = bind(&FunctionPtrDemo::OnSaveCompleted, this, placeholders::_1);
		Path::SaveFileDialog(L"", L"CSV\0*.csv", L"", OnSaveCompletedDelegate, D3D::GetDesc().Handle);
	}

	if (ImGui::Button("Load as csv"))
	{
		function<void(wstring)> OnLoadCompletedDelegate = bind(&FunctionPtrDemo::OnLoadCompleted, this, placeholders::_1);
		Path::OpenFileDialog(L"", L"CSV\0*.csv", L"", OnLoadCompletedDelegate, D3D::GetDesc().Handle);
	}
}

void FunctionPtrDemo::OnSaveCompleted(wstring path)
{
	Path::CreateFolders("../../_Datas/");

	path += L".csv";
	printf("%s", String::ToString(path).c_str());

	vector<float> RandomFloatArray;
	vector<Vector3> RandomVectorArray;
	for (int i = 0; i < 10; i++)
	{
		RandomFloatArray.push_back(Math::Random(10.f, 20.f));
		RandomVectorArray.push_back(Vector3(i, i, i));
	}

	FILE* Buffer = nullptr;
	fopen_s(&Buffer, String::ToString(path).c_str(), "w");

	for (int i = 0; i < 10; i++)
	{
		fprintf
		(
			Buffer,
			"%f,%f,%f,%f\n",
			RandomFloatArray[i], 
			RandomVectorArray[i].x,
			RandomVectorArray[i].y,
			RandomVectorArray[i].z
		);
	}

	fclose(Buffer);
}

void FunctionPtrDemo::OnLoadCompleted(wstring path)
{
	FILE* Buffer = nullptr;
	fopen_s(&Buffer, String::ToString(path).c_str(), "r");

	vector<float> RandomFloatArray;
	RandomFloatArray.assign(10, float());

	vector<Vector3> RandomVectorArray;
	RandomVectorArray.assign(10, Vector3(1,1,1));

	for (int i = 0; i < 10; i++)
	{
		fscanf_s
		(
			Buffer,
			"%f,%f,%f,%f\n",
			&RandomFloatArray[i],
			&RandomVectorArray[i].x,
			&RandomVectorArray[i].y,
			&RandomVectorArray[i].z
		);
	}

	fclose(Buffer);


	for (int i = 0; i < 10; i++)
	{
		DWORD Color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);

		printf("%f, %f, %f, %f\n,", RandomFloatArray[i], RandomVectorArray[i].x, RandomVectorArray[i].y, RandomVectorArray[i].z);
	}
}
