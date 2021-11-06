//#include <Windows.h>
//#define PATH L"DLLCode.dll"
#include "Header.h"
#define PATHDLL L"DLLCode.dll"
#define PATHLIST L"Users.csv"
typedef U*(_cdecl* MyFunction)(LPWSTR, LPDWORD);//прототип импортируемой функции

//typedef int(_cdecl* MyFunction)(LPWSTR);
//typedef int(_cdecl* Summ)(int,int);
//typedef int(_cdecl* file)(char*);

//typedef struct FIO
//{
//	char surname[30]; // Фамилия
//	char name[15]; // Имя
//	char patronymic[30]; // Отчество
//	int age; // Возраст
//}DATA;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    HINSTANCE MyDLL;
    if (!(MyDLL = LoadLibrary(PATHDLL))) return 1;//открываем DLL
    MyFunction UserList = (MyFunction)GetProcAddress(MyDLL, "CreateUserArray");//инициализируем указатель на функцию
    DWORD count = 0;//переменная для количества записей
    U* users = UserList(PATH, &count);// получили массив записей (вызвали функцию из DLL)
    //проверка, что все правильно считалось в массив.
    DWORD w;
    HINSTANCE FileHandle = CreateFile(L"1.txt", GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    for (size_t i = 0; i < count; i++)
    {
        WriteFile(FileHandle, users[i].f, wcslen(users[i].f) * sizeof(WCHAR), &w, NULL);
    }
    HINSTANCE FileHandle1 = CreateFile(L"1.csv", GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    for (size_t i = 0; i < count; i++)
    {

        WriteFile(FileHandle1, users[i].f, wcslen(users[i].f) * sizeof(WCHAR), &w, NULL);
    }
   
    CloseHandle(FileHandle);
    CloseHandle(FileHandle1);
    return 0;
}