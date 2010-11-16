#! /bin/bash

TMP=$(mktemp)
OUT=test_usxsuw
USXSUW=$FLUTIL/usxsuw

if [ ! -f $USXSUW ]; then
	echo $USXSUW not found
        echo -e "\033[31m"$USXSUW" not found\033[0m"
	exit 1
fi

for file in *usrbdx; do echo $file >> $TMP; done

echo "" >> $TMP
echo  $OUT >> $TMP

cat $TMP | $USXSUW > /dev/null || ( echo  "usxsuw conversion failed" && exit 2 )

#usxsuw2txt $OUT > a.txt

#bash ndiff.sh a.txt "$OUT"_sum.lis

bash ndiff.sh USXSUW <(usxsuw2txt $OUT) <(cat "$OUT"_sum.lis)

rm -f $TMP
