
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

#include "PROPOSAL/Constants.h"
#include "PROPOSAL/crossection/CrossSection.h"
#include "PROPOSAL/math/Integral.h"
#include "PROPOSAL/medium/Medium.h"
#include "PROPOSAL/particle/ParticleDef.h"
#include <unordered_map>

using std::add_lvalue_reference;
using std::get;
using std::forward;
using std::shared_ptr;
using std::decay;
using std::unordered_map;

namespace PROPOSAL {
template <typename Param, typename P, typename M>
class CrossSectionIntegral : public crosssection_t<P, M> {

    using param_t = typename decay<Param>::type;
    using particle_t = typename decay<P>::type;
    using medium_t = typename decay<M>::type;
    using base_param_ref_t =
        typename add_lvalue_reference<typename param_t::base_param_t>::type;

    Integral integral;
    param_t param;
    particle_t p_def;
    medium_t medium;
    shared_ptr<const EnergyCutSettings> cut;

    rates_t CalculatedNdx_impl(double energy, std::true_type);
    rates_t CalculatedNdx_impl(double energy, std::false_type);

    double CalculateStochasticLoss_impl(
        const Component&, double, double, std::true_type);
    double CalculateStochasticLoss_impl(
        const Component&, double, double, std::false_type);

public:
    CrossSectionIntegral(Param&& _param, P&& _p_def, M&& _medium,
        shared_ptr<const EnergyCutSettings> _cut)
        : CrossSection<typename decay<P>::type, typename decay<M>::type>()
        , param(forward<Param>(_param))
        , p_def(forward<P>(_p_def))
        , medium(forward<M>(_medium))
        , cut(_cut)
    {
    }

    inline double CalculatedEdx(double energy) override
    {
        return calculate_dedx(reinterpret_cast<base_param_ref_t>(param),
            integral, p_def, medium, *cut, energy,
            typename decay<Param>::type::only_stochastic{});
    }
    inline double CalculatedE2dx(double energy) override
    {
        return calculate_de2dx(reinterpret_cast<base_param_ref_t>(param),
            integral, p_def, medium, *cut, energy,
            typename decay<Param>::type::only_stochastic{});
    }
    inline rates_t CalculatedNdx(double energy) override
    {
        return CalculatedNdx_impl(
            energy, typename param_t::base_param_t::component_wise{});
    }
    inline double CalculateStochasticLoss(
        const Component& comp, double energy, double rate) override
    {
        return CalculateStochasticLoss_impl(comp, energy, rate,
            typename param_t::base_param_t::component_wise{});
    }
    inline size_t GetHash() const noexcept override
    {
        auto hash_digest = size_t{ 0 };
        hash_combine(hash_digest, param.GetHash(), p_def.GetHash(),
            medium.GetHash(), cut->GetHash());
        return hash_digest;
    }
    inline double GetLowerEnergyLim() const override
    {
        return param.GetLowerEnergyLim(p_def);
    }
    inline InteractionType GetInteractionType() const noexcept override
    {
        return param.interaction_type;
    }
};
} // namespace PROPOSAL

namespace PROPOSAL {
template <typename Param>
double calculate_dedx(Param&& param, Integral& integral,
    const ParticleDef& p_def, const Medium& medium,
    const EnergyCutSettings& cut, double energy, std::false_type)
{
    double sum = 0.;
    for (const auto& comp : medium.GetComponents()) {
        auto lim = param.GetKinematicLimits(p_def, comp, energy);
        auto v_cut = cut.GetCut(lim, energy);
        sum += integrate_dedx(integral, param, p_def, comp, energy,
            get<Parametrization::V_MIN>(lim), v_cut);
    }
    return energy * sum;
}

template <typename Param>
double calculate_dedx(Param&&, Integral&, const ParticleDef&, const Medium&,
    const EnergyCutSettings&, double, std::true_type)
{
    return 0.;
}

class Ionization;
template <>
double calculate_dedx(Ionization&, Integral&, const ParticleDef&, const Medium&,
    const EnergyCutSettings&, double, std::false_type);

template <typename Param>
double calculate_de2dx(Param&& param, Integral& integral,
    const ParticleDef& p_def, const Medium& medium,
    const EnergyCutSettings& cut, double energy, std::false_type)
{
    double sum = 0.;
    for (const auto& comp : medium.GetComponents()) {
        auto lim = param.GetKinematicLimits(p_def, comp, energy);
        auto v_cut = cut.GetCut(lim, energy);
        sum += integrate_de2dx(integral, param, p_def, comp, energy,
            get<Parametrization::V_MIN>(lim), v_cut);
    }
    return energy * energy * sum;
}

template <typename Param>
double calculate_de2dx(Param&&, Integral&, const ParticleDef&, const Medium&,
    const EnergyCutSettings&, double, std::true_type)
{
    return 0.;
}

template <typename P, typename M>
double calculate_dndx(P&& param, Integral& integral, const ParticleDef& p_def,
    M&& medium, const EnergyCutSettings& cut, double energy)
{
    auto lim = param.GetKinematicLimits(p_def, medium, energy);
    auto v_cut = cut.GetCut(lim, energy);
    return integrate_dndx(integral, param, p_def, medium, energy, v_cut,
        get<Parametrization::V_MAX>(lim));
}

template <typename Param, typename P, typename M>
rates_t CrossSectionIntegral<Param, P, M>::CalculatedNdx_impl(
    double energy, std::true_type)
{
    rates_t rates;
    for (auto& c : medium.GetComponents())
        rates[&c] = calculate_dndx(reinterpret_cast<base_param_ref_t>(param),
            integral, p_def, c, *cut, energy);
    return rates;
}

template <typename Param, typename P, typename M>
rates_t CrossSectionIntegral<Param, P, M>::CalculatedNdx_impl(
    double energy, std::false_type)
{
    rates_t rates;
    rates[nullptr] = calculate_dndx(reinterpret_cast<base_param_ref_t>(param),
        integral, p_def, medium, *cut, energy);
    return rates;
}

template <typename Param, typename P, typename M>
double CrossSectionIntegral<Param, P, M>::CalculateStochasticLoss_impl(
    const Component& comp, double energy, double rate, std::true_type)
{
    auto lim = param.GetKinematicLimits(p_def, comp, energy);
    auto v_cut = cut->GetCut(lim, energy);
    return calculate_upper_lim_dndx(integral, param, p_def, comp, energy, v_cut,
        get<Parametrization::V_MAX>(lim), -rate);
}

template <typename Param, typename P, typename M>
double CrossSectionIntegral<Param, P, M>::CalculateStochasticLoss_impl(
    const Component& comp, double energy, double rate, std::false_type)
{
    auto lim = param.GetKinematicLimits(p_def, medium, energy);
    auto v_cut = cut->GetCut(lim, energy);
    return calculate_upper_lim_dndx(integral, param, p_def, medium, energy,
        v_cut, get<Parametrization::V_MAX>(lim), -rate);
}
} // namepace PROPOSAL
