echo "============================begin get windows offset============================"
sleep 2
cd libvmi/examples
./dump-memory $1 $1.dd
mv $1.dd ../tools/windows-offset-finder/
cd ../tools/windows-offset-finder
chmod u+x *.sh
./getoffset.sh $1
cd ../../../

echo "============================pyvmi install end============================"
sleep 2
