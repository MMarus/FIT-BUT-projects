#!/bin/bash

test_line()
{
	if [[ $p == 0 ]]
	then 
		if [[ $line == *callq*\<* ]] && [[ $line != *\@plt* ]]  
		then 
			ret=1
		else ret=0
		fi
	else
		if [[ $line == *callq*\<* ]]
		then ret=1
		else ret=0
		fi
	fi
}


g=0
p=0
CALLER='0'
CALLEE='0'
 

while getopts ":r:d:gp" opt; do
	case "$opt" in
	g) g=1;;
	p) p=1;;
  r) CALLEE="$OPTARG";;
  d) CALLER="$OPTARG";;
  esac
done

shift $((OPTIND-1))

#echo $g $p $CALLER $CALLEE
if [[ $CALLER != 0 && $CALLEE != 0 ]]
then echo "zle zadane argumenty" >&2
exit 1
fi

if [[ $g == 1 ]]
then echo "digraph CG {"
fi

objdump -d -j .text $@ >/dev/null 2>/dev/null

if [[ $? != 0 ]]
then echo "zle zadany subor" >&2
exit 1
fi

(objdump -d -j .text $@) |
while read -r line
do
	if [[ $line == *0000000*\<*\>* ]]
		then caller=$(echo $line | sed -e 's/.*<//' -e 's/>.*//' )
	fi
	ret=0 
	test_line
	if [[ $ret == 1 ]] &&  [[ ! "$line" =~ [,.+-] ]] && [[ ! "$caller" =~ [,.+-] ]]
		then callee=$(echo $line | sed -e 's/.*<//' -e 's/>.*//' )
		
		if [[ $CALLER == 0 || $CALLER == $caller ]] && [[ $CALLEE == 0 || $CALLEE == $callee ]]
		then 
				if [ $g == 1 ]
				then callee=$(echo -n $callee | sed -e 's/\@plt.*/_PLT/';echo ";")
				fi
				echo $caller" -> "$callee
		fi
	fi	
done | sort -u 

if [[ $g == 1 ]]
then echo "}" 
fi

