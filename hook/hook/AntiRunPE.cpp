#include "stdafx.h"
#include "AntiRunPE.h"
#include <unordered_map>

AntiRunPE::AntiRunPE()
{
}

void AntiRunPE::LogAction(std::wstring const & action, void * param)
{
	actions.push_back({ action, param });
	checkRunPe();
}

AntiRunPE::~AntiRunPE()
{
}

void AntiRunPE::checkRunPe()
{
	unordered_map<void *, unordered_map<std::wstring, bool>> log; // log[POINTER][SYSCALL]

	for (auto const &l : this->actions) {
		log[l.param];
		log[l.param][l.api] = true;
	}

	for (auto const &process : log) {
		try {
			process.second.at(L"NtUnmapViewOfSection");
			process.second.at(L"NtWriteVirtualMemory");
			//process.second.at(L"NtGetContextThread");
			//process.second.at(L"NtSetContextThread");
			this->flagRunPe();
		}
		catch (std::out_of_range const &ex)
		{
			std::cerr << "RunPE not found" << std::endl;
		}
	}

}

void AntiRunPE::flagRunPe()
{
	Beep(5000, 5000);
	std::cerr << "RUN PE // RUN PE // RUN PE" << std::endl;
}
