#!/bin/bash
virusesFile=$1
countriesFile=$2
numLines=$3

# Produce random number within given limits
random_number() {
    local x=0
    while [ "$x" -le $1 ]
    do
        x=$RANDOM
        let "x %= $2" #Scale $number within $RANGE
    done
    echo "$x"
}

# Produce unique random numbers within given limits
num_array=( $( shuf -i 0-9999 -n "$numLines") )

# If duplicates enabled, 2% of IDs becomes duplicates
p=`bc <<< "$numLines*0.03/1"`

# local k=0
    # while [ $k -le $(( "$numLines"-1 )) ]
    # do
    #     # echo ${num_array[$i]}
    #     k=$(( $k + 1 ))
# done


while [ $p -gt 0 ]
do
    # get random indices in num_array
    k=$(random_number 0 "$numLines")
    j=$(random_number 0 "$numLines")

    # make the one duplicate of the other
    num_array[$j]=${num_array[$k]}

    p=$(( p - 1 ))
done


i=0
while [ $i -lt $numLines ]
do
    # Create random citizenID ############################################################################################
    # string_length=$(random_number 0 5)
    # # string_length=$(random_id)    
    # tr -dc '0-9' </dev/urandom | head -z -c $string_length  >> "recordsFile.txt"
    # echo -n ' ' >> "recordsFile.txt"
    
    citizenID=${num_array[$i]}
    echo $citizenID  | tr '\n' ' ' >> "recordsFile.txt"
    # echo -n ' ' >> "recordsFile.txt"

    # Create random name ############################################################################################
    string_length=$(random_number 2 13) # tr: remove (-d) all chars from incoming command BUT (-c) the ones in the set
    tr -dc 'a-zA-Z0-9' </dev/urandom | head -z -c $string_length  >> "recordsFile.txt"  # head -c : obtain first string_length bytes (chars) from /urandom
    echo -n ' ' >> "recordsFile.txt"

    # Create random surname ############################################################################################
    string_length=$(random_number 2 13)
    tr -dc 'a-zA-Z0-9' </dev/urandom | head -z -c $string_length  >> "recordsFile.txt"
    echo -n ' ' >> "recordsFile.txt"

    # Select random country ############################################################################################
    shuf -n 1 $countriesFile | tr '\n' ' ' >> "recordsFile.txt" 

    # Create random age ############################################################################################

    age=$(random_number 1 120)
    printf "%d " "$age" >> "recordsFile.txt"

    # Select random virus ############################################################################################
    # shuf -n 1 $virusesFile | tr '\n' ' ' >> "recordsFile.txt"
    # printf "\n" >> "recordsFile.txt"
    shuf -n 1 $virusesFile >> "recordsFile.txt"
    i=$(( $i + 1 ))
done

# remove last blank line
truncate -s -1 recordsFile.txt

# echo ${num_array[@]}
#  local i=0
#     while [ $i -le $(( "$numLines"-1 )) ]
#     do
#         # echo ${num_array[$i]}
#         i=$(( $i + 1 ))
#     done