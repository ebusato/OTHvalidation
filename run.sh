#!/bin/bash

################################################################
# To be modified by the user according to his own installation
################################################################

#source /afs/cern.ch/sw/lcg/external/gcc/4.3.2/x86_64-slc5/setup.csh
#cd /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.07/x86_64-slc5-gcc43-opt/root/
#source bin/thisroot.csh
#cd -

source /AtlasDisk/home/tpelzer/Software/scripts/setup-gcc4.8.2_python2.7.7_root5.34.18.sh

dirOpTHyLiC=/AtlasDisk/home/busato/stat/Limit/CLsGenStuff/OpTHyLiC
dirTIFOSI=/AtlasDisk/home/busato/stat/Limit/TIFOSI
dirExclusionPlot=/AtlasDisk/home/busato/stat/Limit/ExclusionPlot

################################################################
# Code below should in principle not be modified by the user
################################################################

echo "-----------------------------------------------------"
echo "Using following configuration:"
echo "  -> ROOT: "${ROOTSYS}
echo "  -> OpTHyLiC directory: "${dirOpTHyLiC}
echo "  -> TIFOSI directory: "${dirTIFOSI}
echo "-----------------------------------------------------"

source ${dirOpTHyLiC}/setup.sh
source ${dirTIFOSI}/setup.sh
source ${dirExclusionPlot}/setup.sh

mkdir results
mkdir dat

# Comparisons with theory and bayesian with uniform prior
root -l -b -q SingleChannelNoUncertainties.C
root -l -b -q MultipleChannelsNoUncertainties_OTHVsAsymptotic.C
root -l -b -q MultipleChannelsNoUncertainties_NoChannelsVsLumi.C
root -l -b -q 'SingleChannelWithUncertaintiesOnBkg.C(0)'
root -l -b -q 'SingleChannelWithUncertaintiesOnBkg.C(1)'

# Comparison with McLimit for full plot
root -l -b <<EOF
.L ComparisonWithMcLimit_SgluonPartialStat.C
ComparisonWithMcLimit_SgluonPartialStat("results/SgluonPartialStat.txt")
makeExclusionPlot("results/SgluonPartialStat.txt")
EOF

# Multithread -> to be written

# Merge all results into a single file 
rm -f results/merged.pdf
gs -q -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -sOutputFile=results/merged.pdf results/*.pdf