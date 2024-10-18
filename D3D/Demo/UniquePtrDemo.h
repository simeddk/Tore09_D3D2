#pragma once

#include "Systems/IExecute.h"

class Object : public enable_shared_from_this<Object>
{
public:
	Object()
	{
		printf("�����ڰ� ȣ���\n");
		data = new int[10];
	}

	~Object()
	{
		printf("�Ҹ��ڰ� ȣ���\n");
		delete[] data;
	}

	shared_ptr<Object> GetThis()
	{
		return shared_from_this(); 
	}

	void SetZero(int value)
	{
		data[0] = value;
	}

	int GetZero()
	{
		return data[0];
	}

	void SetOtherObject(weak_ptr<Object> InOther)
	{
		otherObject = InOther;
	}

	void IsAlive()
	{
		shared_ptr<Object> ptr = otherObject.lock();
		if (ptr != nullptr)
		{
			printf("0x%x ����\n", ptr.get());
		}
		else
		{
			printf("����\n");
		}
	}

private:
	int* data;

	weak_ptr<Object> otherObject;
};

class UniquePtrDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override {};
	virtual void Update() override {};
	virtual void PreRender() override {};
	virtual void Render() override {};
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	
};