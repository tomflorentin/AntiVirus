#pragma once

using namespace std;

class Confirmation
{
public:
	Confirmation(wstring const &reason);
	bool Wait();
	~Confirmation();

private:
	void Kill();
	wstring order;
};

