#pragma once

using namespace std;

class Confirmation
{
public:
	Confirmation(wstring const &reason);
	bool Wait();
	~Confirmation();
	static void Kill();

private:
	wstring order;
};

