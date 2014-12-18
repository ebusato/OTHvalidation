#include "TStopwatch.h"

class Result {
public:
  int NpseudoExp;
  double limit;
  double cpuTime;

  inline double diffWrtMcLimit(double mclimitVal) {return (limit-mclimitVal)/mclimitVal;}
};

enum {expM2,expM1,expMed,expP1,expP2,obs};
Result* runLimit(const int type,
		 const int nbExp,
		 const std::string& file1, 
		 const std::string& file2, 
		 const std::string& file3) {
  TStopwatch w;
  w.Start();

  OpTHyLiC oth(OTH::mclimit,OTH::normal);

  oth.addChannel("ch1",file1);
  oth.addChannel("ch2",file2);
  oth.addChannel("ch3",file3);
  
  double cls;
  double limit=0;
  if(type==expM2)
    limit=oth.expectedM2sigStrengthExclusion(nbExp,cls);
  else if(type==expM1)
    limit=oth.expectedM1sigStrengthExclusion(nbExp,cls);
  else if(type==expMed)
    limit=oth.expectedMedSigStrengthExclusion(nbExp,cls);
  else if(type==expP1)
    limit=oth.expectedP1sigStrengthExclusion(nbExp,cls);
  else if(type==expP2)
    limit=oth.expectedP2sigStrengthExclusion(nbExp,cls);
  else if(type==obs)
    limit=oth.observedSigStrengthExclusion(nbExp,cls);
  w.Stop(); 
  
  cout << "Result:" << endl;
  cout << "  -> limit="   << limit << endl;  
  cout << "  -> CPU time=" << w.CpuTime() << endl;

  Result* res = new Result();
  res->NpseudoExp=nbExp;
  res->limit=limit;
  res->cpuTime=w.CpuTime();
  return res;
}

void ComparisonWithMcLimit_SgluonPartialStat(TString outputFileName)
{
  gSystem->Load("OpTHyLiC_C");

  TStopwatch w;
  w.Start();

  const int nbExp=5e4;

  // Expected minus 2 sigma
  Result* expM2_350 = runLimit(expM2,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_mm.dat");

  Result* expM2_400 = runLimit(expM2,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_mm.dat");

  Result* expM2_500 = runLimit(expM2,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_mm.dat");

  Result* expM2_600 = runLimit(expM2,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_mm.dat");

  Result* expM2_800 = runLimit(expM2,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_mm.dat");

  Result* expM2_1000 = runLimit(expM2,nbExp,
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_ee.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_em.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_mm.dat");
  
  // Expected minus 1 sigma
  Result* expM1_350 = runLimit(expM1,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_mm.dat");

  Result* expM1_400 = runLimit(expM1,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_mm.dat");

  Result* expM1_500 = runLimit(expM1,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_mm.dat");

  Result* expM1_600 = runLimit(expM1,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_mm.dat");

  Result* expM1_800 = runLimit(expM1,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_mm.dat");

  Result* expM1_1000 = runLimit(expM1,nbExp,
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_ee.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_em.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_mm.dat");
  
  // Expected median
  Result* expMed_350 = runLimit(expMed,nbExp,
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_ee.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_em.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_mm.dat");
  
  Result* expMed_400 = runLimit(expMed,nbExp,
				 "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_ee.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_em.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_mm.dat");

  Result* expMed_500 = runLimit(expMed,nbExp,
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_ee.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_em.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_mm.dat");

  Result* expMed_600 = runLimit(expMed,nbExp,
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_ee.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_em.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_mm.dat");

  Result* expMed_800 = runLimit(expMed,nbExp,
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_ee.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_em.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_mm.dat");

  Result* expMed_1000 = runLimit(expMed,nbExp,
				 "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_ee.dat",
				 "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_em.dat",
				 "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_mm.dat");
  
  // Expected plus 1 sigma
  Result* expP1_350 = runLimit(expP1,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_mm.dat");

  Result* expP1_400 = runLimit(expP1,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_mm.dat");

  Result* expP1_500 = runLimit(expP1,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_mm.dat");
  
  Result* expP1_600 = runLimit(expP1,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_mm.dat");

  Result* expP1_800 = runLimit(expP1,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_mm.dat");

  Result* expP1_1000 = runLimit(expP1,nbExp,
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_ee.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_em.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_mm.dat");
  
  // Expected plus 2 sigma
  Result* expP2_350 = runLimit(expP2,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_mm.dat");

  Result* expP2_400 = runLimit(expP2,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_mm.dat");

  Result* expP2_500 = runLimit(expP2,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_mm.dat");

  Result* expP2_600 = runLimit(expP2,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_mm.dat");

  Result* expP2_800 = runLimit(expP2,nbExp,
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_ee.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_em.dat",
			       "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_mm.dat");

  Result* expP2_1000 = runLimit(expP2,nbExp,
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_ee.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_em.dat",
				"datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_mm.dat");
  
  // Observed limits (mclimit,normal)
  Result* obs_350 = runLimit(obs,nbExp,
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_ee.dat",
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_em.dat",
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon350_mm.dat");

  Result* obs_400 = runLimit(obs,nbExp,
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_ee.dat",
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_em.dat",
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon400_mm.dat");

  Result* obs_500 = runLimit(obs,nbExp,
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_ee.dat",
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_em.dat",
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon500_mm.dat");

  Result* obs_600 = runLimit(obs,nbExp,
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_ee.dat",
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_em.dat",
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon600_mm.dat");

  Result* obs_800 = runLimit(obs,nbExp,
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_ee.dat",
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_em.dat",
			     "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon800_mm.dat");

  Result* obs_1000 = runLimit(obs,nbExp,
			      "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_ee.dat",
			      "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_em.dat",
			      "datComparisonWithMcLimit_SgluonPartialStat/CH_Sgluon1000_mm.dat");

  w.Stop();

  cout << endl;
  cout << "--------------------------------------------------" << endl;
  cout << "                 JOB RESULT                      " << endl;

  cout << " -> Printing result in results/SgluonPartialStat.txt" << endl;
  ofstream myfile;
  myfile.open (outputFileName);
  myfile << "  & 350 & 400 & 500 & 600 & 800 & 1000 \\\\ \\hline" << endl;
  myfile << "-2$\\sigma$ &"
	 << expM2_350->limit << " & " 
	 << expM2_400->limit << " & " 
	 << expM2_500->limit << " & " 
	 << expM2_600->limit << " & " 
	 << expM2_800->limit << " & " 
	 << expM2_1000->limit << " \\\\ \\hline " 
	 << endl;
  myfile << "-1$\\sigma$ &"
	 << expM1_350->limit << " & " 
	 << expM1_400->limit << " & " 
	 << expM1_500->limit << " & " 
	 << expM1_600->limit << " & " 
	 << expM1_800->limit << " & " 
	 << expM1_1000->limit << " \\\\ \\hline " 
	 << endl;
  myfile << "median &"
	 << expMed_350->limit << " & " 
	 << expMed_400->limit << " & " 
	 << expMed_500->limit << " & " 
	 << expMed_600->limit << " & " 
	 << expMed_800->limit << " & " 
	 << expMed_1000->limit << " \\\\ \\hline " 
	 << endl;
  myfile << "+1$\\sigma$ &"
	 << expP1_350->limit << " & " 
	 << expP1_400->limit << " & " 
	 << expP1_500->limit << " & " 
	 << expP1_600->limit << " & " 
	 << expP1_800->limit << " & " 
	 << expP1_1000->limit << " \\\\ \\hline " 
	 << endl;
  myfile << "+2$\\sigma$ &"
	 << expP2_350->limit << " & " 
	 << expP2_400->limit << " & " 
	 << expP2_500->limit << " & " 
	 << expP2_600->limit << " & " 
	 << expP2_800->limit << " & " 
	 << expP2_1000->limit << " \\\\ \\hline " 
	 << endl;
  myfile << "observed &"
	 << obs_350->limit << " & " 
	 << obs_400->limit << " & " 
	 << obs_500->limit << " & " 
	 << obs_600->limit << " & " 
	 << obs_800->limit << " & " 
	 << obs_1000->limit << " \\\\ \\hline " 
	 << endl;
  myfile.close();

  cout << endl;
  cout << "\\documentclass[a4paper,notitlepage]{article}" << endl;
  cout << "\\begin{document}" << endl;
  cout << "\\begin{table}" << endl;
  cout << "\\begin{center}" << endl;
  cout << "\\caption{Limits for sgluon (total cpu time=" << w.CpuTime() << " sec, total real time=" << w.RealTime() << " sec). Following parameters have been used: No pseudo exp.=" << nbExp << ", interpolation/extrapolation=mclimit, stat constraint p.d.f=normal.}" << endl;
  cout << "\\begin{tabular}{ | c | c | c | c | c | c | c | c |}" << endl;
  cout << "\\hline" << endl;
  cout << "  & 350 & 400 & 500 & 600 & 800 & 1000 \\\\ \\hline" << endl;
  cout << "-2$\\sigma$ &"
       << expM2_350->limit << " & " 
       << expM2_400->limit << " & " 
       << expM2_500->limit << " & " 
       << expM2_600->limit << " & " 
       << expM2_800->limit << " & " 
       << expM2_1000->limit << " \\\\ \\hline " 
       << endl;
  cout << "-1$\\sigma$ &"
       << expM1_350->limit << " & " 
       << expM1_400->limit << " & " 
       << expM1_500->limit << " & " 
       << expM1_600->limit << " & " 
       << expM1_800->limit << " & " 
       << expM1_1000->limit << " \\\\ \\hline " 
       << endl;
  cout << "median &"
       << expMed_350->limit << " & " 
       << expMed_400->limit << " & " 
       << expMed_500->limit << " & " 
       << expMed_600->limit << " & " 
       << expMed_800->limit << " & " 
       << expMed_1000->limit << " \\\\ \\hline " 
       << endl;
  cout << "+1$\\sigma$ &"
       << expP1_350->limit << " & " 
       << expP1_400->limit << " & " 
       << expP1_500->limit << " & " 
       << expP1_600->limit << " & " 
       << expP1_800->limit << " & " 
       << expP1_1000->limit << " \\\\ \\hline " 
       << endl;
  cout << "+2$\\sigma$ &"
       << expP2_350->limit << " & " 
       << expP2_400->limit << " & " 
       << expP2_500->limit << " & " 
       << expP2_600->limit << " & " 
       << expP2_800->limit << " & " 
       << expP2_1000->limit << " \\\\ \\hline " 
       << endl;
  cout << "observed &"
       << obs_350->limit << " & " 
       << obs_400->limit << " & " 
       << obs_500->limit << " & " 
       << obs_600->limit << " & " 
       << obs_800->limit << " & " 
       << obs_1000->limit << " \\\\ \\hline " 
       << endl;
  cout << "\\end{tabular}" << endl;
  cout << "\\end{center}" << endl;
  cout << "\\end{table}" << endl;
  cout << "\\end{document}" << endl;
  cout << "--------------------------------------------------" << endl << endl;

}

void makeExclusionPlot(const std::string file1, const std::string file2="")
{
  gSystem->Load("ExclusionPlot_C");

  gStyle->SetOptTitle(0);

  TCanvas* c = new TCanvas("c","c",1200,800);

  LimitBrasilPlot limitBrasil1("McLimit",TheoXsec::SgluonPartialStat);
  limitBrasil1.readFile(file1);
  limitBrasil1.makePlot(false,1e-2,4);
  //limitBrasil1.plotTheoGraph(true);

  if(file2!="") {
    LimitBrasilPlot limitBrasil2("OpTHyLiC",TheoXsec::SgluonPartialStat);
    limitBrasil2.readFile(file2);
    limitBrasil2.makePlot(true,1e-2,4);
    //limitBrasil2.plotTheoGraph(true);
  }

  gPad->SaveAs("results/ExclusionPlot_Sgluon.pdf");
  gPad->SaveAs("results/ExclusionPlot_Sgluon.png");
}
