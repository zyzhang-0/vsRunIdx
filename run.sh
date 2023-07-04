#!/bin/bash
start=$(date +%s)

echo "Generating the reversed pedestal RunId list:"
pedlist=$(ls fst/bk | sed "/BAD/d" | sed "/cmn/d" | cut -d '_' -f4 | sort -r | uniq) #reversed pedestal runid list
echo $pedlist
echo ""

echo "Setting up output directories..."                     
mkdir -p output
cd output
mkdir -p csh err list log ROOT plots report TRASH
cd ..
echo "Done."
echo ""

echo "Generating the xml template..."
echo "Making the fake input file list..."
cp input/submit_template submit.xml
cp input/filelist_template filelist.list
PWD=$(pwd)
sed -i "s#&PWD;#${PWD}#" submit.xml
sed -i "s#&PWD;#${PWD}#" filelist.list
echo "Done."
echo ""

rm prodRunId.h
touch prodRunId.h
echo "TString runId[] = {" >> prodRunId.h

echo "Start loop to submit jobs" 
echo "============================================================================================================="
while read RunId; do
	echo "\"${RunId}\"," >> prodRunId.h
	for pedId in $pedlist; do #loop over the reversed ped runid list, find the first that is smaller than the current prod runid
		if (($pedId < $RunId)); then
			break 1
		fi
	done
	for i in 1 2 #cp fst_s[n]_pedestals from fst/bk/ to fst, and rename it to what the fstBuilder read
	do
		cp fst/bk/fst_s${i}_pedestals_${pedId}_GOOD.txt fst/fst_s${i}_pedestals.txt
	done
	echo "----------Submit the job for Run ${RunId}----------"
	star-submit-template -template submit.xml -entities run=${RunId},ped=${pedId}

done < input/prodRun.list
sed -i '$ s/.$//' prodRunId.h
echo "}; " >> prodRunId.h
echo "prodRunId.h converted from prodRun.list"
mv *session* output/TRASH/
mv *dataset output/TRASH/

end=$(date +%s)
runtime=$((end - start))
echo "Run Time = ${runtime} seconds" #Monitor total run time
