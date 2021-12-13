// system includes
#include <iomanip>
#include <fstream>
#include <ctime>

// boost includes
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

// root includes
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TH2D.h>
#include <TBox.h>
#include <TEllipse.h>
#include <TLine.h>
#include <TPaveText.h>
#include <TText.h>

#include <NTBMSummary.hh>

class Sharing_file {
public:
  int32_t pl, ecc_id, oss_id, fixedwall_id, trackerwall_id, spotid, zone[2], rawid[2];
  int32_t unix_time, tracker_track_id, babymind_bunch, babymind_nplane, charge, entry_in_daily_file;
  int32_t eventid, track_type, ecc_track_type;
  float chi2_shifter[4], babymind_momentum;
  // spotid:spotA * 100 + spotB
  // chi2_shifter : [0]:ECC-fixedwall [1]:fixedwall-TSS [2]:TSS-tracker
  static bool sort_unix_time(const Sharing_file &lhs, const Sharing_file &rhs) {
    if (lhs.unix_time == rhs.unix_time)return lhs.tracker_track_id < rhs.tracker_track_id;
    return lhs.unix_time < rhs.unix_time;
  }
  static bool sort_eventid(const Sharing_file &lhs, const Sharing_file &rhs) {
    if (lhs.eventid == rhs.eventid)return sort_unix_time(lhs, rhs);
    //lhs.unix_time < rhs.unix_time;
    return lhs.eventid < rhs.eventid;
  }
};

namespace logging = boost::log;

const Double_t offsety[4] = {25. * 5. / 6.,       25. / 6., 25. * 2. / 3.,            0.};
const Double_t offsetx[4] = {            0., 25. * 2. / 3.,      25. / 6., 25. * 5. / 6.};
const Double_t offsetz[4] = { 1.,                       7.,           10.,           16.};

int main (int argc, char *argv[]) {
  
  logging::core::get()->set_filter
    (
     logging::trivial::severity >= logging::trivial::info
    );

  BOOST_LOG_TRIVIAL(info) << "=====Start!=====";

  if (argc != 2) {
    BOOST_LOG_TRIVIAL(info) << "Usage : " << argv[0]
			    << " <input ninjasf file path>";
    std::exit(1);
  }
							     
  try {

    std::ifstream ifs(argv[1]);
    Sharing_file t;

    Int_t num_entry = 0;

    while (ifs >> t.pl >> t.ecc_id >> t.oss_id >> t.fixedwall_id >> t.trackerwall_id
	   >> t.spotid >> t.zone[0] >> t.rawid[0] >> t.zone[1] >> t.rawid[1]
	   >> t.unix_time >> t.tracker_track_id >> t.babymind_bunch >> t.entry_in_daily_file
	   >> t.babymind_nplane >> t.charge >> t.babymind_momentum
	   >> t.chi2_shifter[0] >> t.chi2_shifter[1] >> t.chi2_shifter[2] >> t.chi2_shifter[3]
	   >> t.eventid >> t.track_type >> t.ecc_track_type) {

    //while (ifs.read((char*)& t, sizeof(Sharing_file))) {

      TCanvas *canvas = new TCanvas("monitor", "This is canvas", 500, 1000);
      TPad *pad_side_view = new TPad("pad_side_view", "This is side view", 0.03, 0.23, 0.51, 0.97);
      pad_side_view->Draw();
      pad_side_view->SetNumber(1);
      TPad *pad_top_view = new TPad("pad_top_view", "This is top view", 0.51, 0.23, 0.97, 0.97);
      pad_top_view->Draw();
      pad_top_view->SetNumber(2);
      TPad *pad_text = new TPad("pad_text", "This is text", 0.03, 0.03, 0.97, 0.23);
      pad_text->Draw();
      pad_text->SetNumber(3);
      
      canvas->cd(1)->DrawFrame(0, -600, 20, 600, "Event Display (Side view);z [mm];y [mm]");
      canvas->cd(2)->DrawFrame(20, -600, 40, 600, "Event Display (Top view);z [mm];x [mm]");
      
      TBox *scint = new TBox();
      TEllipse *pred_pos = new TEllipse();
      pred_pos->SetFillColor(kRed);
      TLine *pred_line = new TLine();
      pred_line->SetLineColor(kBlack);
      
      time_t unixtime = (time_t)t.unix_time;
      tm *tm_event = localtime(&unixtime);

      int year = tm_event->tm_year + 1900;
      int month = tm_event->tm_mon + 1;
      int day = tm_event->tm_mday;
      BOOST_LOG_TRIVIAL(debug) << year << "-" << month << "-" << day;
      if (t.eventid == -1) continue;

      TString ntbmfilename = Form("/home/t2k/odagawa/data/trackmatch/track_b2physics_wagasci_babymind_2021_mucl_spline_update_ninja_nodead_ninjamatch_%d_%d_%d.root",
				  year, month, day);
      TFile *ntbmfile = new TFile(ntbmfilename, "read");
      TTree *ntbmtree = (TTree*)ntbmfile->Get("tree");
      NTBMSummary *ntbm = nullptr;
      ntbmtree->SetBranchAddress("NTBMSummary", &ntbm);
      
      ntbmtree->GetEntry(t.entry_in_daily_file - 1);

      // Draw scintillators
      for ( Int_t iview = 0; iview < 2; iview++ ) {
	for ( Int_t iplane = 0; iplane < 4; iplane++ ) {
	  for ( Int_t islot = 0; islot < 31; islot++ ) {
	    if ( ( iview == 0 && iplane == 2 && islot == 30 ) ||
		 ( iview == 1 && iplane == 1 && islot == 0  ) ||
		 ( iview == 1 && iplane == 1 && islot == 1  ) ||
		 ( iview == 1 && iplane == 1 && islot == 7  ) ||
		 ( iview == 1 && iplane == 2 && islot == 20 ) ) {
	      scint->SetFillColor(kBlack);
	    }
	    else scint->SetFillColor(19);	  
	    switch (iview) {
	    case 0 :
	      canvas->cd(1);
	      scint->DrawBox(offsetz[iplane],
			     -448. + 25. * 4. / 3. * islot + offsety[iplane],
			     offsetz[iplane] + 3.,
			     -448. + 25. * 4. / 3. * islot + 24 + offsety[iplane]);
	      break;
	    case 1 :
	      canvas->cd(2);
	      scint->DrawBox(offsetz[iplane] + 20,
			     448. - 25. * 4. / 3. * islot - offsetx[iplane],
			     offsetz[iplane] + 23.,
			     448. - 25. * 4. / 3. * islot - 24 - offsetx[iplane]);
	      break;
	    }
	  }
	}
      }

      // Draw hit histograms
      for ( Int_t icluster = 0; icluster < ntbm->GetNumberOfNinjaClusters(); icluster++ ) {
	if ( ntbm->GetBabyMindTrackId(icluster) != t.tracker_track_id ) continue;
	TH2D *hist_side_hits = new TH2D("hist_side_hits", "", 20, 0, 20, 7200, -600, 600);
	hist_side_hits->GetZaxis()->SetRangeUser(0, 50);
	TH2D *hist_top_hits = new TH2D("hist_top_hits", "", 20, 20, 40, 7200, -600, 600);
	hist_top_hits->GetZaxis()->SetRangeUser(0, 50);
	for ( Int_t iview = 0; iview < 2; iview++ ) {
	  for ( Int_t ihit = 0; ihit < ntbm->GetNumberOfHits(icluster, iview); ihit++ ) {
	    switch (iview) {
	    case 0 :
	      for ( Int_t z = 0; z < 3; z++ ) {
		for ( Int_t y = 0; y < 150; y++ ) {
		  hist_side_hits->Fill(offsetz[ntbm->GetPlane(icluster, iview, ihit)] + 1. * z,
				       -448 + 25. * 4. / 3. * ntbm->GetSlot(icluster, iview, ihit)
				       + offsety[ntbm->GetPlane(icluster, iview, ihit)]
				       + 1. / 6. * (y + 0.5),
				       ntbm->GetPe(icluster, iview, ihit));
		}
	      }
	      canvas->cd(1);
	      hist_side_hits->Draw("same colz");
	      pred_pos->DrawEllipse(10., ntbm->GetNinjaPosition(icluster, iview), 1., 10., 0., 360., 0.);
	      pred_line->DrawLine(0,
				  ntbm->GetNinjaPosition(icluster, iview) - ntbm->GetNinjaTangent(icluster, iview) * 10.,
				  20,
				  ntbm->GetNinjaPosition(icluster, iview) + ntbm->GetNinjaTangent(icluster, iview) * 10.);
	      break;
	    case 1 :
	      for ( Int_t z = 0; z < 3; z++ ) {
		for ( Int_t x = 0; x < 150; x++ ) {
		  hist_top_hits->Fill(20 + offsetz[ntbm->GetPlane(icluster, iview, ihit)] + 1. * z,
				      448 - 25. * 4. / 3. * ntbm->GetSlot(icluster, iview, ihit)
				      - offsetx[ntbm->GetPlane(icluster, iview, ihit)]
				      - 1. / 6. * (x + 1 + 0.5),
				      ntbm->GetPe(icluster, iview, ihit));
		}		
	      }
	      canvas->cd(2);
	      hist_top_hits->Draw("same colz");
	      pred_pos->DrawEllipse(30., ntbm->GetNinjaPosition(icluster, iview), 1., 10., 0., 360., 0.);
	      pred_line->DrawLine(20,
				  ntbm->GetNinjaPosition(icluster, iview) - ntbm->GetNinjaTangent(icluster, iview) * 10.,
				  40,
				  ntbm->GetNinjaPosition(icluster, iview) + ntbm->GetNinjaTangent(icluster, iview) * 10.);
	      
	      break;
	    }
	  }
	}

	break;
      }

      // Draw texts
      canvas->cd(3);
      TPaveText *pave_text = new TPaveText(0.03, 0.03, 0.97, 0.97, "brNDC");
      pave_text->SetFillStyle(0);
      pave_text->SetBorderSize(0);
      TText *text;
      // unixtime
      text = pave_text->AddText(Form("Timestamp : %d", t.unix_time));
      text = pave_text->AddText(Form("Date : %d/%d/%d", year, month, day));
      text = pave_text->AddText(Form("Event ID : %d", t.eventid));
      text = pave_text->AddText(Form("TSS-NT #chi^{2} : %.1f", t.chi2_shifter[3]));
      pave_text->Draw();

      canvas->SaveAs(Form("/home/t2k/odagawa/data/plots/tracker_display_%d.pdf", t.eventid));

      // if (num_entry > 2)  break;
      num_entry++;
    }

  } catch (const std::runtime_error &error) {
    BOOST_LOG_TRIVIAL(fatal) << "Runtime error : " << error.what();
    std::exit(1);
  } catch (const std::invalid_argument &error) {
    BOOST_LOG_TRIVIAL(fatal) << "Invalid argument : " << error.what();
    std::exit(1);
  }

  BOOST_LOG_TRIVIAL(info) << "=====Finish!=====";
  std::exit(0);
}
