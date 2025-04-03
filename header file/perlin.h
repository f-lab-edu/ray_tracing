#ifndef PERLIN_H
#define PERLIN_H

#include "ray_utility.h"

class Perlin {
public:
    Perlin() {
        for (int currentPoint = 0; currentPoint < POINT_COUNT; ++currentPoint)
            tableVector[currentPoint] = getUnitVector(Vec3::getRandomVector(-1,1));

        generatePermutationTable(tablePermutationX);
        generatePermutationTable(tablePermutationY);
        generatePermutationTable(tablePermutationZ);
    }

    double getNoise(const Point3& hitPosition) const {
        auto u = hitPosition.getX() - std::floor(hitPosition.getX());
        auto v = hitPosition.getY() - std::floor(hitPosition.getY());
        auto w = hitPosition.getZ() - std::floor(hitPosition.getZ());
        /*
        u = u * u * (3 - 2 * u);
        v = v * v * (3 - 2 * v);
        w = w * w * (3 - 2 * w);
        */

        auto i = int(std::floor(hitPosition.getX()));
        auto j = int(std::floor(hitPosition.getY()));
        auto k = int(std::floor(hitPosition.getZ()));
        Vec3 c[2][2][2];

        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = tableVector[
                        tablePermutationX[(i + di) & 255] ^
                            tablePermutationY[(j + dj) & 255] ^
                            tablePermutationZ[(k + dk) & 255]
                    ];

        return getPerlinInterpolation(c, u, v, w);
    }

private:
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

    static double getTrilinearInterpolation(double c[2][2][2], double u, double v, double w) {
        auto accumulatedValue = 0.0;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++)
                    accumulatedValue += (i * u + (1 - i) * (1 - u))
                    * (j * v + (1 - j) * (1 - v))
                    * (k * w + (1 - k) * (1 - w))
                    * c[i][j][k];

        return accumulatedValue;
    }
    static double getPerlinInterpolation(const Vec3 c[2][2][2], double u, double v, double w) {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);
        auto accumulatedValue = 0.0;

        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++) {
                    Vec3 weightVector(u - i, v - j, w - k);
                    accumulatedValue += (i * uu + (1 - i) * (1 - uu))
                        * (j * vv + (1 - j) * (1 - vv))
                        * (k * ww + (1 - k) * (1 - ww))
                        * performDot(c[i][j][k], weightVector);
                }

        return accumulatedValue;
    }

    static constexpr int POINT_COUNT = 256;
    Vec3 tableVector[POINT_COUNT];
    int tablePermutationX[POINT_COUNT];
    int tablePermutationY[POINT_COUNT];
    int tablePermutationZ[POINT_COUNT];
};

#endif
