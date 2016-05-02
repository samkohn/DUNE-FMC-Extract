/*
 * Extract the neutrino cross sections from the GENIE cross section
 * file. This file was generated using gspl2root utility.
 */
// The two includes sys/stat.h and unistd.h are for utiliities that
// check whether a file exists (the "stat buffer" thing).
#include <sys/stat.h>
#include <unistd.h>
#include "Configuration.C"
int ExtractAllCrossSections(const int EBINS, const double MINE, const double MAXE)
{
    std::vector<std::string> interaction_classes;
    std::vector<std::string> xsec_types;
    interaction_classes.push_back("nu_e_Ar40");
    interaction_classes.push_back("nu_e_bar_Ar40");
    interaction_classes.push_back("nu_mu_Ar40");
    interaction_classes.push_back("nu_mu_bar_Ar40");
    interaction_classes.push_back("nu_tau_Ar40");
    interaction_classes.push_back("nu_tau_bar_Ar40");
    xsec_types.push_back("tot_nc");
    xsec_types.push_back("tot_cc");
    for(size_t i = 0; i < interaction_classes.size(); ++i)
    {
        for(size_t j = 0; j < xsec_types.size(); ++j)
        {
            ExtractCrossSectionVector(EBINS, MINE, MAXE, interaction_classes.at(i),
                    xsec_types.at(j));
        }
    }
    return 0;

}
int ExtractCrossSectionVector(const int EBINS, const double MINE, const double MAXE,
        std::string interaction_class, std::string xsec_type)
{
    std::string inputfile = "gxspl-big.root";
    std::string inputfiledir = "/dune/app/users/lblpwg_tools/";
    inputfiledir += "SOFTWARE/genie-2.10.0-20151103/splines/";
    std::string outputheader = "# Source: GENIE 2.10.0 splines\n";
    //std::string interaction_class = "nu_e_Ar40";
    //std::string xsec_type = "tot_nc";
    /*
    const double MINE = 0; //GeV
    const double MAXE = 10;
    const int EBINS = 120;
    */
    const double ESTEP = (MAXE - MINE)/EBINS;
    TFile* fin = TFile::Open((inputfiledir + inputfile).c_str(), "READ");
    if(!fin)
    {
        std::cout << "ERROR: Could not open file." <<std::endl;
        return 1;
    }
    else
    {
        std::cout << "INFO: Opened file." << std::endl;
    }

    // Fetch TGraph containing the desired data
    TGraph* xsecgraph = 0;
    xsecgraph = (TGraph*) fin->Get((interaction_class + "/" + xsec_type).c_str());
    if(!xsecgraph)
    {
        std::cout << "ERROR: Could not find the desired graph." <<std::endl;
        return 2;
    }
    else
    {
        std::cout << "INFO: Found desired graph." << std::endl;
    }
    std::ofstream outputfile;
    char outputdir[100];
    outputfile.open((std::string(CFG_OutputDirectory(outputdir)) +
                "120/cross-sections/" + interaction_class + "__" +
                xsec_type + Form("%d.csv", EBINS)).c_str());
    if(!outputfile.is_open())
    {
        std::cout << "ERROR: Could not open file\n";
        return 3;
    }
    else
    {
        std::cout << "INFO: Opened output file\n";
    }
    outputfile << outputheader;
    double energy = MINE + ESTEP/2;
    size_t nentry = 0;
    double value = xsecgraph->Eval(energy);
    outputfile << value;
    ++nentry;
    while(nentry < EBINS)
    {
        energy += ESTEP;
        value = xsecgraph->Eval(energy);
        outputfile << ", " << value;
        ++nentry;
    }
    outputfile.close();
    // Check to see if the energy list exists
    std::string energylistname = std::string(outputdir)
        + "120/cross-sections/" + Form("energies%d.csv", EBINS);
    struct stat buffer;
    if(stat (energylistname.c_str(), &buffer) == 0)
    {
        std::cout << "INFO: Energy file already exists." << std::endl;
        return 0;
    }
    else
    {
        std::cout << "INFO: Creating energy file." << std::endl;
        outputfile.open(energylistname.c_str());
        energy = MINE+ ESTEP/2;
        outputfile << energy;
        energy += ESTEP;
        while(energy < MAXE)
        {
            outputfile << ", " << energy;
            energy += ESTEP;
        }
        outputfile.close();
    }
    return 0;
}
