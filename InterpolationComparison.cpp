#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;

// Функция для вычисления интерполяционного многочлена Лагранжа
void LagrangeInterpolation(vector<double> x, vector<double> y, double point)
{
    double y_k = 0.;
    double P_n = 1.;
    double y_k_Div_diff_P = 0.;
    int n = x.size();
    double L_n = 0.;

    auto startTime = chrono::steady_clock::now();

    for (int i = 0; i < n; ++i) {
        y_k = y[i];
        double diff_P = 1.;
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                P_n *= (point - x[j]);
                diff_P *= (point - x[j]);
            }
            else {
                diff_P *= (x[i] - x[j]);
            }
        }
        y_k_Div_diff_P += (y_k / diff_P);
    }

    L_n = P_n * y_k_Div_diff_P;

    auto endTime = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

    cout << fixed << setprecision(6) << "Интерполяционный многочлен Лагранжа для x = " << point << " равен " << L_n << endl;
    cout << "Время выполнения - " << duration.count() << " мкс" << endl << "\n";
}


//Функция для вычисления разделенных разностей прямой интерполяции
double DividedDifferencesForward(int i, int j, vector<double> x, vector<double> y)
{
    if (i == j) {
        return y[i];
    }
    else {
        return (DividedDifferencesForward(i + 1, j, x, y) - DividedDifferencesForward(i, j - 1, x, y)) / (x[j] - x[i]);
    }
}


//Функция для вычисления разделеных разностей обратной интерполяции
double DividedDifferencesBackward(int i, int j, vector<double> x, vector<double> y)
{
    if (i == j) {
        return y[i];
    }
    else {
        return (DividedDifferencesBackward(i, j - 1, x, y) - DividedDifferencesBackward(i + 1, j, x, y)) / (x[i] - x[j]);
    }
}


//Функция для вычисления интерполяционного многочлена Ньютона
void NewtonInterpolation(vector<double> x, vector<double> y, double point)
{
    double resultForward = 0.;
    double resultBackward = 0.;
    int n = x.size();
    //Прямая интерполяция Ньютона
    auto startTimeForward = chrono::steady_clock::now();

    for (int i = 0; i < n; ++i) {
        double t = DividedDifferencesForward(0, i, x, y);
        for (int j = 0; j < i; ++j) {
            t *= (point - x[j]);
        }
        resultForward += t;
    }

    auto endTimeForward = chrono::steady_clock::now();
    auto durationForward = chrono::duration_cast<chrono::microseconds>(endTimeForward - startTimeForward);
    //Обратная интерполяция Ньютона
    auto startTimeBackward = chrono::steady_clock::now();

    for (int i = n - 1; i >= 0; --i) {
        double t = DividedDifferencesBackward(0, i, x, y);
        for (int j = 0; j < i; ++j) {
            t *= (point - x[j]);
        }
        resultBackward += t;
    }

    auto endTimeBackward = chrono::steady_clock::now();
    auto durationBackward = chrono::duration_cast<chrono::microseconds>(endTimeBackward - startTimeBackward);

    cout << fixed << setprecision(6) << "Интерполяционный многочлен Ньютона \n при интерполяции вперед для x = " << point << " равен " << resultForward << endl;
    cout << "Время выполнения - " << durationForward.count() << " мкс" << endl << "\n";
    
    cout << fixed << setprecision(6) << "Интерполяционный многочлен Ньютона \n при интерполяции назад для x = " << point << " равен " << resultBackward << endl;
    cout << "Время выполнения - " << durationBackward.count() << " мкс" << endl << "\n";
}


int main()
{
    setlocale(LC_ALL, "Russian");
    // Пример данных входных значений
    vector<double> x = {0.64, 1.29, 2.59, 2.96, 3.93, 4.27, 4.38, 4.8, 6.33};
    vector<double> y = {2.74, 2.96, 3.68, 4.46, 5.22, 6.52, 7.43, 7.85, 8.02};

    double point = 4.1;  // Значение, для которого нужно найти интерполяционный многочлен

    LagrangeInterpolation(x, y, point);
    NewtonInterpolation(x, y, point);
    
    return 0;
}