#include "ray_utility.h"

#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(12);

    int inside_circle = 0;
    int inside_circle_stratified = 0;
    int sqrt_N = 1000;

    for (int i = 0; i < sqrt_N; i++) {
        for (int j = 0; j < sqrt_N; j++) {
            auto x = getRandomDouble(-1, 1);
            auto y = getRandomDouble(-1, 1);
            if (x * x + y * y < 1)
                inside_circle++;

            x = 2 * ((i + getRandomDouble()) / sqrt_N) - 1;
            y = 2 * ((j + getRandomDouble()) / sqrt_N) - 1;
            if (x * x + y * y < 1)
                inside_circle_stratified++;
        }
    }

    std::cout
        << "Regular    Estimate of Pi = "
        << (4.0 * inside_circle) / (sqrt_N * sqrt_N) << '\n'
        << "Stratified Estimate of Pi = "
        << (4.0 * inside_circle_stratified) / (sqrt_N * sqrt_N) << '\n';

    return 0;
}
