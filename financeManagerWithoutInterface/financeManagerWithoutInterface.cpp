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
	void addTransaction(const wstring& category, double amount) {
		if ((category == L"Общие " || category == L"Одиночные") && amount > income) {
			MessageBoxW(NULL, L"Сумма транзакции не может превышать доход!", L"Уведомление", MB_OK | MB_ICONINFORMATION);
			return;
		}
		else if (category != L"Общие" && category != L"Одиночные" && calculateTotal() + amount > income) {
			MessageBoxW(NULL, L"Сумма транзакции не может превышать доход!", L"Уведомление", MB_OK | MB_ICONINFORMATION);
			return;
		}
		transactions.push_back(make_pair(category, amount));
	}
	double calculateTotal() {
		double total = 0;
		for (const auto& transaction : transactions) {
			total += transaction.second;
		}
		return total;
	}
	vector <pair<wstring, double>> getTransaction() const {
		return transactions;
	}
	double getIncome() const {
		return income;
	}
	void addCategory(const wstring& category) {
		categories.push_back(category);
	}
	vector <wstring> getCategories() const {
		return categories;
	}
	void showNotification(const std::wstring& message) const {
		MessageBoxW(NULL, message.c_str(), L"Уведомление", MB_OK | MB_ICONINFORMATION);
	}
};
void UpdateTotal(FinanceTracker* tracker, HWND hWnd) {
	double total = tracker->calculateTotal();

	wostringstream totalStream;
	totalStream << L"Общая сумма потраченны средств: $" << fixed << setprecision(2) << total;
}
