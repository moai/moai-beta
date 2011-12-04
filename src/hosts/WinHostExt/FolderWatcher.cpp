#include <windows.h>
#include <string.h>

#include <FolderWatcher.h>
#include <aku/AKU.h>

#define SIZE 64

struct FileInfo {
	FILETIME lastwrite;
	const char * fileName;
	bool shouldDelete;
};

unsigned int ptr = 0;
struct FileInfo * filesInfo[SIZE];
const char * directoryPath;
const char * directoryQueryString;
HANDLE notifyHandle;

//-------- Utility Functions -----------//

static bool isLuaFile(const char * fileName) {
	size_t size = strlen(fileName);
	return fileName[size-3] == 'l' &&
		   fileName[size-2] == 'u' &&
		   fileName[size-1] == 'a';
}

static size_t findLastOccuranceOfDirectorySeparator(const char * path) {
	for (size_t i = strlen(path) - 1; i > 0; i--) 
	{
		if (path[i] == '\\') {
			return i;
			break;
		}
	}
	return 0;
}

static void createDirectoryQueryString(const char * startupScript) {
	size_t dirPathSize = findLastOccuranceOfDirectorySeparator(startupScript);

	char * temp = (char *) calloc(dirPathSize+3,sizeof(char));
	strncpy(temp,startupScript,dirPathSize);
	temp[dirPathSize] = '\\';
	temp[dirPathSize+1] = '*';
	temp[dirPathSize+2] = '\0';
	directoryQueryString = (const char *) temp;

	temp = (char *) calloc(dirPathSize+1,sizeof(char));
	strncpy(temp,startupScript,dirPathSize);
	directoryPath = (const char *) temp;
}

static int findFilePosition(WIN32_FIND_DATA * fileData,int * pos) {
	int markFreeOne = -1;
	for(unsigned int i=0; i < SIZE; i++) {
		if (filesInfo[i] != NULL) {
			if (strcmp(filesInfo[i]->fileName,fileData->cFileName) == 0) {
				*pos = i;
				return false;
			}
		} else if(markFreeOne == -1) {
			markFreeOne = i;
		}
	}

	*pos = markFreeOne;
	assert(*pos != -1);
	return true;
}

static const char * fullPathOf(const char * fileName) {
	char * fullPath = (char *) calloc(strlen(directoryPath) + strlen(fileName) + 2,sizeof(char));
	size_t diretoryPathSize = strlen(directoryPath);
	strcpy(fullPath,directoryPath);
	fullPath[diretoryPathSize] = '\\';
	strcpy(fullPath + strlen(directoryPath) + 1,fileName);
	return (const char *)fullPath;
}
//------------------------------------//

static void deleteFileInfoOnPos(int pos) {
	free((void*)filesInfo[pos]->fileName);
	filesInfo[pos] = NULL;
}

static void markAllForDeletion() {
	for(unsigned int i=0; i < SIZE; i++) {
		if (filesInfo[i] != NULL)
			filesInfo[i]->shouldDelete = true;
	}
}

static void deleteAllMarked() {
	for(unsigned int i=0; i < SIZE; i++) {
		if (filesInfo[i] != NULL && filesInfo[i]->shouldDelete) 
			deleteFileInfoOnPos(i);
	}
}

static void reloadLuaFile(const char * fileName) {
	const char * fullPath = fullPathOf(fileName);
	AKURunScript(fullPath);
	free((void*)fullPath);
}

static void newFileInfoOnPos(WIN32_FIND_DATA * newLuaFile,int pos) {
	if (filesInfo[pos] == NULL) {
		filesInfo[pos] = (struct FileInfo *) malloc(sizeof(struct FileInfo));
		filesInfo[pos]->fileName = (const char *) calloc(sizeof(char),MAX_PATH);
	}

	filesInfo[pos]->shouldDelete = false;
	filesInfo[pos]->lastwrite = newLuaFile->ftLastWriteTime;
	strcpy((char *)filesInfo[pos]->fileName,newLuaFile->cFileName);
}

static void listProjectDirectory() {
	WIN32_FIND_DATA ffd;
	HANDLE fHandle;

	markAllForDeletion();

	fHandle = FindFirstFile(directoryQueryString, &ffd);
	if (INVALID_HANDLE_VALUE == fHandle) 
	{
		printf("Failed to list directory!\n");
	}
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//TODO:directory 
		}
		else if (isLuaFile(ffd.cFileName))
		{
			int pos;
			bool isNew = findFilePosition(&ffd,&pos);
			if (isNew) {
				newFileInfoOnPos(&ffd,pos);
			} 
			else if (CompareFileTime(&ffd.ftLastWriteTime,&filesInfo[pos]->lastwrite) == 1) {
				reloadLuaFile(filesInfo[pos]->fileName);
				filesInfo[pos]->shouldDelete = false;
				filesInfo[pos]->lastwrite = ffd.ftLastWriteTime;
			}
		}
	}
	while (FindNextFile(fHandle, &ffd) != 0);

	deleteAllMarked();
}


void winhostext_WatchFolder(const char* startupScript) {
	for(unsigned int i=0; i < SIZE; i++) {
		filesInfo[i] = NULL;
	}

	createDirectoryQueryString(startupScript);
	
	listProjectDirectory();

	notifyHandle = FindFirstChangeNotification(directoryPath,false,FILE_NOTIFY_CHANGE_LAST_WRITE);
}


static bool safeGuard = true;
void winhostext_Query() {
	if (notifyHandle == INVALID_HANDLE_VALUE)
		return;

	DWORD singleWaitStatus = WaitForSingleObject(notifyHandle,0);
	if (singleWaitStatus == WAIT_OBJECT_0) {
		if (FindNextChangeNotification(notifyHandle)) {
			safeGuard = !safeGuard;
			if (safeGuard) {
				listProjectDirectory();
			}
		}
	} else if (singleWaitStatus == WAIT_TIMEOUT) {
	} else {
		printf("\n ERROR: Unhandled dwWaitStatus.\n");
	}
}

void winhostext_CleanUp() {
	for (int i=0; i< SIZE; i++) {
		if (filesInfo[i] != NULL)
			deleteFileInfoOnPos(i);
	}
	
	FindCloseChangeNotification (notifyHandle);

	free((void*)directoryQueryString);
	free((void*)directoryPath);
}
