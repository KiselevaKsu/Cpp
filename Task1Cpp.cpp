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


int main()
{
    double a, b, c;
    inputABC(a, b, c);

    if (a == 0) {
        double x = -c / b;
        using namespace std;
        cout << "The equation with the given coefficients is not an equation of the form ax^2 + bx + c = 0. X = " << x << endl;
    }

    double disc = discriminant(a, b, c);

}

