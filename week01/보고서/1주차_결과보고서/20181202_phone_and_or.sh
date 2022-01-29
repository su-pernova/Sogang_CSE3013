x=("$@")

if [ -z ${x[0]} ] || [ -z ${x[1]} ]
then
	echo "Usage: phone searchfor [...searchfor]"
	echo "(You didn't tell me what you want to search for.)"

elif [ "or" == "$1" ]
then 
	for x in $@
	do
		args=$args$x$"|"
	done
	egrep ${args:3:(-1)} data2 > a
	awk -f display.awk a

elif [ "and" == "$1" ]
then
	for x in $@
	do
		args=$args$x$"|"
	done
	grep ${args:4:(-1)} data2 > a
	awk -f display.awk a 
fi
exit 0
