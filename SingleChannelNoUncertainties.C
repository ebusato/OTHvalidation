//////////////////////////////////////////////////////////////////////
// Usage:
//  > root -l SingleChannelNoUncertainties.C
//////////////////////////////////////////////////////////////////////

#include "AtlasStyle.C"
#include "AtlasUtils.C"
#include "Utils.C"

void createASCIIFile(const TString filename, const float Nbkg, const float Nsig, const float Nobs)
{
  ofstream myfile;
  myfile.open (filename);
  myfile << "+bg Bkg " << Nbkg << "\n";
  myfile << "+sig Sig " << Nsig << "\n";
  myfile << "+data " << Nobs << "\n";
  myfile.close();
}

double theoPoisson(const float Nbkg, const float Nsig, const float Nobs, const double confLevel)
{
  return (0.5*ROOT::Math::chisquared_quantile(1-(1-confLevel)*(1-ROOT::Math::chisquared_cdf(2*Nbkg,2*(Nobs+1))),2*(Nobs+1))-Nbkg)/Nsig;
}

void SingleChannelNoUncertainties(const double confLevel=0.95)
{
  setStyle();
  gSystem->Load("OpTHyLiC_C");

  float lumii = 1;
  float lumif = 8;

  int nbins = lumif-lumii+1;
  TH1F* h_LimitVsLumi = new TH1F("h_LimitVsLumi","h_LimitVsLumi",nbins,lumii-0.5,lumif+0.5);
  TH1F* h_LimitTheoPoissonVsLumi = new TH1F("h_LimitTheoPoissonVsLumi","h_LimitTheoPoissonVsLumi",nbins,lumii-0.5,lumif+0.5);
  
  setHistoStyle(h_LimitVsLumi,4,kBlack,"#mu_{up}","L");
  setHistoStyle(h_LimitTheoPoissonVsLumi,5,kBlue,"#mu_{up}","L");

  for(float lumi=lumii; lumi<lumif; lumi+=1) {
    cout << endl << "-----------------------------------------------" << endl;
    cout << "-> Processing lumi = " << lumi << endl;
    cout << "-----------------------------------------------" << endl;
    TString fileName("dat/SingleChannelNoUncertainties_"); fileName+=lumi; fileName+=".dat";
    float Nbkg=0.82*lumi;
    float Nsig=2.49*lumi;
    float Nobs=1*lumi;
    createASCIIFile(fileName,Nbkg,Nsig,Nobs);
    double limit = computeObserved(confLevel,1e6,OTH::SystMclimit,OTH::StatNormal,fileName.Data());
    double limit_theoPoisson = theoPoisson(Nbkg,Nsig,Nobs,confLevel);
    h_LimitVsLumi->Fill(lumi,limit);
    h_LimitTheoPoissonVsLumi->Fill(lumi,limit_theoPoisson);
  }

  TCanvas* c = new TCanvas("c","c",600,400);
  TPad* pPad1bis=new TPad("p1bis","",0.00,0.30,1.00,1.00);
  TPad* pPad2bis=new TPad("p2bis","",0.00,0.00,1.00,0.30);
  pPad1bis->SetBottomMargin(0.);
  pPad1bis->Draw();  
  pPad2bis->SetTopMargin(0);
  pPad2bis->SetBottomMargin(0.3);
  pPad2bis->Draw();
  pPad1bis->cd();
  h_LimitVsLumi->GetYaxis()->SetRangeUser(0.1,1.85);
  h_LimitVsLumi->Draw("phist");
  h_LimitTheoPoissonVsLumi->Draw("phistsame");
  TLegend* leg1bis = new TLegend(0.6526846,0.7273425,0.9060403,0.9039939);
  leg1bis->SetFillColor(kWhite);
  leg1bis->SetLineColor(kBlack);
  leg1bis->SetBorderSize(1);
  leg1bis->AddEntry(h_LimitVsLumi,"OpTHyLiC","p");
  leg1bis->AddEntry(h_LimitTheoPoissonVsLumi,"Analytical","p");
  leg1bis->Draw();
  pPad2bis->cd();
  TH1* hratio = makeRatio(h_LimitTheoPoissonVsLumi,h_LimitVsLumi,"hratio","ratio");
  hratio->Draw("phist");
  hratio->GetYaxis()->SetRangeUser(0.975,1.025);
  system("rm -f results/SingleChannelNoUncertainties.*");
  c->SaveAs("results/SingleChannelNoUncertainties.png");
  c->SaveAs("results/SingleChannelNoUncertainties.C");
  c->SaveAs("results/SingleChannelNoUncertainties.pdf");
}

