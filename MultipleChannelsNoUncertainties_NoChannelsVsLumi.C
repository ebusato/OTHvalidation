//////////////////////////////////////////////////////////////////////
// Usage:
//  > root -l StudyNoChannelsVsLumi.C
//////////////////////////////////////////////////////////////////////

#include "AtlasStyle.C"
#include "AtlasUtils.C"
#include "Utils.C"

void createASCIIFile(const TString fileName, const float Nbkg, const float Nsig, const float Nobs)
{
  ofstream myfile;
  myfile.open (fileName);
  myfile << "+bg Bkg " << Nbkg << " \n\n";
  myfile << "+sig Sig " << Nsig << " \n\n";
  myfile << "+data " << Nobs << "\n\n";
  myfile.close();
}

double computeObservedLocal(const int Nexp, const TString& fileName, const unsigned int N=1) 
{
  OpTHyLiC oth(OpTHyLiC::mclimit,OpTHyLiC::normal);
  for(unsigned int i=0; i<N; ++i) {
    TString chName("channel_"); chName+=i;
    oth.addChannel(chName.Data(),fileName.Data());
  }
  double cls;
  double limit = oth.observedSigStrengthFor95excl(Nexp,cls);
  return limit;
}


void MultipleChannelsNoUncertainties_NoChannelsVsLumi()
{
  setStyle();
  gSystem->Load("OpTHyLiC_C");

  float lumii = 1;
  float lumif = 10;

  int nbins = lumif-lumii+1;
  TH1F* h_LimitLumi = new TH1F("h_LimitLumiOTH","h_LimitLumi",nbins,lumii-0.5,lumif+0.5);
  TH1F* h_LimitNoChannels = new TH1F("h_LimitNoChannels","h_LimitNoChannels",nbins,lumii-0.5,lumif+0.5);
  setHistoStyle(h_LimitLumi,25,kBlack,"#mu_{up} (obs.)","N");
  setHistoStyle(h_LimitNoChannels,30,kRed,"#mu_{up} (obs.)","N");

  if(lumii!=1) {
    cout << "ERROR ! lumii is not 1 -> program not made for such a case -> stopping" << endl;
    return;
  }

  TString fileNameFirst("");
  for(float lumi=lumii; lumi<lumif; lumi+=1) {
    cout << endl << "-----------------------------------------------" << endl;
    cout << "-> Processing lumi = " << lumi << endl;
    cout << "-----------------------------------------------" << endl;

    float Nbkg2=0.82*lumi;
    float Nsig2=2.49*lumi;
    float Nobs2=1*lumi;
    
    TString fileName("dat/MultipleChannelsNoUncertainties_NoChannelsVsLumi_"); fileName+=lumi; fileName+=".dat";
    createASCIIFile(fileName,Nbkg2,Nsig2,Nobs2);
    if(1==lumi) 
      fileNameFirst=fileName;
    
    double limit_Lumi = computeObservedLocal(1e6,fileName);
    double limit_NoChannels = computeObservedLocal(1e6,fileNameFirst,lumi);
    
    h_LimitLumi->Fill(lumi,limit_Lumi);
    h_LimitNoChannels->Fill(lumi,limit_NoChannels);
  }
  
  TCanvas* c = new TCanvas("c","c",600,400);
  TPad* pPad1=new TPad("p1","",0.00,0.30,1.00,1.00);
  TPad* pPad2=new TPad("p2","",0.00,0.00,1.00,0.30);
  pPad1->SetBottomMargin(0.);
  pPad1->Draw();  
  pPad2->SetTopMargin(0);
  pPad2->SetBottomMargin(0.3);
  pPad2->Draw();
  pPad1->cd();
  h_LimitLumi->GetYaxis()->SetRangeUser(0.1,1.8);
  h_LimitLumi->Draw("p");
  h_LimitNoChannels->Draw("psame");
  TLegend* leg1 = new TLegend(0.6,0.6,0.92,0.89);
  leg1->SetFillColor(kWhite);
  leg1->SetLineColor(kBlack);
  leg1->SetBorderSize(1);
  leg1->AddEntry(h_LimitLumi,"Lumi increase","p");
  leg1->AddEntry(h_LimitNoChannels,"No channel increase","p");
  leg1->Draw();
  pPad2->cd();
  TH1* hratio = makeRatio(h_LimitNoChannels,h_LimitLumi,"hratio","ratio");
  hratio->GetYaxis()->SetRangeUser(0.98,1.025);
  hratio->Draw("p");
  system("rm -f results/MultipleChannelsNoUncertainties_NoChannelsVsLumi.*");
  c->SaveAs("results/MultipleChannelsNoUncertainties_NoChannelsVsLumi.png");
  c->SaveAs("results/MultipleChannelsNoUncertainties_NoChannelsVsLumi.C");
  c->SaveAs("results/MultipleChannelsNoUncertainties_NoChannelsVsLumi.pdf");
  
}

