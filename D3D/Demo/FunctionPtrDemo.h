#pragma once

#include "Systems/IExecute.h"

class MyClass
{
public:
	static void StaticFunc()
	{
		printf("%s\n", __FUNCTION__);
	}

	void MemberFunc()
	{
		printf("%s\n", __FUNCTION__);
	}
};

class FunctionPtrDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override {};
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};
};
