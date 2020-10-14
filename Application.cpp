# include "Application.h"

# define ERROR_VALIDATION			0x001e

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nShowCmd)
{
	if(!Check())
	{
# ifndef PROTECTION_DISABLED
		exit(ERROR_VALIDATION);
# endif
	}

	return 0;
}
