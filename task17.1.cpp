#include <iostream>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <fstream>

class Rational {
private:
    int nominator;
    unsigned int denominator;

    void reduce() {
        if (nominator == 0) {
            denominator = 1;
            return;
        }
        unsigned int g = std::gcd(std::abs(nominator), denominator);
        nominator /= static_cast<int>(g);
        denominator /= g;
    }

public:
    Rational() : nominator(0), denominator(1) {}
    
    Rational(int nom, int denom) {
        if (denom == 0) {
            throw std::invalid_argument("Критична помилка: Знаменник не може бути нулем!");
        }
        if (denom < 0) {
            nominator = -nom;
            denominator = static_cast<unsigned int>(-denom);
        } else {
            nominator = nom;
            denominator = static_cast<unsigned int>(denom);
        }
        reduce();
    }

    // Перевантаження операторів віднімання
    Rational operator-(const Rational& other) const { // Бінарний
        int new_nom = nominator * other.denominator - other.nominator * denominator;
        int new_denom = denominator * other.denominator;
        return Rational(new_nom, new_denom);
    }

    Rational operator-() const { // Унарний мінус
        return Rational(-nominator, denominator);
    }

    Rational operator/(const Rational& other) const {
        if (other.nominator == 0) {
            throw std::runtime_error("Помилка: Ділення на нуль (чисельник дільника дорівнює 0)!");
        }
        int new_nom = nominator * other.denominator;
        int new_denom = denominator * other.nominator;
        return Rational(new_nom, new_denom);
    }

    // Дружні оператори введення/виведення
    friend std::ostream& operator<<(std::ostream& os, const Rational& r) {
        os << r.nominator << "/" << r.denominator;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Rational& r) {
        int nom, denom;
        is >> nom >> denom;
        if (denom == 0) {
            throw std::invalid_argument("Помилка потоку: Спроба ввести знаменник 0!");
        }
        if (denom < 0) {
            r.nominator = -nom;
            r.denominator = static_cast<unsigned int>(-denom);
        } else {
            r.nominator = nom;
            r.denominator = static_cast<unsigned int>(denom);
        }
        r.reduce();
        return is;
    }

    friend void save_to_file(const std::string& filename, const Rational& r);
};

void save_to_file(const std::string& filename, const Rational& r) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("Не вдалося відкрити або створити файл: " + filename);
    }
    out << r;
    out.close();
}

int main() {
    try {
        Rational r1(3, 4);
        Rational r2;
        std::cout << "Введіть чисельник та знаменник для r2 (через пробіл): ";
        std::cin >> r2;

        std::cout << "r1 - r2 = " << (r1 - r2) << "\n";
        std::cout << "Унарний мінус -r1 = " << (-r1) << "\n";
        std::cout << "r1 / r2 = " << (r1 / r2) << "\n";

        save_to_file("rational.txt", r1);
        std::cout << "Успішно збережено у файл.\n";

    } catch (const std::exception& e) {
        std::cerr << "Перехоплено виключення: " << e.what() << std::endl;
    }
    return 0;
}
