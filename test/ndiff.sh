#! /bin/bash
# A wrapper script to compare 2 text files, ignoring small numeric differences
# Needs ndiff ( http://www.math.utah.edu/~beebe/software/ndiff )
#
# Usage: ndiff.sh TESTID file1 file2
#   or   ndiff.sh TESTID <(cat file1) <(cat file2)
#        TESTID - any string to describe the current test

RELERR=1.0E-5
ABSERR=5.0E-5

TESTID=$1

# awk NF filename - removes empty lines or lines with spaces only ( the same with grep is grep -vc '^[[:space:]]*$'  )
# Terminal display attributes (set color): http://www.termsys.demon.co.uk/vtansi.htm
#ndiff -abserr $ABSERR --relative-error $RELERR <(awk NF $2) <(awk NF $3) && echo -e "\033[32m"$TESTID" TEST PASSED\033[0m" || echo -e "\033[31m"$TESTID" TEST FAILED\033[0m"

awk NF $2 > /tmp/a.txt
awk NF $3 > /tmp/b.txt
ndiff -abserr $ABSERR --relative-error $RELERR /tmp/a.txt /tmp/b.txt && echo -e "\033[32m"$TESTID" TEST PASSED\033[0m" || echo -e "\033[31m"$TESTID" TEST FAILED\033[0m"
