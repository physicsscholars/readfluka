#! /bin/bash

TMP=$(mktemp)
OUT=test_usxsuw

for file in *usrbdx; do echo $file >> $TMP; done

echo "" >> $TMP
echo  $OUT >> $TMP

cat $TMP | $FLUTIL/usxsuw > /dev/null

#usxsuw2txt $OUT > a.txt

#bash ndiff.sh a.txt "$OUT"_sum.lis

bash ndiff.sh USXSUW <(usxsuw2txt $OUT) <(cat "$OUT"_sum.lis)

rm -f $TMP
