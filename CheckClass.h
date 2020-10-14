# ifndef CHECK_CLASS_HEADER
# define CHECK_CLASS_HEADER

# include <Windows.h>
# include "..\\Utilities\\Files\\FilesClass.h"
# include "CheckFile.h"

# define R_SUCCESS			0

// Linked to RagistrationClass.h
# define R_CHECK_FAIL		12
// Linked to ValidateTime.h
// Linked to CheckFile.cpp
# define R_CHECKSUM_MISMATCH	1

class CheckClass
{
private:
	LPWSTR *arPath;
	UINT *arSize;
	UINT *arChkSum;
	UINT n;
	UINT i;

	LPWSTR wPath;
	HINSTANCE hDLL;

public:
	CheckClass(LPWSTR* arPath, UINT* arSize, UINT* arChkSum, UINT n);
	~CheckClass();

	BOOL IntegrityCheck(UINT& ix);
	BOOL GetLic();
	BOOL GetHD();
	BOOL TestPeriod();
	BOOL TestHash();
	BOOL CreateID(LPSTR szMD5);
};

# endif // CHECK_CLASS_HEADER
