x=("$@")

if [ -z ${x[0]} ]
then
	echo "Usage: phone searchfor [...searchfor]"
	echo "(You didn't tell me what you want to search for.)"

else
	for x in $@
	do
		args=$args$x$"|"
	done

	grep ${args:0:(-1)} data2 > a
	awk -f display.awk a
fi
exit 0
