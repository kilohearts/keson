#define CATCH_CONFIG_RUNNER
#include "catch.h"

int main(int argc, char* argv[])
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	Catch::Session s;

	int r = s.applyCommandLine(argc, argv);

	if (r == 0)
	{
		Catch::Timer timer = Catch::Timer();
		timer.start();
		r = s.run();
		uint64_t usec = timer.getElapsedMicroseconds();
		std::cout << "All tests completed in " << std::setprecision(1) << std::fixed << (usec / 1000.0) << "ms" << std::endl;
	}

	return r;
}
