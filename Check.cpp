# include "BuildSets.h"
/*
	_PROTECTION_DISABLED
	PROTECTION_N_DISABLED
*/

# include <CheckParams.h>
# include <Params.h>

# include "Check.h"

# define ALIGN_B_BORDER			32
# define KEY_LEN				33

# define ERROR_S_NOT_REGISTERED	L"Not Registered !"
# define ERROR_S_BAD_DISK_INFO	L"Inconsistent HD info"
# define ERROR_S_FILE_CORRUPTED	L"File Corrupted"
# define ERROR_S_LIC_VIOLATED	L"License Violated"
# define ERROR_S_LIC_EXPIERED	L"License Expiered"
# define ERROR_S_HD_IFO_INVALID	L"HD Info invalid"

# define S_MESSAGE_TITLE		COMPONENT
# define SA_MESSAGE_COPY_ID		"Copy Registration Key to Clipboard"

# define INTEG_CHECK_FAIL		0x0000151
# define AUTH_CHECK_FAIL		0x0000101

BOOL Check()
{
__declspec (align(ALIGN_B_BORDER))\
	UINT arChkSum[] = CKSM_LIST;

	LPWSTR arPath[] = PATH_LIST;
	UINT arSize[] = SIZE_LIST;
	CheckClass Check(arPath, arSize, arChkSum, N_FILES);

	{
	UINT ix;
	if(!Check.IntegrityCheck(ix))
	{
# ifndef PROTECTION_N_DISABLED
		MessageBox(NULL, arPath[ix], ERROR_S_FILE_CORRUPTED, MB_OK);
# endif

# ifndef PROTECTION_I_DISABLED
		return FALSE;
# endif
	}
	}

	if(Check.GetHD())
	{
		if(Check.GetLic())
		{
			if(Check.TestPeriod())
			{
				if(Check.TestHash())
					return TRUE;
				else
				{
# ifndef PROTECTION_N_DISABLED
					MessageBox(NULL, ERROR_S_LIC_VIOLATED, S_MESSAGE_TITLE, MB_OK);
# endif
				}
			}
			else
			{
# ifndef PROTECTION_N_DISABLED
				MessageBox(NULL, ERROR_S_LIC_EXPIERED, S_MESSAGE_TITLE, MB_OK);
# endif
			}
		}
	}
	else
	{
# ifndef PROTECTION_N_DISABLED
		MessageBox(NULL, ERROR_S_HD_IFO_INVALID, S_MESSAGE_TITLE, MB_OK);
# endif
		return FALSE;
	}
# ifndef PROTECTION_N_DISABLED
	MessageBox(NULL, ERROR_S_NOT_REGISTERED, S_MESSAGE_TITLE, MB_OK);
# endif
	{
	CHAR szMD5[KEY_LEN];
	Check.CreateID(szMD5);
	if(MessageBoxA(NULL, szMD5, SA_MESSAGE_COPY_ID, MB_YESNO) == IDYES)
		CopyToClpBrd((LPVOID) szMD5, KEY_LEN, CF_TEXT);
	return FALSE;
	}
}
