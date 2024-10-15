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
	if (ImGui::Button("Save", ImVec2(120, 120)))
	{
		function<void(wstring)> OnSaveCompletedDelegate = bind(); //Todo.
		Path::SaveFileDialog(L"", L"CSV\0*.csv", L"",  );
	}
}
