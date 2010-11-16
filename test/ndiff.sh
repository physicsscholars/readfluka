#! /bin/bash
# A wrapper script to compare 2 text files, ignoring small numeric differences
# Needs ndiff ( http://www.math.utah.edu/~beebe/software/ndiff )
#
# Usage: ndiff.sh TESTID file1 file2
#   or   ndiff.sh TESTID <(cat file1) <(cat file2)
#        TESTID - any string to describe the current test

RELERR=1.0E-5
ABSERR=1.0E-4

TESTID=$1

# ^\ *$ - removes empty lines or lines with spaces only
# Terminal display attributes (set color): http://www.termsys.demon.co.uk/vtansi.htm
ndiff -abserr $ABSERR --relative-error $RELERR <(grep -v "^\ *$" $2) <(grep -v "^\ *$" $3) && echo -e "\033[32m"$TESTID" TEST PASSED\033[0m" || echo -e "\033[31m"$TESTID" TEST FAILED\033[0m"
