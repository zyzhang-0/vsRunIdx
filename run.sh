#!/bin/bash
start=$(date +%s)
starver SL23b

mkdir -p log out plots err

for i in 1 2
do
	cp fst/bk/fst_s${i}_pedestals_24180042_GOOD.txt fst/fst_s${i}_pedestals.txt
done
#for RunId in 24174042 24174043 24174044 24174045 24174046 24174047 24174051 24175001 24175011 24175012 24175013 24175014 24175015 24175016 24175017 24175020 24175021 24175022 24175023 24175025 24175026
for RunId in 24180048
do
	(OnlTools/Jevp/launch fstBuilder -file /evp/a/${RunId} -pdf plots/${RunId}.pdf > log/${RunId}.txt ) >& err/${RunId}.txt
        mv test.root out/${RunId}.root
done

end=$(date +%s)
runtime=$((end - start))
echo "Run Time = ${runtime} seconds"
