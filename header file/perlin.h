#ifndef PERLIN_H
#define PERLIN_H

#include "ray_utility.h"

class Perlin {
public:
    Perlin() {
        for (int currentPoint = 0; currentPoint < POINT_COUNT; ++currentPoint)
            tableFloat[currentPoint] = getRandomDouble();

        generatePermutationTable(tablePermutationX);
        generatePermutationTable(tablePermutationY);
        generatePermutationTable(tablePermutationZ);
    }

    double getNoise(const Point3& hitPosition) const {
        auto i = int(4 * hitPosition.getX()) & (POINT_COUNT - 1);
        auto j = int(4 * hitPosition.getY()) & (POINT_COUNT - 1);
        auto k = int(4 * hitPosition.getZ()) & (POINT_COUNT - 1);

        return tableFloat[tablePermutationX[i] ^ tablePermutationY[j] ^ tablePermutationY[k]];
    }

private:
    static constexpr int POINT_COUNT = 256;
    double tableFloat[POINT_COUNT];
    int tablePermutationX[POINT_COUNT];
    int tablePermutationY[POINT_COUNT];
    int tablePermutationZ[POINT_COUNT];

    static void generatePermutationTable(int* currentTable) {
        for (int currentPoint = 0; currentPoint < POINT_COUNT; ++currentPoint)
            currentTable[currentPoint] = currentPoint;

        permute(currentTable, POINT_COUNT);
    }

    static void permute(int* currentTable, const int END_POINT) {
        for (int currentPoint = END_POINT - 1; currentPoint > 0; --currentPoint) {
            int target = getRandomInt(0, currentPoint);
            int tempValue = currentTable[currentPoint];
            currentTable[currentPoint] = currentTable[target];
            currentTable[target] = tempValue;
        }
    }
};

#endif
