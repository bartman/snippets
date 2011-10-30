#!/bin/bash

set -e

echo >&2 "Computing checksums"

declare -a ALL=( $(md5sum *.jpg *.nef | sed -re 's,[[:space:]]+,:,' | sort ) )

echo >&2 "Found ${#ALL[@]} files."

mkdir -p dups

count=0
lastsum=
lastfile=
for line in "${ALL[@]}"
do

        sum=${line%:**}
        file=${line#*:}

        if [ "$sum" = "$lastsum" ] ; then

                [ -f "$lastfile" ] || continue
                [ -f "$file" ] || continue

                echo >&2 "# -------------------------------- $file"

                if cmp -s "$lastfile" "$file"
                then
                        #echo rm -f "$file"
                        mv "$file" dups/
                        let "count = $count + 1"
                fi

        else

                echo >&2 "# $sum $file"
                
                lastsum="$sum"
                lastfile="$file"
        fi
done

echo >&2 "Found $count duplicates."
