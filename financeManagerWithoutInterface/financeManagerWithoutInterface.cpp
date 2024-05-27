#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

class Transaction 
{
public:
    wstring category;
    double amount;

    Transaction(const wstring& category, double amount) : category(category), amount(amount) {}
};

class FinanceTracker 
{
private:
    double income;
    vector<Transaction> transactions;
    vector<wstring> categories;
    double budget;
    double savings;
    HWND main_window;
    HWND summary_window;
    HWND table_window;

public:
    FinanceTracker() : income(0), budget(0), savings(0), summary_window(NULL), table_window(NULL) {}

    void set_income(double new_income) {income = new_income;}

    void add_transaction(const wstring& category, double amount) 
    {
        if ((category == L"Общие" || category == L"Одиночные") && amount > income) 
        {
            show_notification(L"Сумма транзакции не может превышать доход!");
            return;
        }
        else if (category != L"Общие" && category != L"Одиночные" && calculate_total() + amount > income) 
        {
            show_notification(L"Сумма транзакции не может превышать доход!");
            return;
        }
        transactions.push_back(Transaction(category, amount));
    }

    double calculate_total() 
    {
        double total_spent = 0;
        for (const auto& transaction : transactions) 
        {
            total_spent += transaction.amount;
        }
        return total_spent;
    }

    vector<Transaction> get_transactions() {return transactions;}

    double get_income() {return income;}

    void add_category(const wstring& category) {categories.push_back(category);}

    vector<wstring> get_categories() {return categories;}

    void show_notification(const wstring& message) {MessageBoxW(NULL, message.c_str(), L"Уведомление", MB_OK | MB_ICONINFORMATION);}

    void set_budget(double new_budget) {budget = new_budget;}

    double get_budget() {return budget;}

    void set_savings(double new_savings) {savings = new_savings;}

    double get_savings() {return savings;}

    void create_main_window(HINSTANCE hInstance, int nCmdShow) 
    {
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = wnd_proc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(this);
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = L"FinanceTracker";
        wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

        if (!RegisterClassExW(&wcex)) 
        {
            MessageBoxW(NULL, L"Не удалось зарегистрировать класс окна.", L"Ошибка", MB_OK | MB_ICONERROR);
            return;
        }

        main_window = CreateWindowW(L"FinanceTracker", L"Отслеживание финансов", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, nullptr, nullptr, hInstance, this);

        if (!main_window) 
        {
            MessageBoxW(NULL, L"Не удалось создать окно приложения.", L"Ошибка", MB_OK | MB_ICONERROR);
            return;
        }

        ShowWindow(main_window, nCmdShow);
        UpdateWindow(main_window);
    }

    void show_summary_window(HWND hParent) 
    {
        if (summary_window) 
        {
            SetForegroundWindow(summary_window);
            return;
        }

        WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = FinanceTracker::summary_window_proc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(this);
        wcex.hInstance = GetModuleHandle(NULL);
        wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = L"SummaryWindowClass";
        wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

        if (!RegisterClassExW(&wcex)) 
        {
            MessageBoxW(NULL, L"Не удалось зарегистрировать класс окна.", L"Ошибка", MB_OK | MB_ICONERROR);
            return;
        }

        summary_window = CreateWindowW(L"SummaryWindowClass", L"Сводка транзакций", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 400, 250, hParent, NULL, GetModuleHandle(NULL), this);

        if (!summary_window) 
        {
            MessageBoxW(NULL, L"Не удалось создать окно сводки.", L"Ошибка", MB_OK | MB_ICONERROR);
            return;
        }

        ShowWindow(summary_window, SW_SHOW);
        UpdateWindow(summary_window);
    }

    void show_table_window(HWND hParent) 
    {
        if (table_window) 
        {
            SetForegroundWindow(table_window);
            return;
        }

        WNDCLASSEXW wcex = { sizeof(WNDCLASSEX) };
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = FinanceTracker::table_window_proc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = sizeof(this);
        wcex.hInstance = GetModuleHandle(NULL);
        wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = L"TableWindowClass";
        wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

        if (!RegisterClassExW(&wcex)) 
        {
            MessageBoxW(NULL, L"Не удалось зарегистрировать класс окна.", L"Ошибка", MB_OK | MB_ICONERROR);
            return;
        }

        table_window = CreateWindowW(L"TableWindowClass", L"Таблица транзакций", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 600, 400, hParent, NULL, GetModuleHandle(NULL), this);

        if (!table_window) 
        {
            MessageBoxW(NULL, L"Не удалось создать окно таблицы.", L"Ошибка", MB_OK | MB_ICONERROR);
            return;
        }

        ShowWindow(table_window, SW_SHOW);
        UpdateWindow(table_window);
    }

    void update_total(HWND hWnd) 
    {
        double total_spent = calculate_total();
        std::wostringstream total_spent_str;
        total_spent_str << L"Общая сумма потраченных средств: $" << std::fixed << std::setprecision(2) << total_spent;
        HWND hStaticTotalSpent = GetDlgItem(hWnd, 7);
        SetWindowTextW(hStaticTotalSpent, total_spent_str.str().c_str());

        double income = get_income();
        std::wostringstream income_str;
        income_str << L"Текущий доход: $" << std::fixed << std::setprecision(2) << income;
        HWND hStaticIncome = GetDlgItem(hWnd, 9);
        SetWindowTextW(hStaticIncome, income_str.str().c_str());

        HWND hList = GetDlgItem(hWnd, 8);
        SendMessageW(hList, LB_RESETCONTENT, 0, 0);
        const vector<Transaction>& transactions = get_transactions();
        for (const auto& transaction : transactions) {
            std::wostringstream transaction_str;
            transaction_str << L"Категория: " << transaction.category << L", Сумма: $" << std::fixed << std::setprecision(2) << transaction.amount;
            SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)transaction_str.str().c_str());
        }
    }

private:
    static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
    {
        FinanceTracker* tracker;
        if (message == WM_CREATE) 
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            tracker = (FinanceTracker*)pCreate->lpCreateParams;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)tracker);
        }
        else {tracker = (FinanceTracker*)GetWindowLongPtr(hWnd, GWLP_USERDATA);}

        switch (message) {
        case WM_CREATE: {
            CreateWindowW(L"STATIC", L"Доход:", WS_VISIBLE | WS_CHILD, 10, 10, 80, 30, hWnd, NULL, NULL, NULL);
            CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 10, 100, 30, hWnd, (HMENU)1, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Установить доход", WS_VISIBLE | WS_CHILD, 210, 10, 150, 30, hWnd, (HMENU)2, NULL, NULL);
            CreateWindowW(L"STATIC", L"Категория:", WS_VISIBLE | WS_CHILD, 10, 50, 80, 30, hWnd, NULL, NULL, NULL);
            HWND hCombo = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, 100, 50, 150, 200, hWnd, (HMENU)3, NULL, NULL);
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Общие");
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Одиночные");
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Развлечения");
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Еда");
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Транспорт");
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Одежда");
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Жилье");
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Здоровье");
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Образование");
            SendMessageW(hCombo, CB_ADDSTRING, 0, (LPARAM)L"Прочее");

            CreateWindowW(L"STATIC", L"Сумма:", WS_VISIBLE | WS_CHILD, 10, 90, 80, 30, hWnd, NULL, NULL, NULL);
            CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 90, 100, 30, hWnd, (HMENU)4, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Добавить транзакцию", WS_VISIBLE | WS_CHILD, 210, 90, 150, 30, hWnd, (HMENU)5, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Обновить общую сумму", WS_VISIBLE | WS_CHILD, 10, 130, 150, 30, hWnd, (HMENU)6, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Показать сводку", WS_VISIBLE | WS_CHILD, 170, 130, 150, 30, hWnd, (HMENU)10, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Показать таблицу", WS_VISIBLE | WS_CHILD, 330, 130, 150, 30, hWnd, (HMENU)11, NULL, NULL);
            CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 10, 170, 350, 30, hWnd, (HMENU)7, NULL, NULL);
            CreateWindowW(L"LISTBOX", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY, 10, 210, 380, 200, hWnd, (HMENU)8, NULL, NULL);
            CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 10, 420, 350, 30, hWnd, (HMENU)9, NULL, NULL);
        }
                      break;
        case WM_COMMAND: 
        {
            int wmId = LOWORD(wParam);
            switch (wmId) 
            {
            case 2: 
            {
                wchar_t buffer[256];
                GetDlgItemTextW(hWnd, 1, buffer, 256);
                double income = _wtof(buffer);
                tracker->set_income(income);
                MessageBoxW(hWnd, L"Доход успешно установлен!", L"Уведомление", MB_OK | MB_ICONINFORMATION);
            }
                  break;
            case 5: 
            {
                wchar_t categoryBuffer[256];
                GetDlgItemTextW(hWnd, 3, categoryBuffer, 256);
                std::wstring category = categoryBuffer;
                wchar_t amountBuffer[256];
                GetDlgItemTextW(hWnd, 4, amountBuffer, 256);
                double amount = _wtof(amountBuffer);
                tracker->add_transaction(category, amount);
                MessageBoxW(hWnd, L"Транзакция успешно добавлена!", L"Уведомление", MB_OK | MB_ICONINFORMATION);
            }
                  break;
            case 6:
                tracker->update_total(hWnd);
                break;
            case 10:
                tracker->show_summary_window(hWnd);
                break;
            case 11:
                tracker->show_table_window(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
                       break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    static LRESULT CALLBACK summary_window_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        FinanceTracker* tracker;
        if (message == WM_CREATE) 
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            tracker = (FinanceTracker*)pCreate->lpCreateParams;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)tracker);
        }
        else {tracker = (FinanceTracker*)GetWindowLongPtr(hWnd, GWLP_USERDATA);}

        switch (message) 
        {
        case WM_CREATE: 
        {
            CreateWindowW(L"STATIC", L"Сводка:", WS_VISIBLE | WS_CHILD, 10, 10, 80, 30, hWnd, NULL, NULL, NULL);
            CreateWindowW(L"STATIC", L"Общий доход:", WS_VISIBLE | WS_CHILD, 10, 50, 150, 30, hWnd, (HMENU)1, NULL, NULL);
            CreateWindowW(L"STATIC", L"Общие расходы:", WS_VISIBLE | WS_CHILD, 10, 90, 150, 30, hWnd, (HMENU)2, NULL, NULL);
            CreateWindowW(L"STATIC", L"Общие сбережения:", WS_VISIBLE | WS_CHILD, 10, 130, 150, 30, hWnd, (HMENU)3, NULL, NULL);

            double total_income = tracker->get_income();
            double total_spent = tracker->calculate_total();
            double total_savings = tracker->get_savings();

            wostringstream income_str;
            income_str << L"$" << fixed << setprecision(2) << total_income;
            SetWindowTextW(GetDlgItem(hWnd, 1), income_str.str().c_str());

            wostringstream spent_str;
            spent_str << L"$" << fixed << setprecision(2) << total_spent;
            SetWindowTextW(GetDlgItem(hWnd, 2), spent_str.str().c_str());

            wostringstream savings_str;
            savings_str << L"$" << fixed << setprecision(2) << total_savings;
            SetWindowTextW(GetDlgItem(hWnd, 3), savings_str.str().c_str());
        }
                      break;
        case WM_DESTROY:
            tracker->summary_window = NULL;
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }

    static LRESULT CALLBACK table_window_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        FinanceTracker* tracker;
        if (message == WM_CREATE) 
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            tracker = (FinanceTracker*)pCreate->lpCreateParams;
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)tracker);
        }
        else {tracker = (FinanceTracker*)GetWindowLongPtr(hWnd, GWLP_USERDATA);}

        switch (message) 
        {
        case WM_CREATE: 
        {
            CreateWindowW(L"LISTBOX", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY, 10, 10, 580, 350, hWnd, (HMENU)1, NULL, NULL);
            HWND hList = GetDlgItem(hWnd, 1);
            const vector<Transaction>& transactions = tracker->get_transactions();
            for (const auto& transaction : transactions) 
            {
                std::wostringstream transaction_str;
                transaction_str << L"Категория: " << transaction.category << L", Сумма: $" << std::fixed << std::setprecision(2) << transaction.amount;
                SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)transaction_str.str().c_str());
            }
        }
                      break;
        case WM_DESTROY:
            tracker->table_window = NULL;
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    FinanceTracker tracker;
    tracker.create_main_window(hInstance, nCmdShow);
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}
