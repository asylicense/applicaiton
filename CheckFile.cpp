# include "CheckFile.h"

# define R_CHECKSUM_OK			0
# define R_CHECKSUM_MISMATCH	1
# define R_FILE_NOT_FOUND		2
# define R_FILE_OPEN_ERROR		3
# define R_FILE_SIZE_MISMATCH	4

# define ERROR_S_FILE_NOT_FOUND	L"File not found"
# define ERROR_S_FILE_OPEN		L"File open Error"

UINT CheckFile(LPWSTR szPath, UINT xSize, UINT xChkSum)
{
	HANDLE hFile;
	DWORD nRead;
	UINT size;
	UINT aChkSum = 0;
	UINT n;
	UINT *Buffer;
	BOOL bOdd;

	hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		if(GetLastError() == ERROR_FILE_NOT_FOUND)
			return R_FILE_NOT_FOUND;
		else
			return R_FILE_OPEN_ERROR;
	}

	size = GetFileSize(hFile, NULL);
# ifndef _DEBUG
	if(size != xSize)
		return R_FILE_SIZE_MISMATCH;
# endif
	n = size/sizeof(UINT);

	if(size%sizeof(UINT))
	{
		bOdd = TRUE;
		Buffer = new UINT[n+1];
		Buffer[n] = 0;
	}
	else
	{
		bOdd = FALSE;
		Buffer = new UINT[n];
	}

	ReadFile(hFile, Buffer, size, &nRead, NULL);
	CloseHandle(hFile);

	for(UINT i = 0; i < n; i++)
		aChkSum += (Buffer[i] << i%sizeof(UINT)) | (Buffer[i] >> (sizeof(UINT)*8-i%sizeof(UINT)));
	if(bOdd)
		aChkSum += Buffer[n];
	delete Buffer;

	if(aChkSum == xChkSum || aChkSum == 0xFFFFFFFF)
		return R_CHECKSUM_OK;
	else
		return R_CHECKSUM_MISMATCH;
}
