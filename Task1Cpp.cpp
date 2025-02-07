#include <iostream>
#include <cmath>

void inputABC(double& a, double& b, double& c) {
    using namespace std;
    cout << "Enter the coefficients a, b, c of the equation ax^2 + bx + c = 0" << endl;
    cin >> a >> b >> c;
}

double discriminant(double a, double b, double c) {
    return b * b - 4 * a * c;
}

void findRoots(double a, double b, double disc) {
    if (disc == 0) {
        double x = -b / (2 * a);
        using namespace std;
        cout << "The equation has one root x = " << x << endl;
    }  else if (disc > 0) {
        double x1 = (-b + sqrt(disc)) / (2 * a);
        double x2 = (-b - sqrt(disc)) / (2 * a);
        using namespace std;
        cout << "The equation has two roots x1 = " << x1 << ", x2 = " << x2 << endl;
    }  else {
        using namespace std;
        cout << "The equation has no roots" << endl;
    }
}

int main()
{
    double a, b, c;
    inputABC(a, b, c);

    if (a == 0) {
        if (b != 0) {
            double x = -c / b;
            using namespace std;
            cout << "The equation with the given coefficients is linear. X = " << x << endl;
        }
        else {
            using namespace std;
            cout << "The equation has no solution" << endl;
        }
    }

    double disc = discriminant(a, b, c);
    findRoots(a, b, disc);

    return 0;
}

