///////////////////////////////////////////////////////////
// Usage : 
//  > root -l SingleChannelStatUncertNegativeBinomial.C
///////////////////////////////////////////////////////////

#include "AtlasStyle.C"
#include "AtlasUtils.C"
#include "Utils.C"

using namespace RooFit;

void createASCIIFile(const TString filename, const int Nbkg, const float NbkgUncert, const float Nsig, const float Nobs)
{
  ofstream myfile;
  myfile.open (filename);
  myfile << "+bg Bkg " << Nbkg << "  " << NbkgUncert << "\n";
  myfile << "+sig Sig " << Nsig << "\n";
  myfile << "+data " << Nobs << "\n";
  myfile.close();
}

void setPlotStyle(TPad* c, RooPlot* frame, const std::string Xtitle, const std::string Ytitle)
{
  c->SetLeftMargin(0.12);
  c->SetBottomMargin(0.15);
  
  frame->SetTitle("");
  frame->GetXaxis()->SetTitle(Xtitle.c_str());
  frame->GetYaxis()->SetTitle(Ytitle.c_str());
  frame->GetXaxis()->SetTitleSize(0.07);
  frame->GetXaxis()->SetTitleOffset(0.89);
  frame->GetXaxis()->SetLabelSize(0.06);
  frame->GetXaxis()->SetLabelOffset(0.017);
  frame->GetYaxis()->SetTitleSize(0.07);
  frame->GetYaxis()->SetTitleOffset(0.74);
  frame->GetYaxis()->SetLabelSize(0.06);
}

TH1* ShiftHisto(TH1* h)
{
  TString newName(h->GetName());
  TH1F* hNew = new TH1F(newName,newName,h->GetNbinsX(),h->GetXaxis()->GetBinCenter(1),h->GetXaxis()->GetBinCenter(h->GetNbinsX())+0.5);
  for(int i=0; i<h->GetNbinsX(); ++i) {
    hNew->SetBinContent(i,h->GetBinContent(i));
  }

  return hNew;
}

void makePlot(RooPlot* frame, RooRealVar& n, const int Nbkg, const float NbkgUncert, const std::string fileName)
{
  int Nexp=1e6;

  OpTHyLiC* oth = new OpTHyLiC(OpTHyLiC::mclimit,OpTHyLiC::gamma);
  createASCIIFile(fileName.c_str(),Nbkg,NbkgUncert,15,15);
  oth->addChannel("ch1",fileName.c_str());
  double cls;
  oth->observedSigStrengthFor95excl(Nexp,cls);

  TH1* hDistrBg = oth->getHisto(0,OTH::Channel::hDistrBg);
  TH1* hDistrBgShifted = ShiftHisto(hDistrBg);
  RooDataHist* dataHist = new RooDataHist("dataHist","dataHist",RooArgList(n),hDistrBgShifted);
  RooHistPdf* pdfHist = new RooHistPdf("pdfHist","pdfHist",RooArgSet(n),*dataHist);

  double yield=Nbkg;
  double yieldErr=NbkgUncert;

  stringstream ssYield;
  ssYield << yield;
  stringstream ssYieldErr;
  ssYieldErr << yieldErr;

  // Negative binomial
  RooRealVar a("a","a",yield/yieldErr/yieldErr);
  RooRealVar b("b","b",yield*yield/yieldErr/yieldErr);
  RooFormulaVar p("p","a/(a+1)",RooArgSet(a));
  // warning: in ROOT::Math::negative_binomial(k,p,n), k is the number of fails and n the number of successes
  RooGenericPdf* negbin = new RooGenericPdf("negbin","ROOT::Math::negative_binomial_pdf(n,p,b)",RooArgSet(n,p,b));
  pdfHist->plotOn(frame,Name("oth"),LineColor(kGreen+1),LineWidth(4),Normalization(1,RooAbsReal::Raw),Precision(1e-4));
  negbin->plotOn(frame,Name("negbin"),LineColor(kRed),LineWidth(4),LineStyle(kDashed),Normalization(1,RooAbsReal::Raw),Precision(1e-4));
}

void SingleChannelStatUncertNegativeBinomial() 
{
  setStyle();
  gSystem->Load("OpTHyLiC_C");
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);

  const int yield=15;

  // Poisson
  RooRealVar n("n","n",0,60);
  RooRealVar lambda("lambda","lambda",yield);
  RooPoisson pois("poisson","poisson",n,lambda);
  
  RooPlot* frame = n.frame();
  TCanvas* c = new TCanvas("c","c",900,400);
 
  setPlotStyle(c,frame,"n","P(N=n|b^{nom}, #sigma )");
  pois.plotOn(frame,Name("pois"),LineColor(kBlue),Normalization(1,RooAbsReal::Raw),Precision(1e-4));
  
  makePlot(frame,n,yield,2,"dat/SingleChannelStatUncertNegativeBinomial_1.dat");
  makePlot(frame,n,yield,7,"dat/SingleChannelStatUncertNegativeBinomial_2.dat");
  makePlot(frame,n,yield,14,"dat/SingleChannelStatUncertNegativeBinomial_4.dat");

  frame->Draw();

  TArrow* arr1 = new TArrow(23,0.06,20,0.051,0.02,"|>");
  arr1->SetAngle(40);
  arr1->Draw();
  TArrow* arr2 = new TArrow(25,0.047,18,0.04,0.02,"|>");
  arr2->SetAngle(40);
  arr2->Draw();
  TArrow* arr3 = new TArrow(27,0.035,18,0.022,0.02,"|>");
  arr3->SetAngle(40);
  arr3->Draw();

  TLatex latex;
  latex.SetTextSize(0.06);
  latex.SetTextAlign(13);  //align at top
  latex.DrawLatexNDC(0.7,0.6,"b^{nom} = 15");
  latex.DrawLatexNDC(0.5,0.45,"#sigma = 14");
  latex.DrawLatexNDC(0.47,0.54,"#sigma = 7");
  latex.DrawLatexNDC(0.44,0.65,"#sigma = 2");

  TLegend* leg = new TLegend(0.4151786,0.7069892,0.9375,0.9086022);
  leg->SetFillColor(kWhite);
  leg->SetLineColor(kWhite);
  leg->SetBorderSize(0);
  leg->AddEntry("pois","Poisson (analytical without uncertainty)","L");
  leg->AddEntry("negbin","Negative binomial (analytical with uncertainty)","L");
  leg->AddEntry("oth","Marginal distribution from OpTHyLiC","L");
  leg->Draw();

  system("rm -f results/SingleChannelStatUncertNegativeBinomial.*");
  c->SaveAs("results/SingleChannelStatUncertNegativeBinomial.png");
  c->SaveAs("results/SingleChannelStatUncertNegativeBinomial.C");
  c->SaveAs("results/SingleChannelStatUncertNegativeBinomial.pdf");
}
