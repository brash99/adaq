

if [ "$#" -lt 1 ]; then
    echo "NO port number"
    exit
fi
port=$1
cnt=1
ncnt=10

if [ "$#" -gt 1 ]; then
    ncnt=$2
fi

bdt=`date` 
while [ $cnt -lt $ncnt ]; 
do /home/highv/slowc/hvg.newAug2014/hvcli -p $port -h sbscdet -m 'LL' 
echo $cnt 
let cnt=cnt+1
done
echo "Start: $bdt"
echo -n "End:   "
date