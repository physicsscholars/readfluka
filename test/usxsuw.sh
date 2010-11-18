#! /bin/bash

TMP=$(mktemp)
OUT=test_usxsuw
SUMLIS="$OUT"_sum.lis
USXSUW=$FLUTIL/usxsuw

if [ ! -f $USXSUW ]; then
	echo $USXSUW not found
        echo -e "\033[31m"$USXSUW" not found\033[0m"
	exit 1
fi

for file in *usrbdx; do echo $file >> $TMP; done

echo "" >> $TMP
echo  $OUT >> $TMP

cat $TMP | $USXSUW > /dev/null || {
 echo  "usxsuw conversion failed"
 rm -f $TMP
 exit 2 
}

TMP1=$(mktemp)
usxsuw2txt $OUT > $TMP1 || { 
	echo -e "\033[31m usxsuw2txt conversion failed \033[0m"
	rm -f $TMP $TMP1
	exit 3
}

if [ $(awk NF $TMP1 | wc -l) -ne $(awk NF $SUMLIS | wc -l) ]; then
        echo -e "\033[31m Line number test filed\033[0m"
        exit 4
fi

bash $READFLUKA_SYS/../test/ndiff.sh USXSUW $TMP1 "$OUT"_sum.lis

rm -f $TMP $TMP1
