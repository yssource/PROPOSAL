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


#include "PROPOSAL/particle/Particle.h"
#include "PROPOSAL/math/Vector3D.h"
#include <memory>
#include <vector>

namespace PROPOSAL {

class Secondaries {

public:
    Secondaries();
    Secondaries(size_t number_secondaries);

    /* void push_back(const Particle& particle); */
    void push_back(const DynamicData& continuous_loss);
    void push_back(const Particle& particle, const DynamicData::Type& secondary, const double& energyloss);
    void emplace_back(const DynamicData::Type& type, const Vector3D& position,
        const Vector3D& direction, const double& energy, const double& parent_particle_energy,
        const double& time, const double& distance);

    void append(Secondaries secondaries);

    void clear() { secondaries_.clear(); };
    std::vector<DynamicData> GetSecondaries() { secondaries_.shrink_to_fit(); return secondaries_; };
    unsigned int GetNumberOfParticles() { return secondaries_.size(); };

private:
    std::vector<DynamicData> secondaries_;
};

} // namespace PROPOSAL
