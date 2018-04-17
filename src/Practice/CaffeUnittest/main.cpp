#include "stdafx.h"
#include "Fixture.hpp"

#include "CppUnit/TestRunner.h"
#include <vector>
#include <string>

using namespace std;

int main(int ac, char **av)
{
	Fixture::GetGlobal().Init();

	int result = 0;
	{
		vector<string> args(av, av + ac);
		if (args.size() == 1)
		{
			args.push_back("-all");
		}

		CppUnit::TestRunner runner;
		runner.addTest("CoreSystemTest", Fixture::GetGlobal().DetachTestSuite());
		result = runner.run(args) ? 0 : 1;
	}

	Fixture::GetGlobal().Exit();

//	_CrtDumpMemoryLeaks();
	return result;
}
