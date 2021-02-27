#!/bin/bash

# Check command line args
# Number of args has to be 4
if [ $# -ne 4 ]
then
    echo "You must enter exactly 4 parameters"
    exit 2
fi

# First two args must refer to existing files
if [[ ! -f $1 ]] || [[ ! -f $2 ]]
then
    echo "You must enter two existing files for arg 1 and arg 2"
    exit 2
fi

# Third arg has to be a contiguous string of digits
if [[ ! $3 =~ ^[0-9]+$ ]]
then
    echo "You must enter an integer for arg 3"
    exit 2
fi

# Fourth arg has to be either 0 or 1
if [[ ! $4 =~ ^[0-9]+$ ]] || [[ $4 -ne 0 ]] && [[ $4 -ne 1 ]]
then
    echo "You must enter 0 or 1 for arg 4"
    exit 2
fi

# Store args
virusesFile=$1
countriesFile=$2
numLines=$3
duplicatesAllowed=$4

# Toss a YES/NO coin
toss_coin() {
    local x=$(( $RANDOM%2 )) # x can be either 0 or 1
    if [ "$x" -eq 0 ]
    then
        echo "NO"
    else
        echo "YES"
    fi
}

# Produce unique random numbers within given limits
random_id() {
    num_array=( $( shuf -i 0-9999 -n "$numLines") )
    # If duplicates are allowed, up to 2% of IDs will be duplicates
    if [ $1 -eq 1 ]
    then
        local p=`bc <<< "$numLines*0.03/1"`   # floor to nearest integer
        while [ $p -gt 0 ]
        do
            # get random indices in num_array
            local k=$(( $RANDOM%$numLines ))
            local j=$(( $RANDOM%$numLines ))
            # make the one duplicate of the other
            num_array[$j]=${num_array[$k]}

            p=$(( p - 1 ))
        done
    fi
}

# Produce random date
random_date() {
    # let d=$date+%d
    local dd=$(( RANDOM%1+2020 ))
    local mm=$(( RANDOM%12+1 ))
    local yyyy=$(( RANDOM%28+1 ))
    d=`date -d "$dd-$mm-$yyyy" '+%d-%m-%Y'`
    echo "$d"
}

random_id $duplicatesAllowed
i=0
while [ $i -lt $numLines ]
do
    # Create random citizenID
    citizenID=${num_array[$i]}
    echo $citizenID  | tr '\n' ' ' >> "recordsFile.txt"

    # Create random name
    name_length=$(( $RANDOM%13+3 ))
    # tr -dc: remove all chars from incoming command BUT the ones in the set
    # head -c : keep first <name_length> bytes (chars) from /urandom
    tr -dc 'a-zA-Z' </dev/urandom | head -z -c $name_length  >> "recordsFile.txt"
    echo -n ' ' >> "recordsFile.txt"

    # Create random surname
    name_length=$(( $RANDOM%13+3 ))
    tr -dc 'a-zA-Z' </dev/urandom | head -z -c $name_length  >> "recordsFile.txt"
    echo -n ' ' >> "recordsFile.txt"

    # Select random country
    shuf -n 1 $countriesFile | tr '\n' ' ' >> "recordsFile.txt" 

    # Create random age
    age=$(( $RANDOM%120+1 ))
    printf "%d " "$age" >> "recordsFile.txt"

    # Select random virus
    shuf -n 1 $virusesFile | tr '\n' ' '>> "recordsFile.txt"

    # Randomly choose YES/NO
    vaccined=$(toss_coin)
    printf "%s" "$vaccined" >> "recordsFile.txt"

    # Create random date, if YES was generated
    if [[ $vaccined == "YES" ]]
    then
        date=$(random_date)
        printf " %s \n" "$date" >> "recordsFile.txt"
    else
        printf " \n" >> "recordsFile.txt"
    fi

    i=$(( $i + 1 ))
done

# Remove last blank line
truncate -s -1 recordsFile.txt