#! /bin/bash
# A wrapper script to compare 2 text files, ignoring small numeric differences
# Needs ndiff ( http://www.math.utah.edu/~beebe/software/ndiff )

RELERR=1.0E-5
ABSERR=1.0E-5

ndiff -abserr $ABSERR --relative-error $RELERR <(grep -v "^\ *$" $1) <(grep -v "^\ *$" $2) && echo -e "GOOD:\t" \"$1\" and \"$2\" are the same with max possible relative error $RELERR and absolute error $ABSERR