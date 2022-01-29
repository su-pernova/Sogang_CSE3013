dir=("$@")

if [ $# == 0 ];then
	dir=$(pwd)	
else
	dir=("$@")
	cd $dir
fi

if [ $? -ne 0 ];then
	exit 10
fi

echo -e "\nworking directory:"
echo -e ${dir}"\n"

idx=0
for i in *
do
	title=""
	number=0
	max="${#i}"
	((max--))

	while [ $number -le $max ]
	do
		if [[ ${i:$number:1} =~ [a-z] ]]
		then
			#echo ${i:$number:1}" = 소문자!"
			a=$(echo ${i:$number:1} | tr [:lower:] [:upper:])
			title=$title$a
		else
			#echo ${i:$number:1}" = 대문자!"
			b=$(echo ${i:$number:1} | tr [:upper:] [:lower:])
			title=$title$b
		fi
		((number++))
	done
	rename "s/$i/$title/" $i
done

exit 0
