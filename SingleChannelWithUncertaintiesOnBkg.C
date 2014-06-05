//////////////////////////////////////////////////////////////////////
// Usage:
//  > root -l SingleChannelWithUncertaintiesOnBkg.C
//////////////////////////////////////////////////////////////////////

#include "AtlasStyle.C"
#include "AtlasUtils.C"
#include "Utils.C"

void createASCIIFile(const bool withUncertainties, const TString filename, const float Nbkg2, const float Nsig2, const float Nobs2)
{
  ofstream myfile;
  myfile.open (filename);

  myfile << "+bg Bkg1 " << Nbkg2/4.;
  if(withUncertainties) {
    myfile << " 7.\n";
    myfile << ".syst Syst1 0.1 -0.3" << "\n";
    myfile << ".syst Syst2 0.3 -0.2" << "\n";
  }
  else myfile << "\n";
  myfile << "+bg Bkg2 " << Nbkg2/4;
  if(withUncertainties) {
    myfile << " 12\n";
    myfile << ".syst Syst1 0.2 -0.05" << "\n";
    myfile << ".syst Syst3 -0.06 0.15" << "\n";
  }
  else 
    myfile << "\n";
  myfile << "+bg Bkg3 " << Nbkg2/3.;
  if(withUncertainties) {
    myfile << " 3.5\n";
    myfile << ".syst Syst2 -0.1 0.3" << "\n";
    myfile << ".syst Syst3 0.15 -0.15" << "\n";
    myfile << ".syst Syst4 -0.6 0.6" << "\n";
  }
  else 
    myfile << "\n";
  myfile << "+bg Bkg4 " << Nbkg2/6.;
  if(withUncertainties) {
    myfile << " 5\n";
    myfile << ".syst Syst5 -0.3 0.25" << "\n";
  }
  else 
    myfile << "\n";
  myfile << "+sig Sig " << Nsig2 <<  " \n";
  myfile << "+data " << Nobs2 << "\n";
  
  myfile.close();
}

double computeObservedBayesianMCMC(const std::string& file) 
{
  //model::Model model(model::Model::normal,4); // poly/expo
  model::Model model(model::Model::normal,1); // piece-wise expo
  //model::Model model(model::Model::normal,0); // piece-wise linear
  model::Channel* ch = model.addChannel("ch",file);
  model.makeModel();
  RooWorkspace* w = model.getWorkspace();
  BayesianMCMC bay(w);
  bay.setNumIters(1e7);
  double limit = bay.computeLimit();
  bay.printInterval();
  return limit;
}

void SingleChannelWithUncertaintiesOnBkg(bool withUncertainties=true)
{
  setStyle();
  gSystem->Load("OpTHyLiC_C");
  gSystem->Load("Models/Model_C");
  gSystem->Load("BayesianMCMC/BayesianMCMC_C");

  float lumii = 1;
  float lumif = 8;

  int nbins = lumif-lumii+1;
  TH1F* h_LimitOTHVsLumi = new TH1F("h_LimitOTHVsLumi","h_LimitOTHVsLumi",nbins,lumii-0.5,lumif+0.5);
  TH1F* h_LimitBayesianVsLumi = new TH1F("h_LimitBayesianVsLumi","h_LimitBayesianVsLumi",nbins,lumii-0.5,lumif+0.5);
  setHistoStyle(h_LimitOTHVsLumi,4,kBlack,"#mu_{up}","L");
  setHistoStyle(h_LimitBayesianVsLumi,5,kBlue,"#mu_{up}","L");

  for(float lumi=lumii; lumi<lumif; lumi+=1) {
    cout << endl << "-----------------------------------------------" << endl;
    cout << "-> Processing lumi = " << lumi << endl;
    cout << "-----------------------------------------------" << endl;
    TString fileName("dat/SingleChannel");
    if(!withUncertainties)
      fileName+="ForComparison";
    fileName+="WithUncertaintiesOnBkg_"; 
    fileName+=lumi; fileName+=".dat";
    float Nbkg2=100*lumi;
    float Nsig2=5*lumi;
    float Nobs2=90*lumi;
    createASCIIFile(withUncertainties,fileName,Nbkg2,Nsig2,Nobs2);
    double limit_OTH = computeObserved(1e6,OpTHyLiC::expo,OpTHyLiC::normal,fileName.Data());
    double limit_bayesianMCMC = computeObservedBayesianMCMC(fileName.Data());
    
    h_LimitOTHVsLumi->Fill(lumi,limit_OTH);
    h_LimitBayesianVsLumi->Fill(lumi,limit_bayesianMCMC);
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
  if(withUncertainties) {
    h_LimitOTHVsLumi->GetYaxis()->SetRangeUser(5.35,8.8);
  }
  else {
    h_LimitOTHVsLumi->GetYaxis()->SetRangeUser(.3,3.3);
  }
  h_LimitOTHVsLumi->Draw("p");
  h_LimitBayesianVsLumi->Draw("psame");
  TLegend* leg1 = new TLegend(0.5553691,0.7235023,0.9110738,0.9001536);
  leg1->SetFillColor(kWhite);
  leg1->SetLineColor(kBlack);
  leg1->SetBorderSize(1);
  leg1->AddEntry(h_LimitOTHVsLumi,"OpTHyLiC","p");
  leg1->AddEntry(h_LimitBayesianVsLumi,"Bayesian (uniform prior)","p");
  leg1->Draw();
  pPad2->cd();
  TH1* hratio = makeRatio(h_LimitBayesianVsLumi,h_LimitOTHVsLumi,"hratio","ratio");
  hratio->GetYaxis()->SetRangeUser(.95,1.05);
  hratio->Draw("p");
  if(withUncertainties) {
    system("rm -f results/SingleChannelWithUncertaintiesOnBkg.*");
    c->SaveAs("results/SingleChannelWithUncertaintiesOnBkg.png");
    c->SaveAs("results/SingleChannelWithUncertaintiesOnBkg.C");
    c->SaveAs("results/SingleChannelWithUncertaintiesOnBkg.pdf");
  }
  else {
    system("rm -f results/SingleChannelForComparisonWithUncertaintiesOnBkg.*");
    c->SaveAs("results/SingleChannelForComparisonWithUncertaintiesOnBkg.png");
    c->SaveAs("results/SingleChannelForComparisonWithUncertaintiesOnBkg.C");
    c->SaveAs("results/SingleChannelForComparisonWithUncertaintiesOnBkg.pdf");
  }
}

