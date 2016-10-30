#!/bin/bash

g=0
obj1=0
obj2=0

while getopts ":gr:d:" opt; do
	case "$opt" in
	g) g=1;;
  r) obj2="$OPTARG";;
  d) obj1="$OPTARG";;
  esac
done

shift $((OPTIND-1))

#echo $g $obj2 $obj1

if [[ $obj2 != 0 && $obj1 != 0 ]]
then echo "zle zadane argumenty" >&2
exit 1
fi

findobj()
{
	while read line
	do 
		if [[ $line == *\(${a[2]}\)* ]]
		then b=( $line ) 
			OBJ2=${b[0]} sym=${b[1]}
			TOBJ1=$(basename $OBJ1)
			TOBJ2=$(basename $OBJ2)
			if [[ $obj1 == 0 || $TOBJ1 == $obj1 ]] && [[ $obj2 == 0 || $TOBJ2 == $obj2 ]]
			then
				if [[ $g == 1 ]] 
				then 
					echo -n $TOBJ1  | sed -e 's/\./D/' -e 's/\-/\_/' -e 's/\+/P/'
					echo -n " -> "
					echo -n $TOBJ2	| sed -e 's/\./D/' -e 's/\-/\_/' -e 's/\+/P/'
					echo ' '$sym | sed -e 's/(/[label="/' -e 's/)/"];/'
					if [[ $du != *"[label=\"$OBJ1\"];"*  ]]; then
						du=$(echo "$du"; echo -n "$TOBJ1" | sed -e 's/\./D/' -e 's/\-/\_/' -e 's/\+/P/'; echo -n " [label=\""; echo -n "$OBJ1"; echo "\"];" )
					fi
					if [[ $du != *"[label=\"$OBJ2\"];"*  ]]; then
						du=$(echo "$du"; echo -n "$TOBJ2" | sed -e 's/\./D/' -e 's/\-/\_/' -e 's/\+/P/'; echo -n " [label=\""; echo -n "$OBJ2"; echo "\"];" )
					fi
					

				else echo $OBJ1 "->" $OBJ2 $sym
				fi
			fi
		fi
	done < $TEMP_F
}

nm $@ >/dev/null 2>/dev/null
if [[ $? != 0 ]]; then
	echo "zle zadany subor" >&2
	exit 1
fi

TEMP_F=$(mktemp /tmp/$USER.XXXXXX)

trap "rm $TEMP_F; exit" SIGHUP SIGINT SIGTERM

nm $@ |
while read -r line
do
	a=( $line )
	if [[ ${#a[*]} == 3 ]] && [[ "${a[1]}" =~ [U,B,C,D,G,T] ]]
	then echo $OBJ1 "("${a[2]}")"
	fi
	if [[ ${#a[*]} == 2 ]]
	then echo $OBJ1 "->" ${a[1]}
	fi
	if [[ $line == *.o* ]]
	then	OBJ1=${line%?}
	fi
done >$TEMP_F 

if [[ $g == 1 ]]
	then echo "digraph GSYM {"
fi

while read line
do
	a=( $line )
	if [[ ${#a[*]} == 3 ]]
	then OBJ1=${a[0]} 
	findobj 
	fi
done < $TEMP_F



if [[ $g == 1 ]]
then \
	echo -e "$du" | tail -n +2
	echo "}"
fi

rm $TEMP_F
exit