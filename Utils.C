

void setStyle()
{
  TStyle* atlasStyle=AtlasStyle();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
}

void setHistoStyle(TH1* h,const int markerstyle, const int markercolor, const TString Ytitle, const TString Xtitle)
{
  h->GetXaxis()->SetTitle("L");
  h->GetXaxis()->SetTitleSize(0.15);
  h->GetXaxis()->SetTitleOffset(0.83);
  h->GetXaxis()->SetLabelSize(0.16);
  h->GetXaxis()->SetLabelOffset(0.063);
  h->GetYaxis()->SetTitle(Ytitle);
  h->GetYaxis()->SetTitleSize(0.08);
  h->GetYaxis()->SetTitleOffset(0.67);
  h->GetYaxis()->SetLabelSize(0.07);
  h->GetYaxis()->SetLabelOffset(0.007);
  h->SetMarkerStyle(markerstyle);
  h->SetMarkerColor(markercolor);
  h->SetMarkerSize(1.5);
  h->SetLineColor(markercolor);
}

TH1* makeRatio(const TH1* h1, const TH1* h2, const TString name, const TString Ytitle)
{
  TH1* h_ratio = (TH1F*) h1->Clone(name);
  h_ratio->Divide(h2);
  h_ratio->SetMarkerStyle(21);
  h_ratio->SetMarkerSize(1.2);
  h_ratio->SetMarkerColor(kRed);
  h_ratio->GetYaxis()->SetTitleSize(0.16);
  h_ratio->GetYaxis()->SetTitleOffset(0.36);
  h_ratio->GetYaxis()->SetLabelSize(0.13);
  h_ratio->GetYaxis()->SetNdivisions(509);
  h_ratio->GetYaxis()->SetTitle(Ytitle);
  h_ratio->GetYaxis()->CenterTitle();
  return h_ratio;
}

double computeObserved(const int Nexp, const int interpExtrap, const int statConstraint, const std::string& file1, const std::string& file2="", const std::string& file3="")
{
  OpTHyLiC oth(interpExtrap,statConstraint);
  oth.addChannel("ch1",file1);
  if(file2!="") 
    oth.addChannel("ch2",file2);
  if(file3!="") 
    oth.addChannel("ch3",file3);

  //oth.printSamples();

  double cls;
  double obs = oth.observedSigStrengthFor95excl(Nexp,cls);
  return obs;
}

