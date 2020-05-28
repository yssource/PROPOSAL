/******************************************************************************
 *                                                                            *
 * This file is part of the simulation tool PROPOSAL.                         *
 *                                                                            *
 * Copyright (C) 2017 TU Dortmund University, Department of Physics,          *
 *                    Chair Experimental Physics 5b                           *
 *                                                                            *
 * This software may be modified and distributed under the terms of a         *
 * modified GNU Lesser General Public Licence version 3 (LGPL),               *
 * copied verbatim in the file "LICENSE".                                     *
 *                                                                            *
 * Modifcations to the LGPL License:                                          *
 *                                                                            *
 *      1. The user shall acknowledge the use of PROPOSAL by citing the       *
 *         following reference:                                               *
 *                                                                            *
 *         J.H. Koehne et al.  Comput.Phys.Commun. 184 (2013) 2070-2090 DOI:  *
 *         10.1016/j.cpc.2013.04.001                                          *
 *                                                                            *
 *      2. The user should report any bugs/errors or improvments to the       *
 *         current maintainer of PROPOSAL or open an issue on the             *
 *         GitHub webpage                                                     *
 *                                                                            *
 *         "https://github.com/tudo-astroparticlephysics/PROPOSAL"            *
 *                                                                            *
 ******************************************************************************/

#pragma once

#include "PROPOSAL/crossection/parametrization/Parametrization.h"
#include <memory>

#define BREMSSTRAHLUNG_DEF(param)                                              \
    struct Brems##param : public Bremsstrahlung {                              \
        Brems##param(bool);                                                    \
        using base_param_t = Bremsstrahlung;                                   \
                                                                               \
        double CalculateParametrization(const ParticleDef&, const Component&,  \
            double energy, double v) override;                                 \
    };

using std::unique_ptr;

namespace PROPOSAL {
class Interpolant;
} // namespace PROPOSAL

namespace PROPOSAL {
class Bremsstrahlung : public Parametrization {

protected:
    bool lorenz_;       // enable lorenz cut
    double lorenz_cut_; // in [MeV]
    bool init_lpm_effect_;
    bool lpm_;
    double eLpm_;

    double lpm(const ParticleDef&, const Component&, double, double, double);

public:
    using only_stochastic = std::false_type;
    using component_wise = std::true_type;
    Bremsstrahlung(bool);
    virtual ~Bremsstrahlung() = default;

    virtual double DifferentialCrossSection(
        const ParticleDef&, const Component&, double, double);
    virtual double CalculateParametrization(
        const ParticleDef&, const Component&, double, double)
        = 0;

    KinematicLimits GetKinematicLimits(
        const ParticleDef&, const Component&, double energy);
    double GetLowerEnergyLim(const ParticleDef&) const override;
    size_t GetHash() const;
};

BREMSSTRAHLUNG_DEF(PetrukhinShestakov)
BREMSSTRAHLUNG_DEF(KelnerKokoulinPetrukhin)
BREMSSTRAHLUNG_DEF(CompleteScreening)
BREMSSTRAHLUNG_DEF(AndreevBezrukovBugaev)
BREMSSTRAHLUNG_DEF(SandrockSoedingreksoRhode)

class BremsElectronScreening : public Bremsstrahlung {
    std::unique_ptr<Interpolant> interpolant_;

public:
    BremsElectronScreening(bool);

    double CalculateParametrization(
        const ParticleDef&, const Component&, double energy, double v) override;
    double DifferentialCrossSection(
        const ParticleDef&, const Component&, double energy, double v) override;
};

#undef BREMSSTRAHLUNG_DEF

} // namespace PROPOSAL
