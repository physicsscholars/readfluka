#! /bin/bash

TMP=$(mktemp)
echo $TMP
OUT=test_ustsuw
SUMLIS="$OUT"_sum.lis
USTSUW=$FLUTIL/ustsuw

if [ ! -f $USTSUW ]; then
	echo $USTSUW not found
        echo -e "\033[31m"$USTSUW" not found\033[0m"
	exit 1
fi

for file in *usrtrack; do echo $file >> $TMP; done

echo "" >> $TMP
echo  $OUT >> $TMP

cat $TMP

cat $TMP | $USTSUW > /dev/null || {
 echo  "ustsuw conversion failed"
 rm -f $TMP
 exit 2 
}

TMP1=$(mktemp)
ustsuw2txt $OUT > $TMP1 || { 
	echo -e "\033[31m ustsuw2txt conversion failed \033[0m"
	rm -f $TMP $TMP1
	exit 3
}

if [ $(awk NF $TMP1 | wc -l) -ne $(awk NF $SUMLIS | wc -l) ]; then
        echo -e "\033[31m Line number test failed\033[0m"
        exit 4
fi

bash $READFLUKA_SYS/../test/ndiff.sh USTSUW $TMP1 "$OUT"_sum.lis

rm -f $TMP $TMP1
