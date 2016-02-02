/*
 * This macro creates a set of three oscillation probability vectors for
 * energy bins as specified in the arguments. The probability assigned
 * to each bin is that of a neutrino with an energy at the midpoint of
 * the bin. Uses the Prob3++ oscillation probability calculator and the
 * Nu-Fit JHEP 11 (2014) 052 [arXiv:1409.5439] oscillation parameters.
 */
#include "/dune/app/users/lblpwg_tools/INSTALLATION/include/Prob3++/BargerPropagator.h"
#include <fstream>
#include <TMath.h>
#include "NuIndex2str.C"

int nuIndex2str(int, std::string&);
int CreateOscillationVectors(const size_t, const double, const double);

int CreateOscillationVectors(const size_t NBINS, const double EMIN,
        const double EMAX)
{
    const size_t MAXBINS = 1000;
    if(NBINS > MAXBINS)
    {
        std::cout << "ERROR: NBINS > MAXBINS (" << MAXBINS << ")\n";
        return 1;
    }
    // Oscprobs from Nu-Fit  JHEP 11 (2014) 052 [arXiv:1409.5439]
    // I am assuming normal ordering, delta-cp = 0
    const double x13 = 0.0218; // sin^2(x)
    const double x12 = 0.304; // sin^2(x)
    const double x23 = 0.452; // sin^2(x)
    const double dm21 = 0.0000750; // eV^2
    const double dm31 = 0.002457; // eV^2
    const double dcp = 0.0;
    const bool THETA_STYLE = true;
    const double BASELINE = 1300; // km
    const double DENSITY = 2.7; // g/cm^3

    const double ESTEP = (EMAX - EMIN) / NBINS;
    const double ESTART = EMIN + ESTEP/2;
    for(int startnu = -3; startnu <= +3; ++startnu)
    {
        if(startnu == 0)
        {
            continue;
        }
        int nusign = startnu > 0 ? 1 : -1;
        for(int endnuflavor = 1; endnuflavor <= 3; ++endnuflavor)
        {
            int endnu = endnuflavor * nusign;
            double probabilities[MAXBINS];
            std::string startnustr;
            std::string endnustr;
            nuIndex2str(startnu, startnustr);
            nuIndex2str(endnu, endnustr);
            size_t ebin = 0;
            for(double energy = ESTART; energy < EMAX; energy += ESTEP, ++ebin)
            {
                BargerPropagator* BP = new BargerPropagator();
                BP->SetMNS(x12, x13, x23, dm21, dm31, dcp, energy,
                        THETA_STYLE, startnu);
                BP->propagateLinear(startnu, BASELINE, DENSITY);
                probabilities[ebin] = BP->GetProb(startnu, endnu);
                delete BP;
                BP = 0;
            }
            // dump the probabilities vector to a csv
            std::ofstream fout;
            char filenameend[10]; // 10 is safe with 1000 max bins
            sprintf(filenameend, "%d.csv", NBINS);
            fout.open((std::string("/afs/fnal.gov/files/home/room3/") +
                        "skohn/outputs/oscvectors/" + startnustr +
                        "_" + endnustr + filenameend).c_str());
            if(!fout.is_open())
            {
                std::cout << "ERROR: Could not open file\n";
                return 2;
            }
            else
            {
                std::cout << "INFO: Opened output file\n";
            }
            ebin = 0;
            fout << probabilities[ebin];
            ++ebin;
            while(ebin < NBINS)
            {
                fout << ", " << probabilities[ebin];
                ++ebin;
            }
            fout.close();
        }
    }
    return 0;
}
