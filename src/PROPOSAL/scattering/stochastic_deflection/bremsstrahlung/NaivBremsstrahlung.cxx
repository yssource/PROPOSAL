#include "PROPOSAL/scattering/stochastic_deflection/bremsstrahlung/NaivBremsstrahlung.h"

using namespace PROPOSAL;

std::array<double, 2>
stochastic_deflection::NaivBremsstrahlung::CalculateStochasticDeflection(
    double initial_energy, double final_energy,
    std::vector<double> const&) const
{
    return std::array<double, 2> { 1.23, 3.45 };
}