#include "Configuration.C"

int ExtractBeamFluxes(const int NBINS, const double EMIN,
        const double EMAX)
{
    std::vector<std::string> filenames;
    filenames.push_back("nuflux_numuflux_numu");
    filenames.push_back("nuflux_nueflux_nue");
    filenames.push_back("nuflux_numubarflux_numubar");
    filenames.push_back("nuflux_nuebarflux_nuebar");
    filenames.push_back("anuflux_numuflux_numu");
    filenames.push_back("anuflux_nueflux_nue");
    filenames.push_back("anuflux_numubarflux_numubar");
    filenames.push_back("anuflux_nuebarflux_nuebar");
    char outputdir[100];
    std::string prefix = std::string(CFG_OutputDirectory(outputdir)) +
        "oscprob/";
    std::string suffix = "__OSCPROB.root";
    std::vector<std::string>::iterator it = filenames.begin();
    for(it; it != filenames.end(); ++it)
    {
        // Create the correct file name
        std::string temp = *it;
        it->assign(prefix);
        it->append(temp);
        it->append(suffix);
        std::cout << *it << std::endl;
        std::string& filename = *it;

        // Open the file
        TFile* fin = TFile::Open(filename.c_str(), "READ");
        if(!fin)
        {
            std::cout << "ERROR: ROOT file does not exist.\n";
            return 1;
        }
        TTree* fluxData = (TTree*) fin->Get("OSCPROB");
        TCanvas* c1 = new TCanvas();
        TH1D* spectrum = new TH1D(temp.c_str(), temp.c_str(),
                NBINS, EMIN, EMAX);
        // Draw the energy spectrum of the unoscillated beam neutrinos
        fluxData->Draw((std::string("Ev>>") + temp).c_str());
        spectrum->GetXaxis()->SetTitle("E_{#nu} [GeV]");
        spectrum->GetYaxis()->SetTitle("Spectrum [a.u.]");

        // Dump the results to a CSV file
        std::ofstream outputfile;
        outputfile.open((std::string("/afs/fnal.gov/files/home/room3/") + 
                    "skohn/outputs/beam-flux/" + temp +
                    Form("%d.csv", NBINS)).c_str());
        if(!outputfile.is_open())
        {
            std::cout << "ERROR: Could not open file\n";
            return 2;
        }
        else
        {
            std::cout << "INFO: Opened output file\n";
        }
        for(size_t ebin = 1; ebin <= NBINS; ++ebin)
        {
            double value = spectrum->GetBinContent(ebin);
            outputfile << value << "\n";
        }
        outputfile.close();
        fin->Close();
    }
    return 0;
}
