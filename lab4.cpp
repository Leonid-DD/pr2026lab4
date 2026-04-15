#include <iostream>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

struct CityNode {
    string cityName;     
    string regionName;    
    int population;      

    CityNode* next;       
    CityNode* prev;      

    CityNode(string city, string region, int pop) {
        cityName = city;
        regionName = region;
        population = pop;
        next = nullptr;
        prev = nullptr;
    }
};

struct RegionStat {
    string regionName;
    long long totalPopulation;
};

class DoublyLinkedList {
private:
    CityNode* head;   
    CityNode* tail;   

    int getValidatedPopulation() {
        int population;
        bool valid = false;

        do {
            cout << "Количество жителей: ";
            cin >> population;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Ошибка: Введите число!\n";
            }
            else if (population <= 0) {
                cout << "Ошибка: Население должно быть положительным числом!\n";
            }
            else {
                valid = true;
            }
        } while (!valid);

        cin.ignore();
        return population;
    }

    bool isValidString(const string& str) {
        for (char c : str) {
            if (!isspace(c)) {
                return true; 
            }
        }
        return false;
    }

public:
    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
    }

    ~DoublyLinkedList() {
        clear();
    }

    void append(string city, string region, int population) {
        CityNode* newNode = new CityNode(city, region, population);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void displayAllCities() {
        if (head == nullptr) {
            cout << "Список городов пуст.\n";
            return;
        }

        CityNode* current = head;
        cout << "\n--- Список всех городов ---\n";
        while (current != nullptr) {
            cout << "Город: " << current->cityName
                << ", Регион: " << current->regionName
                << ", Население: " << current->population << endl;
            current = current->next;
        }
        cout << "----------------------------\n";
    }

    void buildFromKeyboard() {
        string city, region;
        int population;

        cout << "\n=== Заполнение списка городов ===\n";
        cout << "Введите 0 в названии города для завершения ввода\n\n";

        while (true) {
            do {
                cout << "Название города: ";
                getline(cin, city);

                if (city == "0") {
                    break;
                }

                if (!isValidString(city)) {
                    cout << "Ошибка: Название города не может быть пустым!\n";
                }
            } while (city != "0" && !isValidString(city));

            if (city == "0") {
                break;
            }

            do {
                cout << "Название региона: ";
                getline(cin, region);

                if (!isValidString(region)) {
                    cout << "Ошибка: Название региона не может быть пустым!\n";
                }
            } while (!isValidString(region));

            population = getValidatedPopulation();

            append(city, region, population);
            cout << "Город добавлен!\n\n";
        }

        cout << "Заполнение списка завершено.\n";
    }

    void showRegionsByTotalPopulation() {
        if (head == nullptr) {
            cout << "Список городов пуст.\n";
            return;
        }

        RegionStat* stats = nullptr;
        int regionCount = 0;

        CityNode* current = head;

        // поиск уникальных регионов и сбор статистики
        while (current != nullptr) {
            bool regionFound = false;

            for (int i = 0; i < regionCount; i++) {
                if (stats[i].regionName == current->regionName) {
                    stats[i].totalPopulation += current->population;
                    regionFound = true;
                    break;
                }
            }

            if (!regionFound) {
                RegionStat* newStats = new RegionStat[regionCount + 1];

                for (int i = 0; i < regionCount; i++) {
                    newStats[i] = stats[i];
                }

                newStats[regionCount].regionName = current->regionName;
                newStats[regionCount].totalPopulation = current->population;

                delete[] stats;
                stats = newStats;
                regionCount++;
            }

            current = current->next;
        }

        // сортировка регионов по населению
        for (int i = 0; i < regionCount - 1; i++) {
            for (int j = 0; j < regionCount - i - 1; j++) {
                if (stats[j].totalPopulation < stats[j + 1].totalPopulation) {
                    RegionStat temp = stats[j];
                    stats[j] = stats[j + 1];
                    stats[j + 1] = temp;
                }
            }
        }

        // вывод
        cout << "\n=== Регионы по убыванию суммарного населения ===\n";
        for (int i = 0; i < regionCount; i++) {
            cout << "Регион: " << stats[i].regionName
                << ", Суммарное население: " << stats[i].totalPopulation << endl;
        }
        cout << "================================================\n";

        delete[] stats;
    }

    void deleteByRegion(string region) {
        if (head == nullptr) {
            cout << "Список городов пуст.\n";
            return;
        }

        CityNode* current = head;
        int deletedCount = 0;

        while (current != nullptr) {
            CityNode* nextNode = current->next;

            if (current->regionName == region) {
                if (current == head && current == tail) {
                    delete current;
                    head = nullptr;
                    tail = nullptr;
                }
                else if (current == head) {
                    head = current->next;
                    head->prev = nullptr;
                    delete current;
                }
                else if (current == tail) {
                    tail = current->prev;
                    tail->next = nullptr;
                    delete current;
                }
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    delete current;
                }
                deletedCount++;
            }
            current = nextNode;
        }

        if (deletedCount > 0) {
            cout << "Удалено " << deletedCount << " городов из региона '" << region << "'.\n";
        }
        else {
            cout << "Города из региона '" << region << "' не найдены.\n";
        }
    }

    void clear() {
        CityNode* current = head;
        while (current != nullptr) {
            CityNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        tail = nullptr;
        cout << "Список полностью очищен.\n";
    }

    bool isEmpty() {
        return head == nullptr;
    }
};

void showMenu() {
    cout << "\n========== МЕНЮ ==========\n";
    cout << "1. Вывести все города\n";
    cout << "2. Показать регионы по убыванию суммарного населения\n";
    cout << "3. Удалить все города указанного региона\n";
    cout << "4. Очистить весь список\n";
    cout << "0. Выйти из программы\n";
    cout << "==========================\n";
    cout << "Ваш выбор: ";
}

int main() {
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");

    DoublyLinkedList cityList;

    cout << "Программа для работы с линейным двусвязным списком\n";
    cout << "==================================================\n";

    cityList.buildFromKeyboard();

    int choice;
    string regionName;
    string cityName;
    int population;

    do {
        showMenu();
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
        case 1:
            cityList.displayAllCities();
            break;

        case 2:
            cityList.showRegionsByTotalPopulation();
            break;

        case 3:
            if (cityList.isEmpty()) {
                cout << "Список пуст. Сначала добавьте города.\n";
            }
            else {
                cout << "Введите название региона для удаления: ";
                getline(cin, regionName);
                cityList.deleteByRegion(regionName);
            }
            break;

        case 4:
            cityList.clear();
            break;

        case 0:
            cout << "Завершение программы\n";
            break;

        default:
            cout << "Неверный выбор. Пожалуйста, попробуйте снова.\n";
            break;
        }
    } while (choice != 0);

    return 0;
}