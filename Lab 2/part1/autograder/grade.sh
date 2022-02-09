#!/bin/bash

# Check if we have enough arguments
if [[ $# -ne 1 ]]; then
    echo "Usage: ./grade.sh <filename>"
    exit 1 # early exit
fi

# get parameters for display
num_qns=0
num_students=0

# Delete temporary files
rm ./ref/*.out # .out for correct ans
rm ./results.out # remove previous results

# Compile the reference program
gcc ./ref/*.c -o $1

# Generate reference output files
for i in ./ref/*.in; do
    ./$1 < $i > $i.out # note the ./, < and >

    let num_qns++
done

# Now mark submissions

#
# Note: See Lab02Qn.pdf for format of output file. Marks will be deducted for missing elements.
#
echo -e "Test date and time: $(date +%A), $(date +"%d %B %y"), $(date +%T)\n" >> results.out



# Iterate over every submission directory
for i in ./subs/*; do 
    let num_students++

    j=$(basename $i) # get repo name from path, i.e. student number

    # Compile C code
    # Print compile error message to output file
    gcc $i/*.c -o "$i/$1" 2> errors.out
    # Generate output from C code using *.in files in ref
    marks=0
    
    if [[ -e "$i/$1" ]]; then
        for k in ./ref/*.in; do
            l=$(basename $k)
            "$i/$1" < $k > "$i/$l.out"

            # Compare with reference output files  and award 1 mark if they are identical
            filename_ans="$k.out"
            filename_std="$i/$l.out"
            diff_res=$(diff $filename_ans $filename_std)

            if [[ "$diff_res" == "" ]]; then
                let marks++
            fi
        done
    else
        echo "Directory $j has a compile error." >> results.out
    fi

    # print score for student
    echo "Directory $j score $marks / $num_qns" >> results.out
done

# print total files marked.
echo -e "\nProcessed $num_students files." >> results.out

# Delete temporary files
rm ./subs/*/*.out # .out for submissions
    
