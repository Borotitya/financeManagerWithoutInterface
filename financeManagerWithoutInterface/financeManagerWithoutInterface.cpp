#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include<sstream>
#include<iomanip>

using namespace std;

class FinanceTracker { // Класс для учета доходов
private: // поля
	double income; // доход в месяц
	vector<pair<wstring, double>> transactions; //  список транзакции (категория, сумма) 
	vector<wstring> categories; // список категорий (категория, сумма)
public:
	FinanceTracker() : income(0) {} // конструктор по умолчанию (доход = 0)

	void setIncome(double newIncome) { // метод для установки дохода
		income = newIncome; //	устанавливаем новый доход
	}
	void addTransaction(const wstring& category, double amount) { // метод для добавления транзакции
		if ((category == L"Общие " || category == L"Одиночные") && amount > income) { // проверяем, что сумма транзакции не превышает доход
			MessageBoxW(NULL, L"Сумма транзакции не может превышать доход!", L"Уведомление", MB_OK | MB_ICONINFORMATION); // выводим сообщение об ошибке в случае превышения дохода
			return;
		}
		else if (category != L"Общие" && category != L"Одиночные" && calculateTotal() + amount > income) { // проверяем, что сумма транзакции не превышает доход
			MessageBoxW(NULL, L"Сумма транзакции не может превышать доход!", L"Уведомление", MB_OK | MB_ICONINFORMATION); // выводим сообщение об ошибке в случае превышения дохода
			return;
		}
		transactions.push_back(make_pair(category, amount)); // добавляем транзакцию в список 
	}
	double calculateTotal() { // метод для вычисления общей суммы транзакций
		double total = 0; // общая сумма транзакции
		for (const auto& transaction : transactions) { // перебираем все транзакции
			total += transaction.second; // добавляем сумму транзакции к общей сумме
		}
		return total; // возвращаем общую сумму транзакций
	}
	vector <pair<wstring, double>> getTransaction() const { // метод для получения списка транзакций 
		return transactions; // возвращаем список транзакций
	}
	double getIncome() const { // метод для получения дохода 
		return income; // возвращаем доход 
	}
	void addCategory(const wstring& category) { // метод для добавления категории 
		categories.push_back(category); // добавляем категорию в список 
	}
	vector <wstring> getCategories() const { // метод для получения списка категорий
		return categories; // возвращаем список категорий
	}
	void showNotification(const std::wstring& message) const { // метод для вывода уведомления 
		MessageBoxW(NULL, message.c_str(), L"Уведомление", MB_OK | MB_ICONINFORMATION); // выводим уведомление 
	}
};
void UpdateTotal(FinanceTracker* tracker, HWND hWnd) { // функция для обновления общей суммы транзакций 
	double total = tracker->calculateTotal(); // вычисляем общую сумму транзакций 

	wostringstream totalStream; // создаем поток для вывода общей суммы транзакций
	totalStream << L"Общая сумма потраченны средств: $" << fixed << setprecision(2) << total; // формируем строку с общей суммой транзакций

	HWND hStaticTotal = GetDlgItem(hWnd, 7); // получаем дескриптор элемента управления для вывода общей суммы транзакций

	SetWindowTextW(hStaticTotal, totalStream.str().c_str()); // устанавливаем текст элемента управления

	double income = tracker->getIncome(); // получаем доход

	wostringstream incomeStream; // создаем поток для вывода дохода
	incomeStream << L"Доход: $" << fixed << setprecision(2) << income; // формируем строку с доходом

	HWND hStaticIncome = GetDlgItem(hWnd, 9); // получаем дескриптор элемента управления для вывода дохода
	SetWindowTextW(hStaticIncome, incomeStream.str().c_str()); // устанавливаем текст элемента управления

	HWND hListTransactions = GetDlgItem(hWnd, 8); // получаем дескриптор элемента управления для вывода списка транзакций
	SendMessageW(hListTransactions,LB_RESETCONTENT, 0, 0); // очищаем список транзакций
	
	vector<pair<wstring, double>> transactions = tracker->getTransaction(); // получаем список транзакций
	for(const auto& transaction : transactions) { // перебираем все транзакции
			wostringstream transactionStream; // создаем поток для вывода транзакции
			transactionStream << L"Категория: " << transaction.first << L"Сумма: " << fixed << setprecision(2) << transaction.second; // формируем строку с транзакцией
			SendMessageW(hListTransactions, LB_ADDSTRING, 0, (LPARAM)transactionStream.str().c_str()); // добавляем транзакцию в список
		}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { // обработчик сообщений окна
    static FinanceTracker* tracker = new FinanceTracker(); // создаем объект трекера финансов

    switch (message) { // обработка сообщений
    case WM_CREATE: // создание окна
    {
        // Создание элементов управления
        CreateWindowW(L"STATIC", L"Доход:", WS_VISIBLE | WS_CHILD, 10, 10, 80, 30, hWnd, NULL, NULL, NULL); // создаем элемент управления STATIC для текста "Доход"
        CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 10, 100, 30, hWnd, (HMENU)1, NULL, NULL); // создаем элемент управления EDIT для ввода
        CreateWindowW(L"BUTTON", L"Установить доход", WS_VISIBLE | WS_CHILD, 210, 10, 150, 30, hWnd, (HMENU)2, NULL, NULL); // создаем элемент управления BUTTON для установки дохода
        CreateWindowW(L"STATIC", L"Категория:", WS_VISIBLE | WS_CHILD, 10, 50, 80, 30, hWnd, NULL, NULL, NULL); // создаем элемент управления STATIC для текста "Категория"
        HWND hCombo = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 100, 50, 150, 200, hWnd, (HMENU)3, NULL, NULL); // создаем элемент управления COMBOBOX для выбора категории
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Общие"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Одиночные"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Развлечения"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Еда"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Транспорт"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Одежда"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Жилье"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Здоровье"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Образование"); // добавляем строку в элемент управления COMBOBOX
        SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Прочее"); // добавляем строку в элемент управления COMBOBOX

        CreateWindowW(L"STATIC", L"Сумма:", WS_VISIBLE | WS_CHILD, 10, 90, 80, 30, hWnd, NULL, NULL, NULL); // создаем элемент управления STATIC для текста "Сумма"
        CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 90, 100, 30, hWnd, (HMENU)4, NULL, NULL); // создаем элемент управления EDIT для ввода
        CreateWindowW(L"BUTTON", L"Добавить транзакцию", WS_VISIBLE | WS_CHILD, 210, 90, 150, 30, hWnd, (HMENU)5, NULL, NULL); // создаем элемент управления BUTTON для добавления транзакции
        CreateWindowW(L"BUTTON", L"Обновить общую сумму", WS_VISIBLE | WS_CHILD, 10, 130, 150, 30, hWnd, (HMENU)6, NULL, NULL); // создаем элемент управления BUTTON для обновления общей суммы
        // Добавляем элемент управления STATIC для вывода общей суммы
        CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 10, 170, 350, 30, hWnd, (HMENU)7, NULL, NULL);
        // Добавляем элемент управления ListBox для отображения списка транзакций
        CreateWindowW(L"LISTBOX", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY, 10, 210, 380, 200, hWnd, (HMENU)8, NULL, NULL);
        // Добавляем элемент управления STATIC для вывода текущего дохода
        CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 10, 420, 350, 30, hWnd, (HMENU)9, NULL, NULL);
    }
    break;
    case WM_COMMAND: // обработка команд
    {
        int wmId = LOWORD(wParam); // получаем идентификатор команды 
        switch (wmId)
        {
        case 2: // Установить доход
        {
            // Получаем значение введенного дохода
            wchar_t buffer[256];
            GetDlgItemTextW(hWnd, 1, buffer, 256);
            double income = _wtof(buffer); // преобразуем строку в число

            // Устанавливаем доход в трекере финансов
            tracker->setIncome(income);

            MessageBoxW(hWnd, L"Доход успешно установлен!", L"Уведомление", MB_OK | MB_ICONINFORMATION); // показываем уведомление
        }
        break;
        case 5: // Добавить транзакцию
        {
            // Получаем выбранную категорию и сумму транзакции
            wchar_t categoryBuffer[256];
            GetDlgItemTextW(hWnd, 3, categoryBuffer, 256);
            std::wstring category = categoryBuffer; // получаем категорию

            wchar_t amountBuffer[256]; // получаем сумму транзакции
            GetDlgItemTextW(hWnd, 4, amountBuffer, 256); // получаем сумму транзакции
            double amount = _wtof(amountBuffer); // преобразуем строку в число

            // Добавляем транзакцию в трекер финансов
            tracker->addTransaction(category, amount);

            MessageBoxW(hWnd, L"Транзакция успешно добавлена!", L"Уведомление", MB_OK | MB_ICONINFORMATION); // показываем уведомление
        }
        break;
        case 6: // Обновить общую сумму
            UpdateTotal(tracker, hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam); // обработка сообщения по умолчанию
        }
    }
    break;
    case WM_DESTROY: // уничтожение окна 
        PostQuitMessage(0); // посылаем сообщение о завершении работы приложения
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam); // обработка сообщения по умолчанию
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { // точка входа в программу
    // Определение класса окна 
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX); // размер структуры
    wcex.style = CS_HREDRAW | CS_VREDRAW; // стиль окна
    wcex.lpfnWndProc = WndProc; // указатель на оконную процедуру
    wcex.cbClsExtra = 0; // количество дополнительных байтов, которые размещаются после структуры класса окна
    wcex.cbWndExtra = 0; // количество дополнительных байтов, которые размещаются после структуры окна
    wcex.hInstance = hInstance; // дескриптор экземпляра приложения
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION); // дескриптор значка
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); // дескриптор курсора
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // дескриптор кисти фона
    wcex.lpszMenuName = nullptr; // имя меню
    wcex.lpszClassName = L"FinanceTracker"; // имя класса окна 
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION); // дескриптор маленького значка

    // Регистрация класса окна
    if (!RegisterClassExW(&wcex)) {
        MessageBoxW(NULL, L"Не удалось зарегистрировать класс окна.", L"Ошибка", MB_OK | MB_ICONERROR); // показываем сообщение об ошибке
        return 1;
    }

    // Создание окна приложения
    HWND hWnd = CreateWindowW(L"FinanceTracker", L"Отслеживание финансов", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, nullptr, nullptr, hInstance, nullptr); // создаем окно приложения

    if (!hWnd) { // если окно не создано
        MessageBoxW(NULL, L"Не удалось создать окно приложения.", L"Ошибка", MB_OK | MB_ICONERROR); // показываем сообщение об ошибке
        return 1; // возвращаем код ошибки
    }

    // Отображение окна
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd); // обновляем окно

    // Цикл сообщений
    MSG msg; // структура сообщения
    while (GetMessageW(&msg, nullptr, 0, 0)) { // получаем сообщение
        TranslateMessage(&msg); // транслируем сообщение
        DispatchMessageW(&msg); // отправляем сообщение
    }

    return (int)msg.wParam; // возвращаем код выхода
}
