
// #include <iostream>
// #include <string>

#include "gtest/gtest.h"

#include <fstream>
#include "PROPOSAL/Constants.h"
#include "PROPOSAL/crosssection/parametrization/Bremsstrahlung.h"
#include "PROPOSAL/math/RandomGenerator.h"
#include "PROPOSAL/medium/Medium.h"
#include "PROPOSAL/medium/MediumFactory.h"

using namespace PROPOSAL;

ParticleDef getParticleDef(const std::string& name)
{
    if (name == "MuMinus")
    {
        return MuMinusDef();
    } else if (name == "TauMinus")
    {
        return TauMinusDef();
    } else
    {
        return EMinusDef();
    }
}

const std::string testfile_dir = "bin/TestFiles/";

// TEST(Comparison, Comparison_equal)
// {
//     ParticleDef particle_def = MuMinusDef::Get();
//     auto medium = std::make_shared<const Water>();
//     EnergyCutSettings ecuts;
//     double multiplier = 1.;
//     bool lpm          = true;

//     BremsKelnerKokoulinPetrukhin* Brems_A =
//         new BremsKelnerKokoulinPetrukhin(particle_def, medium, ecuts, multiplier, lpm);
//     Parametrization* Brems_B = new BremsKelnerKokoulinPetrukhin(particle_def, medium, ecuts, multiplier, lpm);
//     EXPECT_TRUE(*Brems_A == *Brems_B);

//     BremsKelnerKokoulinPetrukhin param(particle_def, medium, ecuts, multiplier, lpm);
//     EXPECT_TRUE(param == *Brems_A);

//     BremsIntegral* Int_A        = new BremsIntegral(param);
//     CrossSectionIntegral* Int_B = new BremsIntegral(param);
//     EXPECT_TRUE(*Int_A == *Int_B);

//     InterpolationDef InterpolDef;
//     BremsInterpolant* Interpol_A        = new BremsInterpolant(param, InterpolDef);
//     CrossSectionInterpolant* Interpol_B = new BremsInterpolant(param, InterpolDef);
//     EXPECT_TRUE(*Interpol_A == *Interpol_B);

//     delete Brems_A;
//     delete Brems_B;
//     delete Int_A;
//     delete Int_B;
//     delete Interpol_A;
//     delete Interpol_B;
// }

// TEST(Comparison, Comparison_not_equal)
// {
//     ParticleDef mu_def  = MuMinusDef::Get();
//     ParticleDef tau_def = TauMinusDef::Get();
//     auto medium_1 = std::make_shared<const Water>();
//     auto medium_2 = std::make_shared<const Ice>();
//     EnergyCutSettings ecuts_1(500, -1);
//     EnergyCutSettings ecuts_2(-1, 0.05);
//     double multiplier_1 = 1.;
//     double multiplier_2 = 2.;
//     bool lpm_1          = true;
//     bool lpm_2          = false;

//     BremsKelnerKokoulinPetrukhin Brems_A(mu_def, medium_1, ecuts_1, multiplier_1, lpm_1);
//     BremsKelnerKokoulinPetrukhin Brems_B(tau_def, medium_1, ecuts_1, multiplier_1, lpm_1);
//     BremsKelnerKokoulinPetrukhin Brems_C(mu_def, medium_2, ecuts_1, multiplier_1, lpm_1);
//     BremsKelnerKokoulinPetrukhin Brems_D(mu_def, medium_1, ecuts_2, multiplier_1, lpm_1);
//     BremsKelnerKokoulinPetrukhin Brems_E(mu_def, medium_1, ecuts_1, multiplier_2, lpm_1);
//     BremsKelnerKokoulinPetrukhin Brems_F(mu_def, medium_1, ecuts_1, multiplier_1, lpm_2);
//     EXPECT_TRUE(Brems_A != Brems_B);
//     EXPECT_TRUE(Brems_A != Brems_C);
//     EXPECT_TRUE(Brems_A != Brems_C);
//     EXPECT_TRUE(Brems_A != Brems_E);
//     EXPECT_TRUE(Brems_A != Brems_F);

//     BremsAndreevBezrukovBugaev param_2(mu_def, medium_1, ecuts_1, multiplier_1, lpm_1);
//     BremsPetrukhinShestakov param_3(mu_def, medium_1, ecuts_1, multiplier_1, lpm_1);
//     BremsCompleteScreening param_4(mu_def, medium_1, ecuts_1, multiplier_1, lpm_1);
//     EXPECT_TRUE(Brems_A != param_2);
//     EXPECT_TRUE(Brems_A != param_3);
//     EXPECT_TRUE(Brems_A != param_4);
//     EXPECT_TRUE(param_2 != param_3);
//     EXPECT_TRUE(param_2 != param_4);
//     EXPECT_TRUE(param_3 != param_4);

//     BremsIntegral Int_A(param_2);
//     BremsIntegral Int_B(param_3);
//     EXPECT_TRUE(Int_A != Int_B);

//     InterpolationDef InterpolDef;
//     BremsInterpolant Interpol_A(param_2, InterpolDef);
//     BremsInterpolant Interpol_B(param_3, InterpolDef);
//     EXPECT_TRUE(Interpol_A != Interpol_B);
// }

// TEST(Assignment, Copyconstructor)
// {
//     ParticleDef particle_def = MuMinusDef::Get();
//     auto medium = std::make_shared<const Water>();
//     EnergyCutSettings ecuts;
//     double multiplier = 1.;
//     bool lpm          = true;

//     BremsKelnerKokoulinPetrukhin Brems_A(particle_def, medium, ecuts, multiplier, lpm);
//     BremsKelnerKokoulinPetrukhin Brems_B = Brems_A;
//     EXPECT_TRUE(Brems_A == Brems_B);

//     BremsIntegral Int_A(Brems_A);
//     BremsIntegral Int_B = Int_A;
//     EXPECT_TRUE(Int_A == Int_B);

//     InterpolationDef InterpolDef;
//     BremsInterpolant Interpol_A(Brems_A, InterpolDef);
//     BremsInterpolant Interpol_B = Interpol_A;
//     EXPECT_TRUE(Interpol_A == Interpol_B);
// }

// TEST(Assignment, Copyconstructor2)
// {
//     ParticleDef particle_def = MuMinusDef::Get();
//     auto medium = std::make_shared<const Water>();
//     EnergyCutSettings ecuts;
//     double multiplier = 1.;
//     bool lpm          = true;

//     BremsKelnerKokoulinPetrukhin Brems_A(particle_def, medium, ecuts, multiplier, lpm);
//     BremsKelnerKokoulinPetrukhin Brems_B(Brems_A);
//     EXPECT_TRUE(Brems_A == Brems_B);

//     BremsIntegral Int_A(Brems_A);
//     BremsIntegral Int_B(Int_A);
//     EXPECT_TRUE(Int_A == Int_B);

//     InterpolationDef InterpolDef;
//     BremsInterpolant Interpol_A(Brems_A, InterpolDef);
//     BremsInterpolant Interpol_B(Interpol_A);
//     EXPECT_TRUE(Interpol_A == Interpol_B);
// }

// in polymorphism an assignmant and swap operator doesn't make sense

TEST(Bremsstrahlung, Test_of_dEdx)
{
    std::string filename = testfile_dir + "Brems_dEdx.txt";
    std::ifstream in{filename};
    EXPECT_TRUE(in.good()) << "Test resource file '" << filename << "' could not be opened";


    std::string particleName;
    std::string mediumName;
    double ecut;
    double vcut;
    bool cont_rand = false;
    double multiplier;
    bool lpm;
    std::string parametrization;
    double energy;
    double dEdx_stored;
    double dEdx_new;

    std::cout.precision(16);

    while (in >> particleName >> mediumName >> ecut >> vcut >> multiplier >> lpm >> energy >> dEdx_stored >>
              parametrization)
    {
        parametrization.erase(0,5);
        if (vcut == -1)
            vcut = 1;
        if (ecut == -1)
            ecut = INF;

        ParticleDef particle_def = getParticleDef(particleName);
        auto medium = CreateMedium(mediumName);
        auto ecuts = std::make_shared<EnergyCutSettings>(ecut, vcut, cont_rand);

        auto cross = crosssection::make_bremsstrahlung(particle_def,
            *medium,
            ecuts,
            false,
            lpm,
            parametrization);

        dEdx_new = cross->CalculatedEdx(energy) * medium->GetMassDensity();

        ASSERT_NEAR(dEdx_new, dEdx_stored, 1e-3 * dEdx_stored);
    }
}

TEST(Bremsstrahlung, Test_of_dNdx)
{
    std::string filename = testfile_dir + "Brems_dNdx.txt";
    std::ifstream in{filename};
    EXPECT_TRUE(in.good()) << "Test resource file '" << filename << "' could not be opened";

    std::string particleName;
    std::string mediumName;
    double ecut;
    double vcut;
    bool cont_rand = false;
    double multiplier;
    bool lpm;
    std::string parametrization;
    double energy;
    double dNdx_stored;
    double dNdx_new;

    std::cout.precision(16);

    while (in >> particleName >> mediumName >> ecut >> vcut >> multiplier >> lpm >> energy >> dNdx_stored >>
              parametrization)
    {
        parametrization.erase(0,5);
        if (vcut == -1)
            vcut = 1;
        if (ecut == -1)
            ecut = INF;

        ParticleDef particle_def = getParticleDef(particleName);
        auto medium = CreateMedium(mediumName);
        auto ecuts = std::make_shared<EnergyCutSettings>(ecut, vcut, cont_rand);

        auto cross = crosssection::make_bremsstrahlung(particle_def,
            *medium,
            ecuts,
            false,
            lpm,
            parametrization);

        dNdx_new = cross->CalculatedNdx(energy) * medium->GetMassDensity();

        ASSERT_NEAR(dNdx_new, dNdx_stored, 1e-3 * dNdx_stored);
    }
}

TEST(Bremsstrahlung, Test_of_e)
{
    std::string filename = testfile_dir + "Brems_e.txt";
    std::ifstream in{filename};
    EXPECT_TRUE(in.good()) << "Test resource file '" << filename << "' could not be opened";

    std::string particleName;
    std::string mediumName;
    double ecut;
    double vcut;
    bool cont_rand = false;
    double multiplier;
    bool lpm;
    std::string parametrization;
    double energy;
    double rnd1;
    double rnd2;
    double stochastic_loss_stored;
    double stochastic_loss_new;

    std::cout.precision(16);

    RandomGenerator::Get().SetSeed(0);

    while (in >> particleName >> mediumName >> ecut >> vcut >> multiplier >> lpm >> energy >> rnd1 >> rnd2 >> stochastic_loss_stored >> parametrization)
    {
        parametrization.erase(0,5);
        if (vcut == -1)
            vcut = 1;
        if (ecut == -1)
            ecut = INF;

        ParticleDef particle_def = getParticleDef(particleName);
        auto medium = CreateMedium(mediumName);
        auto ecuts = std::make_shared<EnergyCutSettings>(ecut, vcut, cont_rand);

        auto cross = crosssection::make_bremsstrahlung(particle_def,
            *medium,
            ecuts,
            false,
            lpm,
            parametrization);

        auto dNdx_full = cross->CalculatedNdx(energy);
        auto components = cross->GetTargets();
        double sum = 0;

        for (auto comp : components)
        {
            double dNdx_for_comp = cross->CalculatedNdx(energy, comp);
            sum += dNdx_for_comp;
            if (sum > dNdx_full * (1. - rnd2)) {
                double rate_new = dNdx_for_comp * rnd1;
                if (ecut == INF and vcut == 1 ) {
                    #ifndef NDEBUG
                    EXPECT_DEATH(cross->CalculateStochasticLoss(comp, energy, rate_new), "");
                    #endif
                } else {
                    stochastic_loss_new = energy * cross->CalculateStochasticLoss(comp, energy, rate_new);
                    EXPECT_NEAR(stochastic_loss_new, stochastic_loss_stored, 1E-6 * stochastic_loss_stored);
                    break;
                }
            }
        }
    }
}

TEST(Bremsstrahlung, Test_of_dEdx_Interpolant)
{
    std::string filename = testfile_dir + "Brems_dEdx_interpol.txt";
    std::ifstream in{filename};
    EXPECT_TRUE(in.good()) << "Test resource file '" << filename << "' could not be opened";

    std::string particleName;
    std::string mediumName;
    double ecut;
    double vcut;
    bool cont_rand = false;
    double multiplier;
    bool lpm;
    std::string parametrization;
    double energy;
    double dEdx_stored;
    double dEdx_new;

    std::cout.precision(16);

    while (in >> particleName >> mediumName >> ecut >> vcut >> multiplier >> lpm >> energy >> dEdx_stored >> parametrization)
    {

        parametrization.erase(0,5);
        if (vcut == -1)
            vcut = 1;
        if (ecut == -1)
            ecut = INF;

        ParticleDef particle_def = getParticleDef(particleName);
        auto medium = CreateMedium(mediumName);
        auto ecuts = std::make_shared<EnergyCutSettings>(ecut, vcut, cont_rand);

        auto cross = crosssection::make_bremsstrahlung(particle_def,
            *medium,
            ecuts,
            true,
            lpm,
            parametrization);

        dEdx_new = cross->CalculatedEdx(energy) * medium->GetMassDensity();

        ASSERT_NEAR(dEdx_new, dEdx_stored, 1e-3 * dEdx_stored);
    }
}

TEST(Bremsstrahlung, Test_of_dNdx_Interpolant)
{
    std::string filename = testfile_dir + "Brems_dNdx_interpol.txt";
    std::ifstream in{filename};
    EXPECT_TRUE(in.good()) << "Test resource file '" << filename << "' could not be opened";


    std::string particleName;
    std::string mediumName;
    double ecut;
    double vcut;
    bool cont_rand = false;
    double multiplier;
    bool lpm;
    std::string parametrization;
    double energy;
    double dNdx_stored;
    double dNdx_new;

    std::cout.precision(16);
    InterpolationDef InterpolDef;

    while (in >> particleName >> mediumName >> ecut >> vcut >> multiplier >> lpm >> energy >> dNdx_stored >> parametrization)
    {
        parametrization.erase(0,5);
        if (vcut == -1)
            vcut = 1;
        if (ecut == -1)
            ecut = INF;

        ParticleDef particle_def = getParticleDef(particleName);
        auto medium = CreateMedium(mediumName);
        auto ecuts = std::make_shared<EnergyCutSettings>(ecut, vcut, cont_rand);

        auto cross = crosssection::make_bremsstrahlung(particle_def,
            *medium,
            ecuts,
            true,
            lpm,
            parametrization);

        dNdx_new = cross->CalculatedNdx(energy) * medium->GetMassDensity();

        ASSERT_NEAR(dNdx_new, dNdx_stored, 1e-3 * dNdx_stored);
    }
}

TEST(Bremsstrahlung, Test_of_e_Interpolant)
{
    std::string filename = testfile_dir + "Brems_e_interpol.txt";
    std::ifstream in{filename};
    EXPECT_TRUE(in.good()) << "Test resource file '" << filename << "' could not be opened";

    std::string particleName;
    std::string mediumName;
    double ecut;
    double vcut;
    bool cont_rand = false;
    double multiplier;
    bool lpm;
    std::string parametrization;
    double energy;
    double rnd1;
    double rnd2;
    double stochastic_loss_stored;
    double stochastic_loss_new;

    std::cout.precision(16);
    InterpolationDef InterpolDef;

    RandomGenerator::Get().SetSeed(0);

    while (in >> particleName >> mediumName >> ecut >> vcut >> multiplier >> lpm >> energy >> rnd1 >> rnd2 >> stochastic_loss_stored >> parametrization)
    {
        parametrization.erase(0,5);
        if (vcut == -1)
            vcut = 1;
        if (ecut == -1)
            ecut = INF;

        ParticleDef particle_def = getParticleDef(particleName);
        auto medium = CreateMedium(mediumName);
        auto ecuts = std::make_shared<EnergyCutSettings>(ecut, vcut, cont_rand);

        auto cross = crosssection::make_bremsstrahlung(particle_def,
            *medium,
            ecuts,
            true,
            lpm,
            parametrization);

        auto dNdx_full = cross->CalculatedNdx(energy);
        auto components = cross->GetTargets();
        double sum = 0;

        for (auto comp : components)
        {
            double dNdx_for_comp = cross->CalculatedNdx(energy, comp);
            sum += dNdx_for_comp;
            if (sum > dNdx_full * (1. - rnd2)) {
                double rate_new = dNdx_for_comp * rnd1;
                if (ecut == INF and vcut == 1 ) {
                    #ifndef NDEBUG
                    EXPECT_DEATH(cross->CalculateStochasticLoss(comp, energy, rate_new), "");
                    #endif
                } else {
                    stochastic_loss_new = energy * cross->CalculateStochasticLoss(comp, energy, rate_new);
                    EXPECT_NEAR(stochastic_loss_new, stochastic_loss_stored, 1E-6 * stochastic_loss_stored);
                    break;
                }
            }
        }
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
