#pragma once

#include <map>

struct action
{
	std::wstring api;
	void *param;
};

class AntiRunPE
{
public:
	AntiRunPE();
	void LogAction(std::wstring const &action, void *param);
	~AntiRunPE();

private:
	void checkRunPe();
	void flagRunPe();
	vector<action> actions;
};

