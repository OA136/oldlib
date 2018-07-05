cd volatility
scp -r tools/linux/ root@$1:/root && ssh root@$1 "apt-get install dwarfdump && cd linux && make" && scp root@$1:/root/linux/module.dwarf tools/linux/
zip ./volatility/plugins/overlays/linux/master ./tools/linux/module.dwarf /boot/System.map-4.4.0-116-generic
cp ../libvmi/tools/pyvmi/pyvmiaddressspace.py ./volatility/plugins/addrspaces/
python vol.py -l vmi://master --profile=Linuxmasterx64 linux_pslist

echo "===========================install volatility end==================================================="
sleep 2
