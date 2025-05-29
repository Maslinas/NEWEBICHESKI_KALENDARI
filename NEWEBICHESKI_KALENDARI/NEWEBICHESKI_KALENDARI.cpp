#include <vector>
#include <iomanip>
#include <iostream>

#include <sstream>
#include <string>

#include <algorithm>



#include <set>
#include <fstream>
#include <iostream>
#include <cstdlib> 
#include <ctime>   
#include <map>
using namespace std;

struct Task {
    int day;
    int month;
    string description;
};

bool compareTasks(const Task& a, const Task& b) {
    if (a.month != b.month) {
        return a.month < b.month;
    }
    return a.day < b.day;
}

vector<string> splitString(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
map<string, vector<Task>> loadCalendarFromFile(const string& filename) {
    map<string, vector<Task>> loadedCalendar;
    ifstream infile(filename);
    if (infile.is_open()) {
        time_t t = time(0);
        tm* now = localtime_s(&t);
        int day, month;
        string description;
        while (infile >> day >> month) {
            infile.ignore();
            getline(infile, description);
            Task loadedTask = { day, month, description };
            string monthYear = to_string(month) + "-" + to_string(now->tm_year + 1900);
            loadedCalendar[monthYear].push_back(loadedTask);
        }
        infile.close();
        cout << "Календарь загружен из файла " << filename << endl;
    }
    else {
        cout << "Не удалось открыть файл " << filename << " для загрузки." << endl;
    }
    return loadedCalendar;
}

void displayTasksForDate(const map<string, vector<Task>>& calendar, int day, int month, int year) {
    string dateStr = to_string(month) + "-" + to_string(year);
    cout << "\nЗадачи на " << day << "." << month << "." << year << ":\n";
    if (calendar.count(dateStr)) {
        for (const auto& task : calendar.at(dateStr)) {
            if (task.day == day) {
                cout << task.day << "." << task.month << ": " << task.description << endl;
            }
        }
    }
    else {
        cout << "Нет задач на эту дату." << endl;
    }
}
int main() {
    string da;
    map<string, vector<Task>> calendar;
    time_t t = time(0);
    tm* now = localtime(&t);
    int currentDay = now->tm_mday;
    int currentMonth = now->tm_mon + 1;
    cout << "Сегодня: " << currentDay << "." << currentMonth << endl;

    while (true) {
        cout << "Введите дату (день месяц) и задачу через пробел (или 'выход' для завершения): ";
        string input;
        getline(cin, input);

        if (input == "выход") {

            cout << "может быть вы желаете вспомнить что вы написали? если это так напишите да: ";
            cin >> da;
            if (da == "да")

            {
                ifstream infile("input.txt");
                if (infile.is_open()) {
                    string line;
                    while (getline(infile, line)) {
                        cout << line << endl;
                    }
                    infile.close();
                }
                else {
                    cout << "Не удалось открыть файл для чтения." << endl;
                }
            }
            else
                break;

        }

        vector<string> parts = splitString(input, ' ');
        if (parts.size() < 3) {
            cout << "Неверный формат ввода. Попробуйте еще раз." << endl;
            continue;
        }

        int day = stoi(parts[0]);
        int month = stoi(parts[1]);
        string description;
        for (size_t i = 2; i < parts.size(); ++i) {
            description += parts[i] + (i == parts.size() - 1 ? "" : " ");
        }

        Task newTask = { day, month, description };
        string monthYear = to_string(month) + "-" + to_string(now->tm_year + 1900);
        calendar[monthYear].push_back(newTask);
    }

    // Сортировка задач по дате
    for (auto& monthTasks : calendar) {
        sort(monthTasks.second.begin(), monthTasks.second.end(), compareTasks);
    }

    // Отображение задач на сегодня

    cout << "\nЗадачи на сегодня (" << currentDay << "." << currentMonth << "):\n";
    string todayStr = to_string(currentMonth) + "-" + to_string(now->tm_year + 1900);
    if (calendar.count(todayStr)) {
        for (const auto& task : calendar[todayStr]) {
            if (task.day == currentDay) {
                cout << task.day << "." << task.month << ": " << task.description << endl;
            }
        }
    }
    else {
        cout << "Нет задач на сегодня." << endl;
    }

    // Задачи на вчера
    int yesterdayDay, yesterdayMonth;
    if (currentDay == 1) {
        if (currentMonth == 1) {
            yesterdayMonth = 12;
            yesterdayDay = 31;
        }
        else {
            yesterdayMonth = currentMonth - 1;
            if (yesterdayMonth == 2) {
                yesterdayDay = 28;  // Simplification: Doesn't account for leap years
            }
            else if (yesterdayMonth == 4 || yesterdayMonth == 6 || yesterdayMonth == 9 || yesterdayMonth == 11) {
                yesterdayDay = 30;
            }
            else {
                yesterdayDay = 31;
            }
        }
    }
    else {
        yesterdayDay = currentDay - 1;
        yesterdayMonth = currentMonth;
    }

    cout << "\nЗадачи на вчера (" << yesterdayDay << "." << yesterdayMonth << "):\n";
    string yesterdayStr = to_string(yesterdayMonth) + "-" + to_string(now->tm_year + 1900);
    if (calendar.count(yesterdayStr)) {
        for (const auto& task : calendar[yesterdayStr]) {
            if (task.day == yesterdayDay) {
                cout << task.day << "." << task.month << ": " << task.description << endl;
            }
        }
    }
    else {
        cout << "Нет задач на вчера." << endl;
    }

    // Задачи на завтра
    int tomorrowDay, tomorrowMonth;
    int daysInMonth;
    if (currentMonth == 2) {
        daysInMonth = 28;  // Simplification: Doesn't account for leap years
    }
    else if (currentMonth == 4 || currentMonth == 6 || currentMonth == 9 || currentMonth == 11) {
        daysInMonth = 30;
    }
    else {
        daysInMonth = 31;
    }

    if (currentDay == daysInMonth) {
        if (currentMonth == 12) {
            tomorrowMonth = 1;
            tomorrowDay = 1;
        }
        else {
            tomorrowMonth = currentMonth + 1;
            tomorrowDay = 1;
        }
    }
    else {
        tomorrowDay = currentDay + 1;
        tomorrowMonth = currentMonth;
    }

    cout << "\nЗадачи на завтра (" << tomorrowDay << "." << tomorrowMonth << "):\n";
    string tomorrowStr = to_string(tomorrowMonth) + "-" + to_string(now->tm_year + 1900);
    if (calendar.count(tomorrowStr)) {
        for (const auto& task : calendar[tomorrowStr]) {
            if (task.day == tomorrowDay) {
                cout << task.day << "." << task.month << ": " << task.description << endl;
            }
        }
    }
    else {
        cout << "Нет задач на завтра." << endl;
    }
    // Сохранение календаря в файл
    ofstream outfile("input.txt");
    if (outfile.is_open()) {
        for (const auto& monthTasks : calendar) {
            for (const auto& task : monthTasks.second) {
                outfile << task.day << " " << task.month << " " << task.description << endl;
            }
        }
        outfile.close();
        cout << "Календарь сохранен в файл input.txt" << endl;
    }
    else {
        cout << "Не удалось открыть файл для сохранения." << endl;
    }

    // Загрузка календаря из файла
    calendar.clear();
    ifstream infile("input.txt");
    if (infile.is_open()) {
        int day, month;
        string description;
        while (infile >> day >> month) {
            infile.ignore();
            getline(infile, description);
            Task loadedTask = { day, month, description };
            string monthYear = to_string(month) + "-" + to_string(now->tm_year + 1900);
            calendar[monthYear].push_back(loadedTask);
        }
        infile.close();

    }
    else {
        cout << "Не удалось открыть файл для загрузки." << endl;
    }
    return 0;


}
