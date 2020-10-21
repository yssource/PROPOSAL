#pragma once

#include "PROPOSAL/math/InterpolantBuilder.h"


namespace PROPOSAL {
struct Time {
    Time() = default;
    virtual ~Time() = default;

    static std::unique_ptr<Interpolant1DBuilder::Definition> interpol_def;

    virtual double TimeElapsed(double, double, double, double) = 0;
};
}
