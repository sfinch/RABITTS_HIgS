
if [ "$#" == 1 ]
then
    a=${1}
    b=${1}
elif [ "$#" == 2 ]
then
    a=${1}
    b=${2}
    if (($b < $a))
    then
        b=$a
    fi
else
    echo "Requires 1 or 2 arguments"
    exit
fi

for (( i=$a; i<=$b; i++ ))
do
    echo "Run number" $i
    root -l -q "run_info.C($i)"
    #root -l -q "analysis_6s9s.C($i)"
    #root -l -q "analysis_60s90s.C($i)"
    #root -l -q "analysis_180s240s.C($i)"
    root -l -q "analysis_cycle.C($i)"
done
