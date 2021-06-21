#include "pch.h"
#include "FileInfo.h"

CFileInfo::CFileInfo()
{
}

CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(const CString & fileFullPath)
{
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	TCHAR szRelativePath[MAX_PATH]{};
	PathRelativePathTo(szRelativePath, szCurDir, FILE_ATTRIBUTE_DIRECTORY, fileFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);
	return CString(szRelativePath);
}
