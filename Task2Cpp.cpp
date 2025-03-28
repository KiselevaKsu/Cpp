#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;

class QuadraticEquation {
public:
    double a, b, c;

    QuadraticEquation(double a = 0, double b = 0, double c = 0) : a(a), b(b), c(c) {}

    string toString() const {
        return to_string(a) + "x² + " + to_string(b) + "x + " + to_string(c) + " = 0";
    }
};

class Student {
public:
    string lastName;
    string firstName;
    string middleName;
    string type; // "good", "average", "bad"

    Student(const string& last, const string& first, const string& middle, const string& type)
        : lastName(last), firstName(first), middleName(middle), type(type) {}

    string getFullName() const {
        return lastName + " " + firstName + " " + middleName;
    }

    vector<double> solveEquation(const QuadraticEquation& eq) const {
        if (type == "good") {
            return solveCorrectly(eq);
        }
        else if (type == "average") {
            return solveAverage(eq);
        }
        else {
            return { 0.0 }; // плохой студент всегда пишет 0
        }
    }

private:
    vector<double> solveCorrectly(const QuadraticEquation& eq) const {
        double a = eq.a, b = eq.b, c = eq.c;
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return {};
        }
        else if (discriminant == 0) {
            double x = -b / (2 * a);
            return { x };
        }
        else {
            double x1 = (-b + sqrt(discriminant)) / (2 * a);
            double x2 = (-b - sqrt(discriminant)) / (2 * a);
            return { x1, x2 };
        }
    }

    vector<double> solveAverage(const QuadraticEquation& eq) const {
        // 70% решает правильно
        if (rand() % 100 < 70) {
            return solveCorrectly(eq);
        }
        else {
            // случайный неправильный ответ
            int choice = rand() % 5;
            switch (choice) {
            case 0: return {};
            case 1: return { 0.0 };
            case 2: return { 1.0 };
            case 3: return { -1.0 };
            case 4: return { static_cast<double>(rand() % 20 - 10) };
            default: return { 0.0 };
            }
        }
    }
};

class Teacher {
public:
    struct Mail {
        QuadraticEquation equation;
        vector<double> solution;
        string studentName;
    };

    vector<Mail> mailbox;
    map<string, int> results;

    void receiveMail(const QuadraticEquation& eq, const vector<double>& sol, const string& name) {
        mailbox.push_back({ eq, sol, name });
    }

    void checkMails() {

        for (const auto& mail : mailbox) {
            vector<double> correctSolution = solveCorrectly(mail.equation);
            if (compareSolutions(mail.solution, correctSolution)) {
                results[mail.studentName]++;
            }
        }
        mailbox.clear();
    }

    void publishResults(const string& filename, const vector<Student>& students) {
        ofstream out(filename);
        out << "Таблица успеваемости:\n";
        out << "----------------------------------------\n";
        out << "| ФИО студента            | Решено верно |\n";
        out << "----------------------------------------\n";

        for (const auto& student : students) {
            string name = student.getFullName();
            out << "| " << left << setw(23) << name
                << " | " << setw(12) << results[name] << " |\n";
        }
        out << "----------------------------------------\n";
        out.close();
    }

private:
    vector<double> solveCorrectly(const QuadraticEquation& eq) const {
        double a = eq.a, b = eq.b, c = eq.c;
        double discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return {};
        }
        else if (discriminant == 0) {
            double x = -b / (2 * a);
            return { x };
        }
        else {
            double x1 = (-b + sqrt(discriminant)) / (2 * a);
            double x2 = (-b - sqrt(discriminant)) / (2 * a);

            if (x1 > x2) return { x2, x1 };
            return { x1, x2 };
        }
    }

    bool compareSolutions(const vector<double>& s1, const vector<double>& s2) const {
        if (s1.size() != s2.size()) return false;

        for (size_t i = 0; i < s1.size(); ++i) {
            if (fabs(s1[i] - s2[i]) > 1e-6) {
                return false;
            }
        }
        return true;
    }
};

vector<QuadraticEquation> readEquations(const string& filename) {
    ifstream in(filename);
    vector<QuadraticEquation> equations;

    double a, b, c;
    while (in >> a >> b >> c) {
        equations.emplace_back(a, b, c);
    }

    in.close();
    return equations;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); 

    vector<QuadraticEquation> equations = readEquations("equations.txt");

    vector<Student> students = {
        Student("Иванов", "Иван", "Иванович", "good"),
        Student("Петров", "Петр", "Петрович", "average"),
        Student("Сидоров", "Сидор", "Сидорович", "bad"),
        Student("Смирнова", "Анна", "Владимировна", "good"),
        Student("Кузнецов", "Дмитрий", "Сергеевич", "average"),
        Student("Васильев", "Алексей", "Николаевич", "bad"),
        Student("Николаева", "Елена", "Дмитриевна", "good"),
        Student("Федоров", "Михаил", "Александрович", "average")
    };

    Teacher teacher;

    for (const auto& eq : equations) {
        for (const auto& student : students) {
            vector<double> solution = student.solveEquation(eq);
            teacher.receiveMail(eq, solution, student.getFullName());
        }
    }

    teacher.checkMails();

    teacher.publishResults("results.txt", students);

    cout << "Results are in results.txt" << endl;

    return 0;
}