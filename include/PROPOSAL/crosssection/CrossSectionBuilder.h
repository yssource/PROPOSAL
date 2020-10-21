
 #pragma once

 #include "PROPOSAL/crosssection/CrossSection.h"
 #include "PROPOSAL/crosssection/CrossSectionIntegral.h"
 #include "PROPOSAL/crosssection/CrossSectionInterpolant.h"

namespace PROPOSAL{


template <typename Param, typename P, typename M>
std::unique_ptr<crosssection_t<P, M>> make_crosssection(Param&& param, P&& p_def,
    M&& medium, std::shared_ptr<const EnergyCutSettings> cuts, bool interpolate)
{
    if (interpolate)
        return PROPOSAL::make_unique<CrossSectionInterpolant<Param, P, M>>(
            param, std::forward<P>(p_def), std::forward<M>(medium), cuts);
    return PROPOSAL::make_unique<CrossSectionIntegral<Param, P, M>>(
        param, std::forward<P>(p_def), std::forward<M>(medium), cuts);
}

}
