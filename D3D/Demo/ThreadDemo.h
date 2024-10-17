#pragma once

#include "Systems/IExecute.h"

class ThreadDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override {};
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override {};
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	void SharedResource();

	void RaceCondition();
	void IncreaseCount(int& count);
	
	void Deadlock();
	void Thread1(mutex& m1, mutex& m2);
	void Thread2(mutex& m1, mutex& m2);

	void ProviderAndComsumer();

private:
	float progress = 0.f;
	mutex m;

	queue<string> Queue;

public:
	static string Datas[5];
	static string* GetDatas() { return Datas; }
};