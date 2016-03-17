/*
 * This macro creates a set of three oscillation probability vectors for
 * energy bins as specified in the arguments. The probability assigned
 * to each bin is that of a neutrino with an energy at the midpoint of
 * the bin. Uses the Prob3++ oscillation probability calculator, the
 * Nu-Fit JHEP 11 (2014) 052 [arXiv:1409.5439] oscillation parameters
 * using the normal hierarchy and delta-cp = 0, and a baseline of 1300km
 * and earth density of 2.7 g/cm^3.
 *
 * To run this macro:
 * $ root
 * [] .L path-to-barger-propagator-.so-file (sometimes called
 * libThreeProb.so)
 * [] .L CreateOscillationVectors.C+
 * [] and you're off!
 */
#include "/dune/app/users/lblpwg_tools/INSTALLATION/include/Prob3++/BargerPropagator.h"
#include <fstream>
#include <TMath.h>
#include "NuIndex2str.C"
#include "Configuration.C"

int CreateOscillationVectors(const size_t, const double, const double);
int CreateManyOscillationVectors(const size_t, const double, const double);
int CreateOscillationVectorsDriver(const size_t NBINS, const double EMIN,
        const double EMAX, std::string fout, const double x13,
        const double x12, const double x23, const double dm21,
        const double dm31, const double dcp, std::string header);

int CreateOscillationVectors(const size_t NBINS, const double EMIN,
        const double EMAX)
{
    // Oscprobs from Nu-Fit  JHEP 11 (2014) 052 [arXiv:1409.5439]
    // I am assuming normal ordering, delta-cp = 0
    const double x13 = 0.0218; // sin^2(x)
    const double x12 = 0.304; // sin^2(x)
    const double x23 = 0.452; // sin^2(x)
    const double dm21 = 0.0000750; // eV^2
    const double dm31 = 0.002457; // eV^2
    const double dcp = 0.0;
    char outputdir[100];
    std::string foutprefix = std::string(CFG_OutputDirectory(outputdir)) +
                        "oscvectors/";
    std::string fileheader = "# Nu-Fit JHEP 11 (2014) 052 [arXiv:1409.5439] NO";

    return CreateOscillationVectorsDriver(NBINS, EMIN, EMAX, foutprefix,
            x13, x12, x23, dm21, dm31, dcp, fileheader);
}

int CreateManyOscillationVectors(const size_t NBINS, const double EMIN,
        const double EMAX)
{
    // Loop over many oscillation parameters:
    // theta23 = 40, 45, 50 degrees
    // inverted and normal hierarchy
    // delta CP = 0, pi/2, pi, -pi/2, 0.15pi (which is the predicted
    // boundary for a 3-sigma result)
    std::vector<double> x13s;
    x13s.push_back(0.0218);
    std::vector<double> x12s;
    x12s.push_back(0.304);
    std::vector<double> x23s;
    x23s.push_back(0.413175911);
    x23s.push_back(0.45); // ~nu-fit value
    x23s.push_back(0.586825089);
    std::vector<double> dm21s;
    dm21s.push_back(0.0000750);
    std::vector<double> dm31s;
    dm31s.push_back(0.002457); // NO
    dm31s.push_back(-0.002449 + 0.0000750); // IO
    std::vector<double> dcps;
    dcps.push_back(0);
    dcps.push_back(0.15*TMath::Pi());
    dcps.push_back(TMath::PiOver2());
    dcps.push_back(-TMath::PiOver2());
    dcps.push_back(TMath::Pi());

    int paramSetCount = 0;
    std::vector<double>::iterator x13;
    std::vector<double>::iterator x12;
    std::vector<double>::iterator x23;
    std::vector<double>::iterator dm21;
    std::vector<double>::iterator dm31;
    std::vector<double>::iterator dcp;
    for(x13=x13s.begin(); x13 != x13s.end(); ++x13)
    {
        for(x12=x12s.begin(); x12 != x12s.end(); ++x12)
        {
            for(x23=x23s.begin(); x23 != x23s.end(); ++x23)
            {
                for(dm21=dm21s.begin(); dm21 != dm21s.end(); ++dm21)
                {
                    for(dm31=dm31s.begin();dm31!=dm31s.end();++dm31)
                    {
                        for(dcp=dcps.begin(); dcp!=dcps.end();++dcp)
                        {
                            ++paramSetCount;
                            char foldername[100];
                            char outputdir[100];
                            sprintf(foldername, "oscvectorsets/oscvectors_%d/", paramSetCount);
                            std::string foutprefix(CFG_OutputDirectory(outputdir));
                            foutprefix += foldername;

                            char fileheader[400];
                            sprintf(fileheader, "# %s\n#%s: %f\n#%s: %f\n#%s: %f\n#%s: %f\n#%s: %f\n#%s: %f",
                                    "Created with the following parameters:",
                                    "sin^2(theta13)", *x13,
                                    "sin^2(theta12)", *x12,
                                    "sin^2(theta23)", *x23,
                                    "delta m^2 21", *dm21,
                                    "delta m^2 31", *dm31,
                                    "delta CP", *dcp);
                            CreateOscillationVectorsDriver(NBINS, EMIN, EMAX,
                                    foutprefix, *x13, *x12, *x23, *dm21,
                                    *dm31, *dcp, fileheader);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

int CreateOscillationVectorsDriver(const size_t NBINS, const double EMIN,
        const double EMAX, std::string fout_prefix, const double x13,
        const double x12, const double x23, const double dm21,
        const double dm31, const double dcp, std::string fileheader)
{
    const size_t MAXBINS = 1000;
    if(NBINS > MAXBINS)
    {
        std::cout << "ERROR: NBINS > MAXBINS (" << MAXBINS << ")\n";
        return 1;
    }
    // Oscprobs from Nu-Fit  JHEP 11 (2014) 052 [arXiv:1409.5439]
    // I am assuming normal ordering, delta-cp = 0
    /*
    const double x13 = 0.0218; // sin^2(x)
    const double x12 = 0.304; // sin^2(x)
    const double x23 = 0.452; // sin^2(x)
    const double dm21 = 0.0000750; // eV^2
    const double dm31 = 0.002457; // eV^2
    const double dcp = 0.0;
    */
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
            NuIndex2str(startnu, startnustr);
            NuIndex2str(endnu, endnustr);
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
            char outputdir[100];
            sprintf(filenameend, "%d.csv", NBINS);
            fout.open((fout_prefix + startnustr + "_" + endnustr +
                        filenameend).c_str());
            if(!fout.is_open())
            {
                std::cout << "ERROR: Could not open file\n";
                return 2;
            }
            else
            {
                std::cout << "INFO: Opened output file\n";
            }
            fout << fileheader << "\n";
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
