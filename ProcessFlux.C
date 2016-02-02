/*
 * This macro file will process a flux vector of a single neutrino flavor
 * in CSV format through the approximate matrix-based procedure to get
 * reconstructed spectra for the given neutrino flavor to oscillate to
 * all 3 flavors.
 */
#include "NuIndex2str.C"
#include "csv2array.C"
const size_t NUM_FLAVORS = 3;

/*
 * This macro converts an initial flux into 3 oscillated fluxes, one for
 * each flavor.
 *
 * If no outfile is given, the default of tmp.csv will be used.
 *
 * The input format is a CSV file (separators are , and newline) with
 * NBINS entries denoting the beam flux of neutrino flavor STARTNU (1=e,
 * 2=mu, 3=tau, negative = bar), in energy bins running from EMIN to
 * EMAX.
 *
 * The output format is a CSV file with NUM_FLAVORS*NBINS (NUM_FLAVORS =
 * 3, currently) entries denoting the oscillated flux into e, nu and tau
 * for the first, second and third NBINS entries. (e.g. if NBINS = 40,
 * the first 40 entries give the flux for oscillations to nue(bar), the
 * second 40 (i.e. 40-79) give the flux for oscillations to numu(bar),
 * and so on.
 */
int Flux2OscFlux(const int STARTNU, const size_t NBINS,
        const double EMIN, const double EMAX,
        std::string infile, std::string outfile = "tmp.csv")
{
    double influx[NBINS];
    std::string influxstr[NBINS];
    double outflux[NUM_FLAVORS][NBINS];
    double probs[NUM_FLAVORS][NBINS];
    std::string probsstr[NUM_FLAVORS][NBINS];
    int result = csv2array(infile, influxstr, NBINS);
    if(result != 0)
    {
        std::cout << "ERROR: Result != 0: " << result << "\n";
        return result;
    }
    else
    {
        std::cout << "INFO: Correctly read in csv file.\n";
    }
    // Convert the string array into a double array
    double inbin = 0;
    for(size_t i = 0; i < NBINS; ++i)
    {
        inbin = atof(influxstr[i].c_str());
        influx[i] = inbin;
    }

    // Now we have the input flux.
    // We need to process the flux according to the oscillation
    // probabilities, so the next step is to read in the oscillation
    // probabilities.
    int nusign = STARTNU > 0 ? 1 : -1;
    char filenameend[10];
    sprintf(filenameend, "%d.csv", NBINS);
    std::string startnustr;
    std::string endnustr[NUM_FLAVORS];
    std::string finstr[NUM_FLAVORS];

    NuIndex2str(STARTNU, startnustr);
    for(size_t i = 0; i < NUM_FLAVORS; ++i)
    {
        NuIndex2str((i + 1) * nusign, endnustr[i]);
        finstr[i] = std::string("/afs/fnal.gov/files/home/room3/") +
            "skohn/outputs/oscvectors/" + startnustr +
            "_" + endnustr[i] + filenameend;
        // Read in the probabilities
        result = csv2array(finstr[i], probsstr[i], NBINS);
        if(result != 0)
        {
            return result;
        }
        for(size_t bin = 0; bin < NBINS; ++bin)
        {
            double probbin = atof(probsstr[i][bin].c_str());
            probs[i][bin] = probbin;
        }
    }

    // Now perform the multiplication to get the oscillated spectra
    std::ofstream fout;
    fout.open(outfile.c_str());
    if(!fout.is_open())
    {
        std::cout << "Error: Could not open file\n";
        return 2;
    }
    else
    {
        std::cout << "INFO: Opened output file\n";
    }
    for(size_t flavor = 0; flavor < NUM_FLAVORS; ++flavor)
    {
        for(size_t bin = 0; bin < NBINS; ++bin)
        {
            outflux[flavor][bin] = influx[bin] * probs[flavor][bin];
            fout << outflux[flavor][bin] << "\n";
        }
    }
    fout.close();
    return 0;
}

/*
 * This macro converts an oscillated neutrino flux (e, mu and tau) into
 * true interaction spectra by using the computed charged-current cross
 * sections.
 *
 * If no infile or outfile is given, the default of tmp.csv will be
 * used and the input will be overwritten. If an infile is given but no
 * outfile, the default of tmp.csv will be used for the outfile and the
 * input will be preserved.
 *
 * The input format is the same as the output format of Flux2OscFlux.
 * The output format is analogous to the input format, except the
 * entries represent numbers of interactions rather than fluxes.
 */
int OscFlux2TrueSpectrum(const size_t NBINS, const double EMIN,
        const double EMAX, std::string infile = "tmp.csv",
        std::string outfile = "tmp.csv")
{
    double influx[NUM_FLAVORS][NBINS];
    double influxstr[NUM_FLAVORS * NBINS];
    double outspec[NUM_FLAVORS][NBINS];
    double xsec[NUM_FLAVORS][NBINS];
    int result = csv2array(infile, influxstr, NUM_FLAVORS * NBINS);
    if(result != 0)
    {
        std::cout << "ERROR: Result != 0: " << result << "\n";
        return result;
    }
    else
    {
        std::cout << "INFO: Correctly read in csv file.\n";
    }

    // Convert the 3N*1 string array into an N*3 double array
    double value = 0;
    size_t flavor = 0;
    size_t ebin = 0;
    for(size_t bin = 0; bin < NUM_FLAVORS * NBINS; ++bin)
    {
        flavor = bin/NBINS; // Integer division
        ebin = bin % NBINS;
        value = atof(influxstr[bin].c_str());
        influx[flavor][ebin] = value;
    }

    // Read in cross sections
}
