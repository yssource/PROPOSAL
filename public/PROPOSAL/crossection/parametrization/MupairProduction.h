
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

#include <functional>
#include <cmath>

#include "PROPOSAL/crossection/parametrization/Parametrization.h"
#include "PROPOSAL/medium/Medium.h"

#include "PROPOSAL/math/Integral.h"
#include "PROPOSAL/math/Interpolant.h"
#include "PROPOSAL/math/InterpolantBuilder.h"

#include "PROPOSAL/methods.h"
#include "PROPOSAL/Logging.h"

#define MUPAIR_PARAM_INTEGRAL_DEC(param)                                                                               \
    class Mupair##param : public MupairProductionRhoIntegral                                                           \
    {                                                                                                                  \
    public:                                                                                                            \
        Mupair##param(const ParticleDef&, std::shared_ptr<const Medium>, double multiplier);                                           \
        Mupair##param(const Mupair##param&);                                                                           \
        virtual ~Mupair##param();                                                                                      \
                                                                                                                       \
        virtual Parametrization* clone() const { return new Mupair##param(*this); }                                    \
        static MupairProduction* create(const ParticleDef& particle_def,                                               \
                                       std::shared_ptr<const Medium> medium,                                                           \
                                       double multiplier)                                                              \
        {                                                                                                              \
            return new Mupair##param(particle_def, medium, multiplier);                                                \
        }                                                                                                              \
                                                                                                                       \
        double FunctionToIntegral(double energy, double v, double r);                                                  \
                                                                                                                       \
        const std::string& GetName() const { return name_; }                                                           \
                                                                                                                       \
    protected:                                                                                                         \
        static const std::string name_;                                                                                \
    };


namespace PROPOSAL {

class Interpolant;

class MupairProduction : public Parametrization
{
public:
    MupairProduction(const ParticleDef&, std::shared_ptr<const Medium>, double multiplier);
    MupairProduction(const MupairProduction&);
    virtual ~MupairProduction();

    virtual Parametrization* clone() const = 0;

    // ----------------------------------------------------------------- //
    // Public methods
    // ----------------------------------------------------------------- //

    // ----------------------------------------------------------------------------
    /// @brief This is the calculation of the dSigma/dv
    // ----------------------------------------------------------------------------
    virtual InteractionType GetInteractionType() const final {return InteractionType::MuPair;}
    virtual double DifferentialCrossSection(double energy, double v) = 0;
    virtual double FunctionToIntegral(double energy, double v, double rho) = 0;
    virtual double Calculaterho(double energy, double v, double rnd1, double rnd2);

    virtual KinematicLimits GetKinematicLimits(double energy);

protected:
    bool compare(const Parametrization&) const;
    Integral drho_integral_;
};

// ------------------------------------------------------------------------- //
// Differentiate between rho integration & interpolation
// ------------------------------------------------------------------------- //

class MupairProductionRhoIntegral : public MupairProduction
{
public:
    MupairProductionRhoIntegral(const ParticleDef&,
                               std::shared_ptr<const Medium>,
                               double multiplier);
    MupairProductionRhoIntegral(const MupairProductionRhoIntegral&);
    virtual ~MupairProductionRhoIntegral();

    Parametrization* clone() const = 0;

    virtual double DifferentialCrossSection(double energy, double v);

    // ----------------------------------------------------------------------------
    /// @brief This is the calculation of the d2Sigma/dvdRo - interface to Integral
    ///
    // ----------------------------------------------------------------------------

    virtual size_t GetHash() const;

private:
    bool compare(const Parametrization&) const;
    //virtual void print(std::ostream&) const;

    Integral integral_;
};

/******************************************************************************
 *                     Declare Integral Parametrizations                      *
 ******************************************************************************/

MUPAIR_PARAM_INTEGRAL_DEC(KelnerKokoulinPetrukhin)

/******************************************************************************
 *                    Declare Interpolant Parametrizations                    *
 ******************************************************************************/

template<class Param = MupairKelnerKokoulinPetrukhin>
class MupairProductionRhoInterpolant : public Param
{
public:
    typedef std::vector<std::unique_ptr<Interpolant>> InterpolantVec;

public:
    MupairProductionRhoInterpolant(const ParticleDef&,
                       std::shared_ptr<const Medium>,
                       double multiplier,
                       std::shared_ptr<const InterpolationDef>);
    MupairProductionRhoInterpolant(const MupairProductionRhoInterpolant&);

    Parametrization* clone() const { return new MupairProductionRhoInterpolant<Param>(*this); }
    static MupairProduction* create(const ParticleDef& particle_def,
                                std::shared_ptr<const Medium> medium,
                                double multiplier,
                                std::shared_ptr<const InterpolationDef> def)
    {
        return new MupairProductionRhoInterpolant<Param>(particle_def, medium, multiplier, def);
    }

    double DifferentialCrossSection(double energy, double v);

protected:
    virtual bool compare(const Parametrization&) const;
    double FunctionToBuildPhotoInterpolant(double energy, double v, int component);

    InterpolantVec interpolant_;
};

template<class Param>
MupairProductionRhoInterpolant<Param>::MupairProductionRhoInterpolant(const ParticleDef& particle_def,
                                              std::shared_ptr<const Medium> medium,
                                              double multiplier,
                                              std::shared_ptr<const InterpolationDef> def)
    : Param(particle_def, medium, multiplier)
    , interpolant_(this->medium_->GetNumComponents())
{
    log_warn("Using MupairProductionRhoInterpolant may lead to inaccurate results for specific values. "
             "Consider using the Integral option");
    std::vector<Interpolant2DBuilder> builder2d(this->components_.size());
    Helper::InterpolantBuilderContainer builder_container2d(this->components_.size());

    for (unsigned int i = 0; i < this->components_.size(); ++i)
    {
        builder2d[i]
            .SetMax1(def->nodes_cross_section)
            .SetX1Min(particle_def.mass)
            .SetX1Max(def->max_node_energy)
            .SetMax2(def->nodes_cross_section)
            .SetX2Min(0.0)
            .SetX2Max(1.0)
            .SetRomberg1(def->order_of_interpolation)
            .SetRational1(false)
            .SetRelative1(false)
            .SetIsLog1(true)
            .SetRomberg2(def->order_of_interpolation)
            .SetRational2(false)
            .SetRelative2(false)
            .SetIsLog2(false)
            .SetRombergY(def->order_of_interpolation)
            .SetRationalY(false)
            .SetRelativeY(false)
            .SetLogSubst(false)
            .SetFunction2D(std::bind(&MupairProductionRhoInterpolant::FunctionToBuildPhotoInterpolant, this, std::placeholders::_1, std::placeholders::_2, i));

        builder_container2d[i]  = &builder2d[i];
    }

    interpolant_ = Helper::InitializeInterpolation("Mupair", builder_container2d, this->GetHash(), *def);
}

template<class Param>
MupairProductionRhoInterpolant<Param>::MupairProductionRhoInterpolant(const MupairProductionRhoInterpolant& photo)
    : Param(photo)
    , interpolant_()
{
    interpolant_.resize(photo.interpolant_.size());

    for (unsigned int i = 0; i < photo.interpolant_.size(); ++i)
    {
        interpolant_[i] = std::unique_ptr<Interpolant>(new Interpolant(*photo.interpolant_[i]));
    }
}

template<class Param>
bool MupairProductionRhoInterpolant<Param>::compare(const Parametrization& parametrization) const
{
    const MupairProductionRhoInterpolant<Param>* mupair = static_cast<const MupairProductionRhoInterpolant<Param>*>(&parametrization);

    if (interpolant_.size() != mupair->interpolant_.size())
        return false;

    for (unsigned int i = 0; i < interpolant_.size(); ++i)
    {
        if (*interpolant_[i] != *mupair->interpolant_[i])
            return false;
    }

    return MupairProduction::compare(parametrization);
}

template<class Param>
double MupairProductionRhoInterpolant<Param>::DifferentialCrossSection(double energy, double v)
{
    Parametrization::KinematicLimits limits = this->GetKinematicLimits(energy);

    if (v > limits.vMin && v < limits.vMax)
    {
        return std::max(
            interpolant_.at(this->component_index_)->Interpolate(energy, std::log(v / limits.vMin) / std::log(limits.vMax / limits.vMin)),
            0.0);
    } else
    {
        return Param::DifferentialCrossSection(energy, v);
    }
}

template<class Param>
double MupairProductionRhoInterpolant<Param>::FunctionToBuildPhotoInterpolant(double energy, double v, int component)
{
    this->component_index_                 = component;
    Parametrization::KinematicLimits limits = this->GetKinematicLimits(energy);

    // TODO: This substitution results in interpolated values that deviate more than IPREC from the integrated values and way more for v close to 1 (jean-marco)
    v = limits.vMin * std::exp(v * std::log(limits.vMax / limits.vMin));

    return Param::DifferentialCrossSection(energy, v);
}

#undef MUPAIR_PARAM_INTEGRAL_DEC

} // namespace PROPOSAL
