
  Very simple FASTBUS decoder example for CDet test stand
     original author: R. Michaels   Oct 24, 2014
     modifications:  E. Brash January, 2017

  This file:  README

  Main code is  

       Fastbus_main1.C

  Support code is all the other *.C and *.h files

  To compile type "make"

  Then run the executible

         ./fbana <runno> <rocno> <nevents>

         The arguments here are:
	 <runno> CODA run number
	 <rocno> ROC number = 14
         <nevents>  Number of events to analyze

  The output is a root file with some histograms.  

         sbs_<runno>_<rocno>.root

  If you turn on the "debug" flag in the main code, it will
  produce a bunch of output.

ANALYZING WITH ROOT

	.x init.C(<runno>)

LIST OF PLOT FUNCTIONS in init.C

plot_adc(Int_t pmt=1, Int_t tdc_min=1300, Int_t tdc_width=200)

// Plots pedastel subtracted ADC spectra for a given PMT, with
// and without good TDC cut

plot_tdc_hits(Int_t pmt=1, Int_t adc_cut=40)

// Plots TDC spectra for a given PMT, with and without ADC cut

plot_tdc_hits(Int_t pmt=1, Int_t adc_cut=40)

// Plots spectra of the number of hits recorded on all paddles of
// a given PMT, with and without ADC cut.  Note that both leading
// and trailing edge times constitute a hit.

plot_ratio(Int_t pmt=1, Int_t tdc_min=1300, Int_t tdc_width=200)

// For a given ratio, calulate the ratio of ADC with TDC cut to ADC
// without TDC cut, and plot that ratio for each paddle.  This ratio
// is then fit with an appropriate step function (1/2 - erfc(x))

plot_occupancy(Int_t adc_cut=40, Int_t multiplicity_cut = 12, Int_t tdc_min=1300, Int_t tdc_width=200)

// Plots occupancy (subject to the specified ADC, TDC,
// and multiplcity cuts), and multiplicity histograms.
 

