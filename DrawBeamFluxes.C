int DrawBeamFluxes()
{
    std::vector<std::string> filenames;
    filenames.push_back("nuflux_numuflux_numu");
    filenames.push_back("nuflux_nueflux_nue");
    filenames.push_back("nuflux_numubarflux_numubar");
    filenames.push_back("nuflux_nuebarflux_nuebar");
    filenames.push_back("nuflux_numuflux_nue");
    filenames.push_back("nuflux_numubarflux_nuebar");
    filenames.push_back("nuflux_numuflux_nutau");
    filenames.push_back("nuflux_numubarflux_nutaubar");
    filenames.push_back("anuflux_numuflux_numu");
    filenames.push_back("anuflux_nueflux_nue");
    filenames.push_back("anuflux_numubarflux_numubar");
    filenames.push_back("anuflux_nuebarflux_nuebar");
    filenames.push_back("anuflux_numuflux_nue");
    filenames.push_back("anuflux_numubarflux_nuebar");
    filenames.push_back("anuflux_numuflux_nutau");
    filenames.push_back("anuflux_numubarflux_nutaubar");
    std::string prefix = "/dune/data/users/lblpwg_tools/FastMC_Data/outputs/cherdack/v3r2p4b/nominal";
    prefix.append("/fastmcNtp_20160105_lbne_g4lbnev3r2p4b_");
    std::string suffix = "_LAr_1_g280_Ar40_5000_GENIE_2100_Test.root";
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
        TTree* fluxData = (TTree*) fin->Get("gst");
        TCanvas* c1 = new TCanvas();
        TH1D* spectrum = new TH1D(temp.c_str(), temp.c_str(), 360, 0, 120); // GeV
        fluxData->Draw((std::string("Ev>>") + temp).c_str());

        spectrum->GetXaxis()->SetTitle("E_{#nu} [GeV]");
        spectrum->GetYaxis()->SetTitle("Spectrum [a.u.]");
        c1->Print((std::string("~/outputs/") + temp + ".pdf").c_str());
        fin->Close();
    }
}
