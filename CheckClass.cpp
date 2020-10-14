# include "CheckClass.h"
// Exit on Bad Error

# define REGISTRATION_DLL		L"Registration.dll"
# define MAX_PATH_LEN			1000

# define FN_GET_LIC				"GetLic"
# define FN_GET_HD				"GetHDInfo"
# define FN_TEST_PERIOD			"TestPeriod"
# define FN_TEST_HASH			"TestHash"
# define FN_CREATE_ID			"CreateID"

//# define R_CHECKSUM_OK			0
//# define R_AUTH_OK				0
//# define R_AUTH_FAIL			2
# define R_DLL_LOAD_FAIL		3
# define R_INVALID_REG_DLL		1

typedef BOOL (CALLBACK* LPFNDLLBL)();
typedef BOOL (CALLBACK* LPFNDLLBL_SZ)(LPSTR sz);

CheckClass::CheckClass(LPWSTR* arPath, UINT* arSize, UINT* arChkSum, UINT n)
	:arPath(arPath), arSize(arSize), arChkSum(arChkSum), n(n)
{
	wPath = new WCHAR[MAX_PATH_LEN];
	LPWSTR mPath = new WCHAR[MAX_PATH_LEN];
	GetCurrentDirectory(MAX_PATH_LEN, wPath);
	GetModuleFileName(NULL, mPath, MAX_PATH_LEN);
	FilesClass::GetFilePath(mPath);
	SetCurrentDirectory(mPath);

	hDLL = LoadLibrary(REGISTRATION_DLL);
	if(!hDLL)
		//MessageBox
		exit(R_DLL_LOAD_FAIL);

	delete mPath;
}

CheckClass::~CheckClass()
{
	FreeLibrary(hDLL);
	SetCurrentDirectory(wPath);
	delete wPath;
}

BOOL CheckClass::IntegrityCheck(UINT& ix)
{
	for(UINT i = 0; i < n; i++)
	{
		UINT rVal = CheckFile(arPath[i], arSize[i], arChkSum[i]);
		switch(rVal)
		{
		case R_CHECKSUM_MISMATCH:
			ix = i;
			return FALSE;
		case R_SUCCESS:
			break;
		default:
			exit(rVal);
		}
	}
	return TRUE;
}

BOOL CheckClass::GetLic()
{
	LPFNDLLBL FnGetLic;
	FnGetLic = (LPFNDLLBL)GetProcAddress(hDLL, FN_GET_LIC);
	if(!FnGetLic)
		exit(R_INVALID_REG_DLL);
	UINT rVal = FnGetLic();
	switch(rVal)
	{
	case R_SUCCESS:
		return TRUE;
	default:
		return FALSE;
	}
}

BOOL CheckClass::GetHD()
{
	LPFNDLLBL FnGetHD;
	FnGetHD = (LPFNDLLBL)GetProcAddress(hDLL, FN_GET_HD);
	if(!FnGetHD)
		exit(R_INVALID_REG_DLL);
	UINT rVal = FnGetHD();
	switch(rVal)
	{
	case R_SUCCESS:
		return TRUE;
	default:
		return FALSE;
	}
}

BOOL CheckClass::TestPeriod()
{
	LPFNDLLBL FnTestPeriod;
	FnTestPeriod = (LPFNDLLBL)GetProcAddress(hDLL, FN_TEST_PERIOD);
	if(!FnTestPeriod)
		exit(R_INVALID_REG_DLL);
	UINT rVal = FnTestPeriod();
	switch(rVal)
	{
	case R_SUCCESS:
		return TRUE;
	default:
		return rVal;
	}
}

BOOL CheckClass::TestHash()
{
	LPFNDLLBL FnTestHash;
	FnTestHash = (LPFNDLLBL)GetProcAddress(hDLL, FN_TEST_HASH);
	if(!FnTestHash)
		exit(R_INVALID_REG_DLL);
	else
	{
		UINT rVal = FnTestHash();
		switch(rVal)
		{
		case R_CHECK_FAIL:
			return FALSE;
		case R_SUCCESS:
			return TRUE;
		default:
			//MessageBox(NULL, ERROR_S_BAD_DISK_INFO, MESSAGEBOX_TITLE, MB_OK);
			exit(rVal);
		}
	}
}

BOOL CheckClass::CreateID(LPSTR szMD5)
{
	LPFNDLLBL_SZ FnCreateID;
	FnCreateID = (LPFNDLLBL_SZ)GetProcAddress(hDLL, FN_CREATE_ID);
	if(!FnCreateID)
		exit(R_INVALID_REG_DLL);
	else
	{
		UINT rVal = FnCreateID(szMD5);
		switch(rVal)
		{
		case R_SUCCESS:
			return TRUE;
		default:
			return FALSE;
		}
	}
}
