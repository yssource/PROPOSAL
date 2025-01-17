
#pragma once

#include "PROPOSAL/crosssection/CrossSection.h"
#include "PROPOSAL/crosssection/CrossSectionIntegral.h"
#include "PROPOSAL/crosssection/CrossSectionInterpolant.h"
#include "PROPOSAL/crosssection/CrossSectionDirect.h"

namespace PROPOSAL {
namespace detail {
template <typename Param, typename P, typename M>
auto make_crosssection_impl(Param&& param, P&& p_def, M&& medium,
                            std::shared_ptr<const EnergyCutSettings> cuts,
                            bool interpolate, std::false_type)
{
    auto cross = std::unique_ptr<CrossSectionBase>();
    if (interpolate)
        cross = std::make_unique<CrossSectionInterpolant<Param>>(
            std::forward<Param>(param), std::forward<P>(p_def),
            std::forward<M>(medium), cuts);
    else
        cross = std::make_unique<CrossSectionIntegral<Param>>(
            std::forward<Param>(param), std::forward<P>(p_def),
            std::forward<M>(medium), cuts);
    return cross;
}

// Implicit function to create CrossSectionDirect object
template <typename Param, typename P, typename M>
auto make_crosssection_impl(Param&& param, P&& p_def, M&& medium,
                            std::shared_ptr<const EnergyCutSettings> cuts,
                            bool interpolate, std::true_type)
{
    auto cross = std::unique_ptr<CrossSectionBase>();
    cross = std::make_unique<CrossSectionDirect>(
            std::forward<Param>(param), std::forward<P>(p_def),
            std::forward<M>(medium), cuts, false);
    return cross;
}
}

template <typename Param, typename P, typename M>
auto make_crosssection(Param&& param, P&& p_def, M&& medium,
                       std::shared_ptr<const EnergyCutSettings> cuts,
                       bool interpolate)
{
    // Call alternative implicit creation function if param
    // is a `ParametrizationDirect` object. In this case, the last argument of
    // `make_crosssection_impl` is a std::true_type, otherwise a std::false_type
    return detail::make_crosssection_impl(
            std::forward<Param>(param), std::forward<P>(p_def),
            std::forward<M>(medium), cuts, interpolate,
            std::is_base_of<typename crosssection::ParametrizationDirect,
                            typename std::decay<Param>::type>{});
}
}