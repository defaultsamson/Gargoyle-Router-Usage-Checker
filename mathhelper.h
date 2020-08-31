#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <cmath>

class MathHelper {
public:
    /// Returns a percentage with 1 decimal point of accuracy
    static double percentage(double value, double max) {
        double p = ratio(value, max) * 100.0;
        return decimalPoint(p);
    }

    /// Returns a double ratio
    static double ratio(double value, double max) {
        return value / max;
    }

    /// Returns a double with 1 decimal point of accuracy
    static double decimalPoint(double value) {
        return std::round(value * 10.0) / 10.0;
    }
};

#endif // MATHHELPER_H
