echo "============================get ubuntu offset============================"
sleep 2
cd libvmi && scp -r ./tools/linux-offset-finder root@$1:/root && ssh root@$1 "apt-get update && apt-get install gcc make && cd linux-offset-finder && make && insmod findoffsets.ko && rmmod findoffsets.ko && dmesg" && scp root@$1:/boot/System.map-4.4.0-116-generic /boot && cd ..
