#include "ray_utility.h"

#include <iostream>
#include <iomanip>

int main() {
    std::cout << std::fixed << std::setprecision(12);

    int inside_circle = 0;
    int runs = 0;
    while (true) {
        runs++;
        auto x = getRandomDouble(-1, 1);
        auto y = getRandomDouble(-1, 1);
        if (x * x + y * y < 1)
            inside_circle++;

        if (runs % 100000 == 0)
            std::cout << "\rEstimate of Pi = " << (4.0 * inside_circle) / runs;
    }

    return 0;
}
