#!/bin/sh

pierwszy_podpunkt() {
	if [ $1 == "receive" ]; then
		x=$(awk -F: '/enp0s3/ {print $1, $2}' /proc/net/dev | awk '{print $2}')
	elif [ $1 == "transmit" ]; then
		x=$(awk -F: '/enp0s3/ {print $1, $2}' /proc/net/dev | awk '{print $10}')
	elif [ $1 == "graph" ]; then
		x=$(awk -F: '/enp0s3/ {print $1, $2}' /proc/net/dev | awk '{print $2}')
	fi
	ilebit=$x
	unit="bites"
	while [ $(echo "$ilebit >= 1024" | bc -l) -eq 1 ]
	do
		ilebit=$(echo "scale=3;$ilebit/1024" | bc -l);
		if [ $unit == "bites" ]; then
			unit="kilobites";
		elif [ $unit == "kilobites" ]; then
			unit="megabites";
		elif [ $unit == "megabites" ]; then
			unit="gigabites";
		fi
	done
	if [ $1 == "receive" ]; then
		echo "Receive - $ilebit $unit"
	elif [ $1 == "transmit" ]; then
		echo "Transmit - $ilebit $unit"
	elif [ $1 == "graph" ]; then
		echo $ilebit
	fi
}

drugi_podpunkt() {
	cpus='cpu cpu1 cpu2 cpu3'
	for x in $cpus
	do
		line=$(awk -v cpu=$x '$1==cpu {print $0}' /proc/stat)
		user=$(echo $line | awk '{print $2}')
		nice=$(echo $line | awk '{print $3}')
		system=$(echo $line | awk '{print $4}')
		idle=$(echo $line | awk '{print $5}')
		iowait=$(echo $line | awk '{print $6}')
		irq=$(echo $line | awk '{print $7}')
		softirq=$(echo $line | awk '{print $8}')

		total=$((user + nice + system + idle + iowait + irq + softirq))
		usage=$((user + nice + system + irq + softirq))
		percentage=$(echo "scale=3; $usage / $total * 100" | bc -l)
		echo "For $x percentage usage is $percentage%"		
	done
}

trzeci_podpunkt() {
	time=$(awk '{print $1}' /proc/uptime)
	echo "$time seconds"
	timemin=$(echo "scale=3; $time / 60" | bc -l)
	echo "$timemin minutes"
	timehour=$(echo "scale=3; $timemin / 60" | bc -l)
	echo "$timehour hours"
	timedays=$(echo "scale=3; $timehour / 24" | bc -l)
	echo "$timedays days"
}

czwarty_podpunkt() {
	battery=$(cat "/sys/class/power_supply/BAT0/uevent" | grep POWER_SUPPLY_CAPACITY=)
	x=$(echo $battery | awk -F= '{print $2}')
	echo "Battery capacity $x"
}

piaty_podpunkt() {
	x=$(awk '{print $1}' /proc/loadavg)
	echo "Average load: $x"
}

szosty_podpunkt() {
	totalmem=$(cat /proc/meminfo | grep Mem | awk -F: '$1=="MemTotal" {print $2}' | awk '{print $1}')
	availmem=$(cat /proc/meminfo | grep Mem | awk -F: '$1=="MemAvailable" {print $2}' | awk '{print $1}')
	takenmem=$((totalmem - availmem))
	echo "Memory used: $takenmem"
}

add_to_array() {
	array=$1
	var=$2
	arr+=("${array[@]:1:10}")
	arr+=$var
	array=("${array[@]"}
	echo "${array[@]}"
}

graph_drawer() {
	array=$1
	max=$(findmax $array)
	for height in {10...1}; do
		for x in ${array[@]};do
			x=$(echo "$x / $max * 10" | bc -l)
			if [ $x -ge height ]; then
				printf "\u2B1C"
			else
				printf "  "
			fi
		done
	done
}			

findmax() {
	array=$1
	max=0
	for x in ${array[@]}; do
		if [ $x -gt $max ]; then
			max=$x
		fi
	done
	echo $max
}

array=(0 0 0 0 0 0 0 0 0 0)
while(true)
do
	pierwszy_podpunkt "receive"
	pierwszy_podpunkt "transmit"
	dana=$(pierwszy_podpunkt "graph")
	drugi_podpunkt
	trzeci_podpunkt
	czwarty_podpunkt
	piaty_podpunkt
	szosty_podpunkt
	array=$(add_to_array $array $dana)
	graph_drawer $array
	sleep 2
done
