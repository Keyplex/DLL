#include <Windows.h>
#include "../DLL/Header.h"

//#include "../Первый/Задание на разработку DLL.cpp"

BOOL WINAPI DllMain(HINSTANCE nlnstDll, DWORD dwReason, LPVOID lpReserved)
{
	BOOL bAllWentWell = TRUE;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	if (bAllWentWell)
		return TRUE;
	else	
		return FALSE;	
}


LPWSTR Reading(LPWSTR path)//читает список их файла и возвращает строку
{
    DWORD d = 0;
    HINSTANCE FileHandle = CreateFile(PATH, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    LPWSTR ReadString = calloc(100500, sizeof(WCHAR));//выделяем память под строку
    ReadFile(FileHandle, ReadString, 100500 * sizeof(WCHAR), &d, NULL);//читаем из файла
    CloseHandle(FileHandle);
    ReadString[d / 2] = '\n';//это для того, что другой метод считает количество строк по символам '\n'
    ReadString[d / 2 + 1] = '\0';//закрыл строку
    return ReadString;

}

DWORD LpwstrToDword(LPWSTR str)//перевод из строки в число
{
    DWORD dw = 0;
    for (size_t i = 0; i < wcslen(str); i++)//по всем символам строки
    {
        dw += (str[i] - '0');//получаем цифру
        dw *= 10;//увеличиваем разряд числа на 1
    }
    return dw / 10;//делим на 10, т.к. лишний раз увеличили разряд
}
DWORD CountUsers(LPWSTR str)//функция считает количество записей в файле
{
    DWORD count = 0;
    for (size_t i = 0; i < wcslen(str); i++)//определяем количество строк в файле
    {
        if (str[i] == L'\n') count++;
    }
    return count;
}

__declspec(dllexport) U* CreateUserArray(LPWSTR path, LPDWORD n)//парсинг строки в массив структур
{
    LPWSTR str = Reading(path);//получаем строку из файла
    DWORD count = CountUsers(str);//определяем количество записей
    U* users = calloc(count, sizeof(U));//выделили память под массив структур
    DWORD poz = 0, zap = 0, ind = 0;//индексы для парсинга
    LPWSTR strvozr = calloc(3, sizeof(TCHAR));//строка для возраста (временная)
    for (size_t i = 0; i < wcslen(str); i++)
    {
        if (str[i] == '\n')//если новая строка
        {
            strvozr = calloc(3, sizeof(TCHAR));
            zap++;//переходим к следующему элементу массива
            poz = 0;
            ind = 0;
        }
        else
        {
            if (str[i] == ';')//если разделитель
            {
                poz++;//индекс поля структуры
                ind = 0;//индекс буквы в строке с ФИО
            }
            else
            {
                if (poz == 0) users[zap].f[ind] = str[i];
                if (poz == 1) users[zap].i[ind] = str[i];
                if (poz == 2) users[zap].o[ind] = str[i];
                if (poz == 3)
                {
                    if (str[i] == '\r')//символ перед возвратом строки '\r''\n'
                    {
                        strvozr[ind] = '\0';//закрываем строку с возрастом
                        users[zap].vozr = LpwstrToDword(strvozr);//преобразуем ее в число
                        free(strvozr);
                    }
                    else strvozr[ind] = str[i];//набираем строку, содержащую возраст
                }
                ind++;//переходим к следующей букве
            }
        }
    }
    *n = count;//возвращаем длину массива
    return users;//возвращаем указатель на массив
}

//extern "C" __declspec(dllimport) int MyFunc(LPWSTR str);
//int MyFunc(LPWSTR str)
//{
//	MessageBox(NULL, str, L"Добро пожаловать", MB_OK);
//	return 1;
//}

//extern "C" __declspec(dllimport) int Summ(int a, int b);
//int Summ(int a, int b)
//{
//	return a + b;
//}