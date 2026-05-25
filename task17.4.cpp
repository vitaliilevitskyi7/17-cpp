#include <iostream>
#include <cmath>
#include <stdexcept>
#include <cassert>
#include <iomanip>

// === ВЛАСНИЙ КЛАС КОМПЛЕКСНОГО ЧИСЛА ===
class Complex {
private:
    double re; // Дійсна частина
    double im; // Уявна частина

public:
    Complex(double r = 0.0, double i = 0.0) : re(r), im(i) {}

    // Метод обчислення модуля комплексного числа |z|
    double magnitude() const {
        return std::sqrt(re * re + im * im);
    }

    // Арифметичні операції
    Complex operator+(const Complex& other) const {
        return Complex(re + other.re, im + other.im);
    }

    Complex operator-(const Complex& other) const {
        return Complex(re - other.re, im - other.im);
    }

    Complex operator*(const Complex& other) const {
        return Complex(re * other.re - im * other.im, re * other.im + im * other.re);
    }

    Complex operator/(double scalar) const {
        if (scalar == 0.0) {
            throw std::runtime_error("Помилка: Ділення комплексного числа на нуль!");
        }
        return Complex(re / scalar, im / scalar);
    }

    // Дружні оператори введення-виведення
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << std::fixed << std::setprecision(6) << c.re;
        if (c.im >= 0) os << " + " << c.im << "i";
        else os << " - " << std::abs(c.im) << "i";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Complex& c) {
        std::cout << " Введіть дійсну частину (Re): ";
        if (!(is >> c.re)) throw std::invalid_argument("Некоректне введення дійсної частини.");
        std::cout << " Введіть уявну частину (Im): ";
        if (!(is >> c.im)) throw std::invalid_argument("Некоректне введення уявної частини.");
        return is;
    }
};

// === ФУНКЦІЯ ОБЧИСЛЕННЯ СУМИ РЯДУ ТЕЙЛОРА ДЛЯ arctg(z) ===
Complex taylor_arctg(const Complex& z, double eps) {
    // 1. Твердження assert для перевірки ОДЗ ряду Тейлора (|z| < 1)
    double mod_z = z.magnitude();
    assert(mod_z < 1.0 && "Помилка assert: Ряд розбігається, оскільки |z| >= 1!");

    if (eps <= 0.0) {
        throw std::invalid_argument("Помилка: Точність епсилон має бути строго більшою за нуль.");
    }

    Complex sum(0.0, 0.0);
    Complex z_pow = z; // Починаємо з z^1
    Complex current_term = z;
    int n = 0;

    // Накопичуємо суму доданків, модуль яких не менший за eps
    while (current_term.magnitude() >= eps) {
        int sign = (n % 2 == 0) ? 1 : -1;
        
        // Поточний доданок: (-1)^n * z^(2n+1) / (2n + 1)
        current_term = (z_pow * sign) / (2.0 * n + 1.0);
        
        if (current_term.magnitude() < eps) {
            break;
        }

        sum = sum + current_term;
        
        // Готуємо степінь z для наступного кроку: множимо ще на z^2
        z_pow = z_pow * z * z;
        n++;
    }

    return sum;
}

int main() {
    try {
        std::cout << "--- РОБОТА З КОМПЛЕКСНИМИ ЧИСЛАМИ ТА ВИКЛЮЧЕННЯМИ ---\n";
        Complex z;
        double eps;

        // Введення даних з обробкою виключень
        std::cin >> z;
        std::cout << "Введіть точність обчислення рядів (eps >= 0): ";
        if (!(std::cin >> eps)) {
            throw std::invalid_argument("Некоректне введення точності епсилон.");
        }

        std::cout << "\nПеревірка ОДЗ: |z| = " << z.magnitude() << "\n";
        
        // Обчислення ряду Тейлора
        Complex result = taylor_arctg(z, eps);

        std::cout << "\n--- РЕЗУЛЬТАТ ОБЧИСЛЕННЯ ---\n";
        std::cout << "arctg(z) = " << result << "\n";

    } catch (const std::invalid_argument& e) {
        std::cerr << "Помилка введення даних: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Критична математична помилка: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Змістовне повідомлення про помилку: " << e.what() << std::endl;
    }

    return 0;
}
