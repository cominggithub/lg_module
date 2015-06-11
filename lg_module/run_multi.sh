echo $OSTYPE
if [ "$OSTYPE" == "msys" ]; then
	echo "run on windows"
	./run_windows_multi.sh
else
	echo "run on mac"
	./run_mac.sh
fi