set -x
srcfile="./data/rank.txt"
destfile="./data/rank.xml"
wget "http://top.baidu.com/buzz?b=618&fr=topbuzz_b18" -O $srcfile
grep '<table' -A 10000 $srcfile | grep '</table' -B 10000 > $destfile
./spider
