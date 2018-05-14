# FILE: params.sh
# PURPOSE: define problem parameters
# careful! no spaces around equals signs
# all variables must be set (if one is N/A, set it to an empty string)

# file lists may contain any number of files, separated by spaces
# input files
IFILES="anagrame.in"

# expected output files
OFILES="anagrame.out"

# files used by the verifier
KFILES="anagrame.ok"

# time limit in seconds
LIM_TIME="0.5"

# settings that are only honored under Linux
if [ "$ENV" == linux ]; then
	# data segment limit (in Mb, real number)
	LIM_DATA="512.0"

	# stack segment limit (in Mb, real number)
	LIM_STACK="512.0"
fi

# modules don't work under Linux yet
if [ "$ENV" == dos ]; then
	# name of module or empty string
	MODULE=""
fi
