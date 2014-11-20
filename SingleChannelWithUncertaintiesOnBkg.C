//////////////////////////////////////////////////////////////////////
// Usage:
//  > root -l SingleChannelWithUncertaintiesOnBkg.C
//////////////////////////////////////////////////////////////////////

#include "AtlasStyle.C"
#include "AtlasUtils.C"
#include "Utils.C"

void createASCIIFile(const bool withUncertainties, const TString filename, const float Nbkg2, const float Nsig2, const float Nobs2, const float L)
{
  ofstream myfile;
  myfile.open (filename);

  myfile << "+bg Bkg1 " << Nbkg2*L/4.;
  if(withUncertainties) {
    myfile << " " << 7 << "\n";
    myfile << ".syst Syst1 0.1 -0.3" << "\n";
    myfile << ".syst Syst2 0.3 -0.2" << "\n";
  }
  else myfile << "\n";
  myfile << "+bg Bkg2 " << Nbkg2*L/4;
  if(withUncertainties) {
    myfile << " " << 12 << "\n";
    myfile << ".syst Syst1 0.2 -0.05" << "\n";
    myfile << ".syst Syst3 -0.06 0.15" << "\n";
  }
  else 
    myfile << "\n";
  myfile << "+bg Bkg3 " << Nbkg2*L/3.;
  if(withUncertainties) {
    myfile << " " << 3.5 << "\n";
    myfile << ".syst Syst2 -0.1 0.3" << "\n";
    myfile << ".syst Syst3 0.15 -0.15" << "\n";
    myfile << ".syst Syst4 -0.6 0.6" << "\n";
  }
  else 
    myfile << "\n";
  myfile << "+bg Bkg4 " << Nbkg2*L/6.;
  if(withUncertainties) {
    myfile << " " << 5 << "\n";
    myfile << ".syst Syst5 -0.3 0.25" << "\n";
  }
  else 
    myfile << "\n";
  myfile << "+sig Sig " << Nsig2*L <<  " \n";
  myfile << "+data " << Nobs2*L << "\n";
  
  myfile.close();
}

double computeObservedBayesianMCMC(const std::string& file, const int interpExtrap, const int statConstraint, const int Niter) 
{
  // interp/extrap : 0 -> linear, 1 -> expo, 4 -> poly/expo
  model::Model model(statConstraint,interpExtrap);
  model::Channel* ch = model.addChannel("ch",file);
  model.makeModel();
  RooWorkspace* w = model.getWorkspace();
  BayesianMCMC bay(w);
  bay.setNumIters(Niter);
  double limit = bay.computeLimit();
  bay.printInterval();
  return limit;
}

void SingleChannelWithUncertaintiesOnBkg(bool withUncertainties=true, const int config=1, const int finallumi=8, const int Nexp=1e6, const int Niter=1e7)
{
  /////////////////////////////////////////////////////////////////////////////////////
  // Meaning of config parameter:
  //   - config=1: exponential (interp/extrap) + normal (stat constraint)
  //   - config=2: exponential (interp/extrap) + log-normal (stat constraint)
  //   - config=3: linear (interp/extrap) + normal (stat constraint)
  //   - config=4: linear (interp/extrap) + log-normal (stat constraint)
  //   - config=5: poly+expo (interp/extrap) + normal (stat constraint)
  //   - config=6: poly+expo (interp/extrap) + log-normal (stat constraint)
  /////////////////////////////////////////////////////////////////////////////////////

  setStyle();
  gSystem->Load("OpTHyLiC_C");
  gSystem->Load("Models/Model_C");
  gSystem->Load("BayesianMCMC/BayesianMCMC_C");

  float lumii = 1;
  float lumif = finallumi;

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
    float Nbkg2=100;
    float Nsig2=5;
    float Nobs2=90;
    createASCIIFile(withUncertainties,fileName,Nbkg2,Nsig2,Nobs2,lumi);
    double limit_OTH=0;
    double limit_bayesianMCMC=0;
    if(1==config) {
      limit_OTH = computeObserved(Nexp,OpTHyLiC::expo,OpTHyLiC::normal,fileName.Data());
      limit_bayesianMCMC = computeObservedBayesianMCMC(fileName.Data(),1,model::Model::normal,Niter);
    }
    else if(2==config) {
      limit_OTH = computeObserved(Nexp,OpTHyLiC::expo,OpTHyLiC::logN,fileName.Data());
      limit_bayesianMCMC = computeObservedBayesianMCMC(fileName.Data(),1,model::Model::logN,Niter);
    }
    else if(3==config) {
      limit_OTH = computeObserved(Nexp,OpTHyLiC::linear,OpTHyLiC::normal,fileName.Data());
      limit_bayesianMCMC = computeObservedBayesianMCMC(fileName.Data(),0,model::Model::normal,Niter);
    }
    else if(4==config) {
      limit_OTH = computeObserved(Nexp,OpTHyLiC::linear,OpTHyLiC::logN,fileName.Data());
      limit_bayesianMCMC = computeObservedBayesianMCMC(fileName.Data(),0,model::Model::logN,Niter);
    }
    else if(5==config) {
      limit_OTH = computeObserved(Nexp,OpTHyLiC::polyexpo,OpTHyLiC::normal,fileName.Data());
      limit_bayesianMCMC = computeObservedBayesianMCMC(fileName.Data(),4,model::Model::normal,Niter);
    }
    else if(6==config) {
      limit_OTH = computeObserved(Nexp,OpTHyLiC::polyexpo,OpTHyLiC::logN,fileName.Data());
      limit_bayesianMCMC = computeObservedBayesianMCMC(fileName.Data(),4,model::Model::logN,Niter);
    }
    else {
      cout << "ERROR! config unknown" << endl;
      return;
    }
    
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
    //h_LimitOTHVsLumi->GetYaxis()->SetRangeUser(7,8.6);
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
    TString suffix("");
    if(1==config) {
      suffix="expo-normal";
    }
    else if(2==config) {
      suffix="expo-logN";
    }
    else if(3==config) {
      suffix="linear-normal";
    }
    else if(4==config) {
      suffix="linear-logN";
    }
    else if(5==config) {
      suffix="polyexpo-normal";
    }
    else if(6==config) {
      suffix="polyexpo-logN";
    }
    system("rm -f results/SingleChannelWithUncertaintiesOnBkg.*");
    c->SaveAs(Form("results/SingleChannelWithUncertaintiesOnBkg_%s.png",suffix.Data()));
    c->SaveAs(Form("results/SingleChannelWithUncertaintiesOnBkg_%s.C",suffix.Data()));
    c->SaveAs(Form("results/SingleChannelWithUncertaintiesOnBkg_%s.pdf",suffix.Data()));
  }
  else {
    system("rm -f results/SingleChannelForComparisonWithUncertaintiesOnBkg.*");
    c->SaveAs("results/SingleChannelForComparisonWithUncertaintiesOnBkg.png");
    c->SaveAs("results/SingleChannelForComparisonWithUncertaintiesOnBkg.C");
    c->SaveAs("results/SingleChannelForComparisonWithUncertaintiesOnBkg.pdf");
  }
}

