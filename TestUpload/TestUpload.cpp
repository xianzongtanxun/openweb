// TestUpload.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <WinInet.h>
#include <atlconv.h>
#include <Shlwapi.h>
//#include <winhttp.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"Shlwapi.lib")
//#pragma comment(lib,"winhttp.lib")

typedef struct _FileInfo
{
	DWORD nFileLen;
	DWORD nFileHighLen;
	LPSTR FileBuf;
}FileInfo, *pFileInfo;

void error(LPSTR lpszFunction)
{
	CHAR szBuf[80];
	DWORD dw = GetLastError();

	sprintf_s(szBuf, "%s failed: GetLastError returned %u\n",
		lpszFunction, dw);

	MessageBoxA(NULL, szBuf, "Error", MB_OK);
	ExitProcess(dw);
}
//--------------------------------------------------------

BOOL GetFileTime(HANDLE hFile, LPWSTR lpszCreationTime, LPWSTR lpszLastAccessTime, LPWSTR lpszLastWriteTime)
{
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC1, stLocal1, stUTC2, stLocal2, stUTC3, stLocal3;

	// -------->获取 FileTime
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
		error("GetFileTime()");
		return FALSE;
	}
	//---------> 转换: FileTime --> LocalTime
	FileTimeToSystemTime(&ftCreate, &stUTC1);
	FileTimeToSystemTime(&ftAccess, &stUTC2);
	FileTimeToSystemTime(&ftWrite, &stUTC3);

	SystemTimeToTzSpecificLocalTime(NULL, &stUTC1, &stLocal1);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC2, &stLocal2);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC3, &stLocal3);

	// ---------> Show the  date and time.
	wsprintf(lpszCreationTime, _T("创建时间:\t%02d/%02d/%d  %02d:%02d"),
		stLocal1.wDay, stLocal1.wMonth, stLocal1.wYear,
		stLocal1.wHour, stLocal1.wMinute);
	wsprintf(lpszLastAccessTime, _T("最后访问时间:\t%02d/%02d/%d  %02d:%02d"),
		stLocal2.wDay, stLocal2.wMonth, stLocal2.wYear,
		stLocal2.wHour, stLocal2.wMinute);
	wsprintf(lpszLastWriteTime, _T("最后修改时间:\t%02d/%02d/%d  %02d:%02d"),
		stLocal3.wDay, stLocal3.wMonth, stLocal3.wYear,
		stLocal3.wHour, stLocal3.wMinute);
	return TRUE;
}

std::string MakePreFileData(string &strBoundary, string &strFileName)
{
	USES_CONVERSION;
	//CDuiString strData;
	string strData;
	//strData += "--";
	strData += strBoundary;
	strData += "\r\n";
	strData += "Content-Disposition: form-data; name=\"file\"; filename=\"";
	//strData.Format(_T("%s\r\nContent-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n\r\n"), A2W(strBoundary.c_str()), A2W(strFileName.c_str()));
	//strData.Format(_T("%s\r\nContent-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n\r\n"), A2W(strBoundary.c_str()),A2W(strFileName.c_str()));
	//strData += strFileName.c_str());
	strData += strFileName;
	strData += "\"";
	strData += "\r\n";
	//strData += "Content-Type:video/mp4";//*************
	//strData += "Content-Type:image/jpeg";//*************
	strData += "Content-Type: application/octet-stream";//*************
	//strData += "\r\n";
	//strData += "Content-Transfer-Encoding:binary";//*************
	strData += "\r\n\r\n";
	return strData;
}

BOOL GetFileInfo(HANDLE &hFile, OUT FileInfo& binaryInfo, IN string FilePath)
{
	BOOL nRet = FALSE;
	DWORD nBytesRead;
	hFile = CreateFileA(FilePath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		return nRet;
	}

	binaryInfo.nFileLen = GetFileSize(hFile, &binaryInfo.nFileHighLen);
	binaryInfo.FileBuf = new char[binaryInfo.nFileLen];
	if (!binaryInfo.FileBuf)
	{
		CloseHandle(hFile);
		return nRet;
	}
	ZeroMemory(binaryInfo.FileBuf, binaryInfo.nFileLen);
	if (!ReadFile(hFile, binaryInfo.FileBuf, binaryInfo.nFileLen, &nBytesRead, NULL))
	{
		CloseHandle(hFile);
		return nRet;
	}
	CloseHandle(hFile);
	nRet = TRUE;

	return nRet;
}

//void cswuyg_winhttp_post()
//{
//	INTERNET_PORT nPort = 8080;
//	HINTERNET hSession = ::WinHttpOpen(L"cswuyg post test/1.0", WINHTTP_ACCESS_TYPE_NO_PROXY, NULL, NULL, NULL);
//	HINTERNET hConnect = ::WinHttpConnect(hSession, L"172.21.1.26", nPort, 0);
//	if (hConnect == NULL)
//	{
//		int i = ERROR_WINHTTP_INCORRECT_HANDLE_TYPE;
//		return;
//	}
//	const wchar_t* lpszAcceptedType[] = { L"*/*", NULL };
//	HINTERNET hRequest = ::WinHttpOpenRequest(hConnect, L"POST", L"/room/getPCMFile", L"HTTP/1.1", WINHTTP_NO_REFERER, lpszAcceptedType, 0);
//	if (hRequest == NULL)
//	{
//		return;
//	}
//	DWORD dwTime = 5000;
//	::WinHttpSetOption(hRequest, WINHTTP_OPTION_CONNECT_TIMEOUT, &dwTime, sizeof(DWORD));
//	std::wstring strHeader = L"Content-Type: multipart/form-data; boundary=--boundary_cswuygtest\r\n";
//	::WinHttpAddRequestHeaders(hRequest, strHeader.c_str(), strHeader.length(), WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE);
//
//	std::string strMIME = "----boundary_cswuygtest\r\n";
//	strMIME += "Content-Disposition: form-data; name=\"file\"; filename=\"C;\\upload_test_http_cswuyg.txt\"\r\n";
//	strMIME += "Content-Type:application/octet-stream\r\n\r\n";
//	strMIME += "1\r\n";
//	/////////////////////////////
//	//除了这个表单之外，后头还必须要有另外的表单信息，否则hfs服务端那边无法写入文件，这里仅为测试。
//	//strMIME += "----boundary_cswuygtest\r\n" ;
//	//strMIME += "Content-Disposition: form-data; name=\"name\"\r\n\r\n";
//	//strMIME += "temp.txt\r\n";
//	/////////////////////////
//	strMIME += "----boundary_cswuygtest--\r\n";
//	/////////////////////////
//	//可以直接写入
//	//::WinHttpSendRequest(hRequest, strHeader.c_str(), strHeader.length(), (LPVOID)strMIME.c_str(), strMIME.length(), strMIME.length(), 0);
//
//	//也可以后面分步写入
//	::WinHttpSendRequest(hRequest, NULL, 0, NULL, 0, strMIME.length(), 0);
//	DWORD dwWritten = 0;
//	while (!strMIME.empty())
//	{
//		::WinHttpWriteData(hRequest, strMIME.c_str(), strMIME.length(), &dwWritten);
//		if (strMIME.length() > dwWritten)
//		{
//			strMIME.substr(dwWritten);
//		}
//		else
//		{
//			strMIME = "";
//		}
//
//	}
//	::WinHttpReceiveResponse(hRequest, NULL);
//
//	char buf[1024] = { 0 };
//	DWORD dwToRead = 1024;
//	DWORD dwHaveRead = 0;
//	::WinHttpReadData(hRequest, buf, dwToRead, &dwHaveRead);
//	std::cout << buf << std::endl;
//	::WinHttpCloseHandle(hRequest);
//	::WinHttpCloseHandle(hConnect);
//	::WinHttpCloseHandle(hSession);
//}

void UploadFile(LPTSTR lpFilePath)
{
	HINTERNET hSession = 0;
	HINTERNET hConnect = 0;
	HINTERNET hRequest = 0;

	DWORD dwNumberOfBytesWritten = 0;
	DWORD dwBytesSend = 0;

	INTERNET_BUFFERS BufferIn;
	DWORD dwFlag;
	LPCTSTR boundary = TEXT("-----------------------------67491722032265"); //随机字符串
	LPCSTR aboundary = "-----------------------------67491722032265"; //ansi
	HANDLE hFile;
	hFile = CreateFile(lpFilePath,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	DWORD dwFileSize = GetFileSize(hFile, 0);

	TCHAR content_type[128];
	_stprintf_s(content_type, TEXT("Content-Type: multipart/form-data; boundary=%s"), boundary);
	//LPTSTR referer = TEXT("Referer: https://172.21.10.110:8999/room/getPCMFile");//http://172.21.1.26:8080/room/getPCMFile
	LPTSTR accept = TEXT("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	LPTSTR accept_lan = TEXT("Accept-Language: zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3");
	LPTSTR accept_encoding = TEXT("Accept-Encoding: gzip, deflate");
	LPTSTR accept_token = TEXT("x-auth-token: 682c6b7c-6cc8-4ff1-a78f-e4cf820c801c");//682c6b7c-6cc8-4ff1-a78f-e4cf820c801c   9b287ec7-2e36-4900-a3ec-35e946fada4e
	LPTSTR user_agent = TEXT("User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:22.0) Gecko/20100101 Firefox/22.0");

	hSession = InternetOpen(_T("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:22.0) Gecko/20100101 Firefox/22.0"),
		INTERNET_OPEN_TYPE_PRECONFIG,
		0,
		INTERNET_INVALID_PORT_NUMBER,
		0);
	if (0 == hSession){
		return;
	}

	hConnect = InternetConnect(hSession,
		TEXT("172.21.10.110"),
		8999,
		_T(""),
		_T(""),
		INTERNET_SERVICE_HTTP,
		0,
		0);
	if (0 == hConnect)
	{

		InternetCloseHandle(hSession);
		return;
	}
	dwFlag = INTERNET_FLAG_KEEP_CONNECTION;

	hRequest = HttpOpenRequest(hConnect,
		_T("POST"),
		_T("/room/getPCMFile"),
		HTTP_VERSION,
		0,                //Referrer
		0,                //AcceptTypes 
		dwFlag,
		0);
	if (0 == hRequest){
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return;
	}
	HttpAddRequestHeaders(hRequest, content_type, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	//HttpAddRequestHeaders(hRequest, referer, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	HttpAddRequestHeaders(hRequest, accept, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	HttpAddRequestHeaders(hRequest, accept_lan, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	HttpAddRequestHeaders(hRequest, accept_encoding, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
	HttpAddRequestHeaders(hRequest, accept_token, -1, HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

	BYTE* lpBuffer = (BYTE*)VirtualAlloc(0, dwFileSize, MEM_COMMIT, PAGE_READWRITE);
	if (0 == lpBuffer)
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return;
	}

	DWORD dwRead;
	ReadFile(hFile, lpBuffer, dwFileSize, &dwRead, 0);
	CloseHandle(hFile);
	char first_boundary[128];
	char delimiter[128];
	char end_boundary[128];
	sprintf_s(first_boundary, "--%s\r\n", aboundary);
	sprintf_s(delimiter, "\r\n--%s\r\n", aboundary);
	sprintf_s(end_boundary, "\r\n--%s--\r\n", aboundary);
	LPSTR content_dispos = "Content-Disposition: form-data; name=\"file\"; filename=\"foobarcapture.pcm\"\r\n";
	LPSTR content_type2 = "Content-Type: application/octet-stream\r\n\r\n";
	LPSTR rn = "\r\n";
	BufferIn.dwStructSize = sizeof(INTERNET_BUFFERS);
	BufferIn.Next = NULL;
	BufferIn.lpcszHeader = NULL;
	BufferIn.dwHeadersLength = 0;
	BufferIn.dwHeadersTotal = 0;
	BufferIn.lpvBuffer = NULL;
	BufferIn.dwBufferLength = 0;
	BufferIn.dwBufferTotal = dwFileSize
		+ strlen(first_boundary)
		+ strlen(content_dispos)
		+ strlen(content_type2)
		+ strlen(end_boundary); //Content-Length:
	BufferIn.dwOffsetLow = 0;
	BufferIn.dwOffsetHigh = 0;

	if (!HttpSendRequestEx(hRequest, &BufferIn, 0, 0, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hSession);
		return;
	}

	InternetWriteFile(hRequest, (byte*)first_boundary, strlen(first_boundary), &dwNumberOfBytesWritten); //first boundary
	InternetWriteFile(hRequest, (byte*)content_dispos, strlen(content_dispos), &dwNumberOfBytesWritten);
	InternetWriteFile(hRequest, (byte*)content_type2, strlen(content_type2), &dwNumberOfBytesWritten);
	InternetWriteFile(hRequest, lpBuffer, dwFileSize, &dwNumberOfBytesWritten);

	//如果还有其他文件
	//InternetWriteFile(hRequest,(byte*)delimiter,strlen(delimiter),&dwNumberOfBytesWritten); //deimiter
	//InternetWriteFile(hRequest,(byte*)content_dispos,strlen(content_dispos),&dwNumberOfBytesWritten);
	//InternetWriteFile(hRequest,(byte*)content_type2,strlen(content_type2),&dwNumberOfBytesWritten);
	//...
	InternetWriteFile(hRequest, (byte*)end_boundary, strlen(end_boundary), &dwNumberOfBytesWritten);//last boundary
	HttpEndRequest(hRequest, 0, 0, 0);
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hSession);

	VirtualFree(lpBuffer, 0, MEM_RELEASE);
}

int main()
{
	HANDLE hFile1;
	TCHAR ObjectName1[] = _T("C:\\foobarcapture.pcm");
	TCHAR szCreationTime[30], szLastAccessTime[30], szLastWriteTime[30];
	hFile1 = CreateFile(ObjectName1, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	GetFileTime(hFile1, szCreationTime, szLastAccessTime, szLastWriteTime);
	if (hFile1 == INVALID_HANDLE_VALUE) {
		error("GetLastWriteTime()");
		return 0;
	}
	TCHAR sOut;
	//sprintf_s(sOut, "%s\n%s\n%s\n", szCreationTime, szLastAccessTime, szLastWriteTime);
	wprintf(_T("%s\n%s\n%s\n"), szCreationTime, szLastAccessTime, szLastWriteTime);
	CloseHandle(hFile1);
	system("pause");
	return 0;
	//cswuyg_winhttp_post();
	//string ServerName = "http://172.21.1.26";
	//string ObjectName = "/room/getPCMFile";//服务器文件对象
	//string HTTPBoundary = "----WebKitFormBoundaryTFbBBLWvSkp9pYIy";//边界值
	INTERNET_PORT nPort = 8080;// 8999;
	char ServerName[] = "172.21.1.24";// "172.21.10.110";
	char ObjectName[] = "C:\\foobarcapture.pcm";
	USES_CONVERSION;
	//UploadFile(A2W(ObjectName));
	//char ObjectPath[] = "/Users/wuer/temp";
	char ObjectPath[] = "/room/getPCMFile";
	//char ObjectName[] = "C:\\testupload.txt";
	if (::PathFileExistsA(ObjectName)){
		int k = 0;
	}
	char Headers[] = "------WebKitFormBoundaryWAMNqONueDYTbH4m";
	//LPVOID Data = "C:\\201812191417.log";
	INTERNET_BUFFERSA BufferIn = { 0 };
	BOOL Red, Wrt;
	LPCVOID lpBuffer[1024];
	char pBuffer[] = "------WebKitFormBoundaryWAMNqONueDYTbH4m\r\n";
	char qBuffer[] = "------WebKitFormBoundaryWAMNqONueDYTbH4m--\r\n";
	DWORD dwBytesRead;
	DWORD dwBytesWrite;
	DWORD BytesWrite;

	HINTERNET hSession, hConnection, hRequest;
	BufferIn.dwStructSize = sizeof(INTERNET_BUFFERSA);
	HANDLE hFile;
	FileInfo localFileInfo;
	if (!GetFileInfo(hFile, localFileInfo, ObjectName)) {
		return FALSE;
	}
	hSession = InternetOpenA("Post_2", /*INTERNET_OPEN_TYPE_PRECONFIG*/INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hSession)
	{
		printf("InternetOpen failed!%lu\n", GetLastError());
		InternetCloseHandle(hSession);
		exit(0);
	}
	hConnection = InternetConnectA(hSession, ServerName, nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (!hConnection)
	{
		printf("InternetConnect failes!%lu\n", GetLastError());
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hSession);
		exit(0);
	}
	DWORD_PTR drRet = NULL;
	hRequest = HttpOpenRequestA(hConnection, "POST", ObjectPath, "HTTP/1.1", NULL, NULL, /*INTERNET_FLAG_CACHE_IF_NET_FAIL*/INTERNET_FLAG_RELOAD, drRet);
	if (!hRequest)
	{
		printf("HttpOpenRequest failed!%lu\n", GetLastError());
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hSession);
		exit(0);
	}
	string strData;
	strData += "Content-Type:multipart/form-data; boundary=";//二进制文件传送Content-Type类型为：multipart/form-data  x-auth-token:9b287ec7-2e36-4900-a3ec-35e946fada4e; 
	strData += Headers;
	strData += "\r\n\r\n";
	if (!HttpAddRequestHeadersA(hRequest, strData.c_str(), strData.length(), HTTP_ADDREQ_FLAG_ADD/* | HTTP_ADDREQ_FLAG_REPLACE*/))
	{
		printf("Failed!%lu\n", GetLastError());
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hSession);
		exit(0);
	}
	/*HANDLE hFile = CreateFileA(ObjectName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Filed to open file!\n");
		CloseHandle(hFile);
		exit(0);
	}*/
	//USES_CONVERSION;
	//char strDataPre[1024] = {0};
	//USES_CONVERSION;
	//CDuiString strData;
	string strDataPre;
	string strFileName = "foobarcapture.pcm";
	string boundary = Headers;
	strDataPre = MakePreFileData(boundary, strFileName);
	//strDataPre += "--";
	////strData += A2W(strBoundary.c_str());
	//strDataPre += Headers;
	//strDataPre += "\r\n";
	//strDataPre += "Content-Disposition:form-data; name=\"file\"; filename=\"";
	////strData.Format(_T("%s\r\nContent-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n\r\n"), A2W(strBoundary.c_str()), A2W(strFileName.c_str()));
	////strData += strFileName.c_str());
	//strDataPre += "foobarcapture.pcm";
	//strDataPre += "\"";
	//strDataPre += "\r\n";
	////strData += "Content-Type:text/plain";//*************
	//strDataPre += "Content-Type:application/octet-stream";//*************
	//strDataPre += "\r\n\r\n";

	BufferIn.dwBufferTotal = localFileInfo.nFileLen + sizeof(strDataPre) + sizeof(qBuffer);
	printf("File size is %d.\n", BufferIn.dwBufferTotal);
	
	//BufferIn.dwBufferTotal = strDataPre.length();
	if (!HttpSendRequestExA(hRequest, &BufferIn, NULL, HSR_SYNC | HSR_INITIATE, 1))
	{
		printf("Error on HttpSend!%lu\n", GetLastError());
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hSession);
		exit(0);
	}
	/*InternetWriteFile(hRequest, pBuffer, sizeof(pBuffer), &BytesWrite);
	DWORD sum = 0;
	do
	{
		if (!(Red = ReadFile(hFile, lpBuffer, sizeof(lpBuffer), &dwBytesRead, NULL)))
		{
			printf("Failed to read!%lu\n", GetLastError());
			CloseHandle(hFile);
			break;
		}
		if (!(Wrt = InternetWriteFile(hRequest, lpBuffer, dwBytesRead, &dwBytesWrite)))
		{
			printf("Failed to write!%lu\n", GetLastError());
			InternetCloseHandle(hRequest);
			break;
		}
		sum += dwBytesWrite;
	} while (dwBytesRead == sizeof(lpBuffer));
	InternetWriteFile(hRequest, qBuffer, sizeof(qBuffer), &BytesWrite);
	CloseHandle(hFile);*/

	//printf("Actually written is %d.\nUpload file %s is OK!\n", sum, ObjectName);
	//6 _A_send http头
	DWORD dwNumberOfBytesWritten;
	//if ((!InternetWriteFile(hRequest, pBuffer, strlen(pBuffer), &dwNumberOfBytesWritten)))
	//{
	//	//goto END;
	//}
	//if (dwNumberOfBytesWritten != strlen(pBuffer))
	//{
	//	//goto END;
	//}
	//////6 _B_send filedata
	if (!InternetWriteFile(hRequest, localFileInfo.FileBuf, localFileInfo.nFileLen, &dwNumberOfBytesWritten))
	{
		//goto END;
		int k = 0;
	}
	/*if (dwNumberOfBytesWritten != localFileInfo.length())
	{
		goto END;
	}*/
	////6 _C_send Http尾
	if (!InternetWriteFile(hRequest, qBuffer, sizeof(qBuffer), &dwNumberOfBytesWritten))
	{
		//goto END;
		int k = 0;
	}
	if (dwNumberOfBytesWritten != sizeof(qBuffer))
	{
		//goto END;
		int k = 0;
	}
	if (!HttpEndRequestA(hRequest, NULL, HSR_SYNC, 1))
	{
		printf("Failed to end Request!%lu\n", GetLastError());
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hSession);
		exit(0);
	}
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnection);
	InternetCloseHandle(hSession);
    return 0;
}