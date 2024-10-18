#include "stdafx.h"
#include "SharedPtrDemo.h"
#include "UniquePtrDemo.h"
#include <iostream>
using namespace std;

void SharedPtrDemo::Initialize()
{
	//RefCount
	//shared_ptr<Object> s1(new Object());
	//shared_ptr<Object> s2(s1);
	//shared_ptr<Object> s3(s2);

	//cout << s1.use_count() << endl;
	//s1.reset();

	//cout << s2.use_count() << endl;
	//s2.reset();

	//cout << s3.use_count() << endl;
	//s3.reset();

	////Sleep(2000);

	//shared_ptr<Object> s1 = make_shared<Object>();

	//RawPoint & This Pointer
	/*shared_ptr<Object> ptr = make_shared<Object>();
	cout << ptr << endl;

	ptr->GetThis()->SetZero(99);
	cout << ptr->GetThis()->GetZero() << endl;*/

	shared_ptr<Object> Player = make_shared<Object>();
	shared_ptr<Object> Enemy = make_shared<Object>();
	
	Player->SetOtherObject(Enemy);
	Enemy->SetOtherObject(Player);

	cout << Player.use_count() << endl;
	cout << Enemy.use_count() << endl;

	//Player.reset();
	Enemy->IsAlive();

}
