
//////////////////////////////////////////////////////////////////////
// Usage:
//  > root -l MultipleChannelsNoUncertainties.C
//////////////////////////////////////////////////////////////////////

#include "AtlasStyle.C"
#include "AtlasUtils.C"
#include "Utils.C"

void createASCIIFile(const TString filename, const float Nsig, const float Nbkg, const int Nobs, const float lumi)
{
  Nbkg*=lumi;
  Nsig*=lumi;
  Nobs*=lumi;

  ofstream myfile;
  myfile.open (filename);
  myfile << "+bg Bkg " << Nbkg << " \n";
  myfile << "+sig Sig " << Nsig << " \n";
  myfile << "+data " << Nobs << "\n";

  myfile.close();
}

double asymptoticWoUncertCalcCLs(double mu,double lumi, 
				 const float Nsig1, const float Nbkg1, const float Nobs1,
				 const float Nsig2, const float Nbkg2, const float Nobs2,
				 const float Nsig3, const float Nbkg3, const float Nobs3)
{
  double NsCh1=Nsig1*lumi;
  double NsCh2=Nsig2*lumi;
  double NsCh3=Nsig3*lumi;

  double NbCh1=Nbkg1*lumi;
  double NbCh2=Nbkg2*lumi;
  double NbCh3=Nbkg3*lumi;

  double NoCh1=Nobs1*lumi;
  double NoCh2=Nobs2*lumi;
  double NoCh3=Nobs3*lumi;

  double betaCh1=TMath::Log((mu*NsCh1+NbCh1)/(NbCh1));
  double betaCh2=TMath::Log((mu*NsCh2+NbCh2)/(NbCh2));
  double betaCh3=TMath::Log((mu*NsCh3+NbCh3)/(NbCh3));
  double Neffobs=NoCh1*betaCh1+NoCh2*betaCh2+NoCh3*betaCh3;

  double yieldCh1sb=mu*NsCh1+NbCh1;
  double yieldCh2sb=mu*NsCh2+NbCh2;
  double yieldCh3sb=mu*NsCh3+NbCh3;
  double meansb=betaCh1*yieldCh1sb+betaCh2*yieldCh2sb+betaCh3*yieldCh3sb;

  double meanb=betaCh1*NbCh1+betaCh2*NbCh2+betaCh3*NbCh3;

  double standardDevsb=TMath::Sqrt(betaCh1*betaCh1*yieldCh1sb +
				   betaCh2*betaCh2*yieldCh2sb +
				   betaCh3*betaCh3*yieldCh3sb);

  double standardDevb=TMath::Sqrt(betaCh1*betaCh1*NbCh1 +
				  betaCh2*betaCh2*NbCh2 +
				  betaCh3*betaCh3*NbCh3);

  double CLsb=ROOT::Math::gaussian_cdf(Neffobs,standardDevsb,meansb);
  double CLb=ROOT::Math::gaussian_cdf(Neffobs,standardDevb,meanb);

  if(CLb==0) {
    cout << "ERROR ! CLb=0 -> returning -1" << endl;
    return -1;
  }
  return CLsb/CLb;
}

double computeAsymptoticWoUncert(double lumi, 
				 const float Nsig1, const float Nbkg1, const float Nobs1,
				 const float Nsig2, const float Nbkg2, const float Nobs2,
				 const float Nsig3, const float Nbkg3, const float Nobs3)
{
  double a = 0.;
  double b = 200.;
  double precision = 0.000001;
  double CLs=0;

  while (fabs(CLs-0.05)>precision && (a+b)/2.>0.000000001) {
    double mu=(a+b)/ 2.;
    CLs=asymptoticWoUncertCalcCLs(mu,lumi,Nsig1,Nbkg1,Nobs1,Nsig2,Nbkg2,Nobs2,Nsig3,Nbkg3,Nobs3);
    //std::cout<<a<<"   "<<b<<"   "<<mu<<"  "<<CLs<<std::endl;
    if(CLs > 0.05) a = mu;
    else b = mu;
  }
  
  return mu;
}

void MultipleChannelsNoUncertainties_OTHVsAsymptotic()
{
  setStyle();
  gSystem->Load("OpTHyLiC_C");

  float lumii = 1;
  float lumif = 15;

  int nbins = lumif-lumii+1;
  TH1F* h_LimitAsymptoticWoUncertVsLumi = new TH1F("h_LimitAsymptoticWoUncertVsLumi","h_LimitAsymptoticWoUncertVsLumi",nbins,lumii-0.5,lumif+0.5);
  TH1F* h_LimitOTHVsLumi = new TH1F("h_LimitOTHVsLumi","h_LimitOTHVsLumi",nbins,lumii-0.5,lumif+0.5);
  setHistoStyle(h_LimitOTHVsLumi,4,kBlack,"#mu_{up}","L");
  setHistoStyle(h_LimitAsymptoticWoUncertVsLumi,5,kBlue,"#mu_{up}","L");

  const float Nsig1=5.18;
  const float Nbkg1=2.22;
  const int Nobs1=3;

  const float Nsig2=3.05;
  const float Nbkg2=1.61;
  const int Nobs2=4;

  const float Nsig3=4.45;
  const float Nbkg3=2.95;
  const int Nobs3=2;

  for(float lumi=lumii; lumi<lumif; lumi+=1) {
    cout << endl << "-----------------------------------------------" << endl;
    cout << "-> Processing lumi = " << lumi << endl;
    cout << "-----------------------------------------------" << endl;

    TString fileName1("dat/MultipleChannelsNoUncertainties_OTHVsAsymptoticCh1_"); fileName1+=lumi; fileName1+=".dat";
    createASCIIFile(fileName1,Nsig1,Nbkg1,Nobs1,lumi);

    TString fileName2("dat/MultipleChannelsNoUncertainties_OTHVsAsymptoticCh2_"); fileName2+=lumi; fileName2+=".dat";
    createASCIIFile(fileName2,Nsig2,Nbkg2,Nobs2,lumi);

    TString fileName3("dat/MultipleChannelsNoUncertainties_OTHVsAsymptoticCh3_"); fileName3+=lumi; fileName3+=".dat";
    createASCIIFile(fileName3,Nsig3,Nbkg3,Nobs3,lumi);

    double limitAsymptoticWoUncert = computeAsymptoticWoUncert(lumi,Nsig1,Nbkg1,Nobs1,Nsig2,Nbkg2,Nobs2,Nsig3,Nbkg3,Nobs3);
    double limitOTH = computeObserved(1e6,OpTHyLiC::mclimit,OpTHyLiC::normal,fileName1.Data(),fileName2.Data(),fileName3.Data());
    
    h_LimitAsymptoticWoUncertVsLumi->Fill(lumi,limitAsymptoticWoUncert);
    h_LimitOTHVsLumi->Fill(lumi,limitOTH);
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
  h_LimitOTHVsLumi->GetYaxis()->SetRangeUser(0.15,0.88);
  h_LimitOTHVsLumi->Draw("p");
  h_LimitAsymptoticWoUncertVsLumi->Draw("psame");
  TLegend* leg1 = new TLegend(0.6157718,0.6927803,0.9194631,0.8924731);
  leg1->SetFillColor(kWhite);
  leg1->SetLineColor(kBlack);
  leg1->SetBorderSize(1);
  leg1->AddEntry(h_LimitOTHVsLumi,"OpTHyLiC","p");
  leg1->AddEntry(h_LimitAsymptoticWoUncertVsLumi,"Asymptotic","p");
  leg1->Draw();
  pPad2->cd();
  TH1* hratio = makeRatio(h_LimitAsymptoticWoUncertVsLumi,h_LimitOTHVsLumi,"hratio","ratio");
  hratio->GetYaxis()->SetRangeUser(0.96,1.07);
  hratio->Draw("p");
  system("rm -f results/MultipleChannelsNoUncertainties_OTHVsAsymptotic.*");
  c->SaveAs("results/MultipleChannelsNoUncertainties_OTHVsAsymptotic.png");
  c->SaveAs("results/MultipleChannelsNoUncertainties_OTHVsAsymptotic.C");
  c->SaveAs("results/MultipleChannelsNoUncertainties_OTHVsAsymptotic.pdf");
}

