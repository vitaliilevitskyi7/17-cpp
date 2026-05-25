#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// Базовий клас
class Person {
protected:
    std::string name;
    std::string gender;
    int age;

public:
    Person(std::string n = "", std::string g = "", int a = 18) : name(n), gender(g), age(a) {
        if (a < 0 || a > 120) throw std::invalid_argument("Некоректний вік особи!");
    }
    virtual ~Person() {}

    virtual void input() {
        std::cout << "ПІБ: "; std::cin >> name;
        std::cout << "Стать: "; std::cin >> gender;
        std::cout << "Вік: "; std::cin >> age;
        if (age < 0 || age > 120) throw std::invalid_argument("Помилка: вік поза межами!");
    }

    virtual void print() const {
        std::cout << "Ім'я: " << name << ", Стать: " << gender << ", Вік: " << age;
    }

    virtual std::string get_vuz() const { return "Не вказано"; }
};

// Віртуальний нащадок 1
class Student : virtual public Person {
protected:
    int course;
    std::string group;
    std::string vuz;

public:
    Student(std::string n="", std::string g_end="", int a=18, int c=1, std::string gr="", std::string v="")
        : Person(n, g_end, a), course(c), group(gr), vuz(v) {}

    void input() override {
        Person::input();
        std::cout << "Курс: "; std::cin >> course;
        std::cout << "Група: "; std::cin >> group;
        std::cout << "ВНЗ: "; std::cin >> vuz;
    }

    void print() const override {
        Person::print();
        std::cout << " | [Студент] Курс: " << course << ", Група: " << group << ", ВНЗ: " << vuz;
    }

    std::string get_vuz() const override { return vuz; }
};

// Віртуальний нащадок 2
class Teacher : virtual public Person {
protected:
    std::string vuz;
    std::string position;
    double salary;

public:
    Teacher(std::string n="", std::string g="", int a=25, std::string v="", std::string pos="", double sal=0)
        : Person(n, g, a), vuz(v), position(pos), salary(sal) {}

    void input() override {
        Person::input();
        std::cout << "ВНЗ роботи: "; std::cin >> vuz;
        std::cout << "Посада: "; std::cin >> position;
        std::cout << "ЗП: "; std::cin >> salary;
    }

    void print() const override {
        Person::print();
        std::cout << " | [Викладач] ВНЗ: " << vuz << ", Посада: " << position << ", ЗП: " << salary;
    }

    std::string get_vuz() const override { return vuz; }
};

// Множинне наслідування (Ромб)
class Aspirant : public Student, public Teacher {
public:
    Aspirant() : Person("", "", 22), Student(), Teacher() {}

    void input() override {
        Person::input(); // Викликається один раз завдяки virtual
        std::cout << "Курс аспірантури: "; std::cin >> course;
        std::cout << "Тема дослідження (замість групи): "; std::cin >> group;
        std::cout << "Посада (напівставка): "; std::cin >> position;
        std::cout << "ВНЗ аспірантури: "; std::cin >> Student::vuz;
        Teacher::vuz = Student::vuz; // Синхронізація члена ВНЗ
    }

    void print() const override {
        Person::print();
        std::cout << " | [Аспірант] Рік: " << course << ", Тема: " << group << ", ВНЗ: " << Student::vuz;
    }

    std::string get_vuz() const override { return Student::vuz; }
};

int main() {
    int size;
    std::cout << "Введіть розмір масиву людей: ";
    std::cin >> size;

    std::vector<Person*> room;

    for (int i = 0; i < size; ++i) {
        int type;
        std::cout << "\nОберіть тип для особи №" << i + 1 << " (1-Студент, 2-Викладач, 3-Аспірант): ";
        std::cin >> type;

        Person* p = nullptr;
        if (type == 1) p = new Student();
        else if (type == 2) p = new Teacher();
        else p = new Aspirant();

        // Цикл обробки виключень при некоректному введенні даних
        while (true) {
            try {
                p->input();
                break; // Успішно введено — виходимо з нескінченного циклу для цієї особи
            } catch (const std::exception& e) {
                std::cout << e.what() << " Повторіть введення спочатку для цього об'єкта:\n";
            }
        }
        room.push_back(p);
    }

    std::cout << "\n--- РЕЗУЛЬТАТИ ВВЕДЕННЯ ТА АНАЛІЗ ВНЗ ---\n";
    for (const auto& person : room) {
        person->print();
        std::cout << " -> ВНЗ об'єкта: " << person->get_vuz() << "\n";
    }

    // Очищення пам'яті
    for (auto& person : room) delete person;

    return 0;
}
