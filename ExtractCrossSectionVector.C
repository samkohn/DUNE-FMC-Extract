/*
 * Extract the neutrino cross sections from the GENIE cross section
 * file. This file was generated using gspl2root utility.
 */
// The two includes sys/stat.h and unistd.h are for utiliities that
// check whether a file exists (the "stat buffer" thing).
#include <sys/stat.h>
#include <unistd.h>
int ExtractCrossSectionVector(std::string interaction_class, std::string xsec_type)
{
    std::string inputfile = "gxspl-big.root";
    std::string inputfiledir = "/dune/app/users/lblpwg_tools/";
    inputfiledir += "SOFTWARE/genie-2.10.0-20151103/splines/";
    //std::string interaction_class = "nu_e_Ar40";
    //std::string xsec_type = "tot_nc";
    const double MINE = 0; //GeV
    const double MAXE = 10;
    const int EBINS = 40;
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
    outputfile.open((std::string("/afs/fnal.gov/files/home/room3/") + 
                "skohn/outputs/cross-sections/" + interaction_class +
                "__" + xsec_type + Form("%d.csv", EBINS)).c_str());
    if(!outputfile.is_open())
    {
        std::cout << "ERROR: Could not open file\n";
        return 3;
    }
    else
    {
        std::cout << "INFO: Opened output file\n";
    }
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
    std::string energylistname = std::string("/afs/fnal.gov/files/home/room3/") +
                "skohn/outputs/cross-sections/" +
                Form("energies%d.csv", EBINS);
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