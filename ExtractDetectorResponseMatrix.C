#include <fstream>
#include "Configuration.C"
int ExtractDetectorResponseMatrix(const int NBINSSQUARE)
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
        std::string fluxtype = *it;
        it->assign(prefix);
        it->append(fluxtype);
        it->append(suffix);
        std::cout << *it << std::endl;
        std::string& filename = *it;

        // Open the file
        TFile* fin = TFile::Open(filename.c_str(), "READ");
        TTree* fluxData = (TTree*) fin->Get("gst");
        TCanvas* c1 = new TCanvas();
        //const int NBINSSQUARE = 20;
        const double MINSQUARE = 0;
        const double MAXSQUARE = 10;
        const int XBINS = NBINSSQUARE;
        const int YBINS = NBINSSQUARE;
        const int XMIN = MINSQUARE;
        const int XMAX = MAXSQUARE;
        const int YMIN = MINSQUARE;
        const int YMAX = MAXSQUARE;
        TH2D* enuresponse = new TH2D(fluxtype.c_str(), fluxtype.c_str(), XBINS, XMIN, XMAX,
                YBINS, YMIN, YMAX);
        fluxData->Draw((std::string("Ev_reco:Ev>>") + fluxtype).c_str(), "", "colz");
        enuresponse->GetXaxis()->SetTitle("E_{#nu} [GeV]");
        enuresponse->GetYaxis()->SetTitle("E_{#nu, reco} [GeV]");
        double nentries = enuresponse->Integral();
        enuresponse->Scale(1.0/nentries); // Normalize
        //c1->Print((std::string("~/outputs/detector-response/") + fluxtype + ".pdf").c_str());
        // Normalize the columns of the histogram individually to get
        // the matrix
        double values[YBINS];
        for(int column = 1; column <= XBINS; ++column)
        {
            double sum = 0;
            for(int row = 1; row <= YBINS; ++row)
            {
                values[row-1] = enuresponse->GetBinContent(column, row);
                sum += values[row-1];
            }
            if(TMath::Abs(sum - 0) < 0.000001)
            {
                // Then all of the bins have 0 content so leave them
                // alone
            }
            else
            {
                for(int row = 1; row <= YBINS; ++row)
                {
                    enuresponse->SetBinContent(column, row, values[row-1]/sum);
                }
            }
        }
        // Print out the matrix information
        std::ofstream outputfile;
        char outputdir[100];
        outputfile.open((std::string(CFG_OutputDirectory(outputdir)) +
                    "detector-response/" + fluxtype +
                    Form("%d.csv", NBINSSQUARE)).c_str());
        if(!outputfile.is_open())
        {
            std::cout << "ERROR\n";
            return 0;
        }
        for(int row = 1; row <= YBINS; ++row)
        {
            for(int column = 1; column <= XBINS; ++column)
            {
                outputfile << (enuresponse->GetBinContent(column, row));
                if(column != XBINS)
                {
                    outputfile << ", ";
                }
                else
                {
                    outputfile << "\n";
                }
            }
        }
        outputfile.close();
        fin->Close();
    }

    return 0;
}
