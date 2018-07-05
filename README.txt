===========宿主机：Ubuntu-16.04.4-server-amd64 内核版本：4.4.0-62-generic==========
===========虚拟机：Ubuntu-16.04.4-server-amd64 内核版本：4.4.0-116-generic==========
===========Win7：cn_windows_7_enterprise_with_sp1_x64_dvd_620654.iso ===============
##学长github
https://github.com/lixingchen12138/libvmi-volatility-master
https://github.com/lixingchen12138/monitor_server
##更换ubuntu源：/etc/apt/sources.list  https://mirrors.tuna.tsinghua.edu.cn/help/ubuntu/
##root用户下

mkdir sq
cd sq
综述：
	先下载必要库，安装libvirt、qemu、libvmi、volatility，创建虚拟机，进行测试libvmi测试、volatility测试
	./download.sh && ./install_libvirt_qemu.sh && ./install_libvmi.sh ./install_pyvmi.sh
	
	创建虚拟机：./create_master.sh && ./create_win.sh
	
	获取linux偏移：	./ssh_withoutpwd.sh VM_ip && ./get_offset_linux.sh VM_ip
	
	获取windows偏移：./get_offset_windows domain_name
	
	测试volatility：./install_volatility.sh && linux：./make_dwarf.sh VM_ip
	
	
#下载：（运行: ./download.sh）
wget https://libvirt.org/sources/libvirt-1.3.1.tar.gz
git clone https://github.com/moyix/pdbparse.git
git clone https://github.com/json-c/json-c.git
git clone https://github.com/FreddieWitherden/libforensic1394.git
wget https://github.com/VirusTotal/yara/archive/v3.7.1.tar.gz
wget http://effbot.org/media/downloads/Imaging-1.1.7.tar.gz
git clone https://github.com/volatilityfoundation/volatility.git

#1、安装libvirt-1.3.1和qemu：(运行：./install_libvirt_qemu.sh)
##依赖
	apt-get install check ebtables dnsmasq bridge-utils libnl-route-3-dev libnl-3-dev libxml2-dev libyajl-dev libxml2-utils xsltproc libdevmapper-dev libpciaccess-dev automake libxml++2.6-2v5 libxml++2.6-dev libxml-xpath-perl libglib2.0-dev libtool autoconf automake flex bison libfuse-dev libjson-c-dev python-dev libncurses5-dev python3-dev libfuse-dev libxc1 libffi-dev cmake libcurl4-openssl-dev autoconf libtool bison flex libpixman-1-dev zlib1g-dev libglib2.0-dev libsnappy-dev libgtk-3-dev libsdl2-dev libjpeg-turbo8-dev libspice-server-dev cabextract mscompress python-pefile
##编译libvirt：
	tar zxvf libvirt-1.3.1.tar.gz && cd libvirt-1.3.1 && ./configure --prefix=/usr --localstatedir=/var --sysconfdir=/etc
	make
	make install
##安装qemu
	apt-get install qemu-kvm
#3、安装libvmi https://github.com/libvmi/libvmi  (运行：./install_libvmi.sh)
##安装pdbparse
	git clone https://github.com/moyix/pdbparse.git
	cd pdbparse && python setup.py install && cd ..
##安装json-c：https://github.com/json-c/json-c
	git clone https://github.com/json-c/json-c.git
	cd json-c && sh autogen.sh && ./configure && make && make install && make check && cd ..
	
##编译libvmi
	cd libvmi && ./autogen.sh && ./configure -build=x86_64-linux-gnu --disable-xen
	make && make install && ldconfig && cd ..
##安装libvmi-python（运行：./install_pyvmi.sh）
	apt install zip python-pip
	pip install --upgrade setuptools pip wheel 
	pip install enum construct==2.5.5-reupload future
	cd libvmi/tools/pyvmi && cp ../../libvmi/*.h /usr/local/include/libvmi && python setup.py build && python setup.py install && cd ../../../
	
###测试是否成功 ：	
	python 
	import libvmi
###测试libvmi是否安装成功
###测试linux虚拟机(获取偏移运行: ssh_withoutpwd.sh VM_ip && ./get_offset_linux.sh VM_ip)
	1、在宿主机里面新建libvmi的配置文件，/etc/libvmi.conf
	
	2、将libvmi/tools/linux-offset-finder文件夹复制到虚拟机master：（如果在宿主机下远程执行命令不成功，则进入master，root 用户下编译，编译时可能需要安装4.9及其以上的gcc：）
		cd libvmi
		scp -r ./tools/linux-offset-finder root@192.168.122.56:/root
		ssh root@192.168.122.56 "apt-get update && apt-get install gcc make && cd linux-offset-finder && make && insmod findoffsets.ko && rmmod findoffsets.ko && dmesg"
		可以看到相关的信息,
			ostype = "Linux";
			sysmap = "[insert path here]";
			linux_name = 0x600;
			linux_tasks = 0x350;
			linux_mm = 0x3a0;
			linux_pid = 0x448;
			linux_pgd = 0x40;
		sysmap需要：ls /boot 此处为：/boot/System.map-4.4.0-116-generic
	3、最终宿主机的libvmi.conf内容为：
			master {
				ostype = "Linux";
				sysmap = "/boot/System.map-4.4.0-116-generic";
				linux_name = 0x600;
				linux_tasks = 0x350;
				linux_mm = 0x3a0;
				linux_pid = 0x448;
				linux_pgd = 0x40;
			}
	4、把虚拟机下的/boot/System.map-4.4.0-116-generic复制到宿主机的/boot目录下（如宿主机的目录下有此内核版本的，则可忽略此步）
		scp root@192.168.122.56:/boot/System.map-4.4.0-116-generic /boot
	5、测试linux：libvmi/examples# ./vmi-process-list master
	
	6、测试windows，安装好windows虚拟机后（运行：./get_offset_windows.sh domain_name）：
		libvmi/examples# ./dump-memory win win.dd
		mv win.dd ../tools/windows-offset-finder/
		cd ../tools/windows-offset-finder
		chmod u+x *.sh
		./getoffset.sh domain_name
	之后将获取到的偏移按照linux的格式添加，测试:libvmi/examples# ./vmi-process-list domain_name
		win {
			ostype = "Windows";
			win_tasks   = 0x188;
			win_pdbase  = 0x28;
			win_pid     = 0x180;
			win_pname   = 0x2e0;
			//下面这三个字段是第一次运行./vmi-process-list win时推荐设置的，速度会很快
			win_ntoskrnl= 0x3a16000;
			win_kdbg    = 0x1f10a0;
			win_kdvb=0xfffff80003c070a0;
		}

#4、安装volatility(安装依赖：./install_volatility.sh；测试linux：./make_dwarf.sh VM_ip)
##安装依赖
	pip install PyCrypto Distorm3 ujson openpyxl IPython && easy_install --upgrade pytz
	git clone https://github.com/FreddieWitherden/libforensic1394.git
	cd libforensic1394 && mkdir build && cd build && cmake -G"Unix Makefiles" ../ && make && make install && cd ../../
    #下载yara
	wget https://github.com/VirusTotal/yara/archive/v3.7.1.tar.gz
	tar zxvf v3.7.1.tar.gz && cd yara-3.7.1 && ./bootstrap.sh && ./configure && make && make install && ldconfig && cd ..
	#下载PIL：
	wget http://effbot.org/media/downloads/Imaging-1.1.7.tar.gz
	tar zxvf Imaging-1.1.7.tar.gz && cd Imaging-1.1.7 && python setup.py install && cd ..

	git clone https://github.com/volatilityfoundation/volatility.git			
	cd volatility
	##测试
	##测试linux虚拟机：
		#将volatility中的文件夹复制到虚拟机master中，并在虚拟机中生成dwarf，并将其拷贝到宿主机中
			scp -r tools/linux/ root@192.168.122.56:/root
			ssh root@192.168.122.56 "apt-get install dwarfdump && cd linux && make"
			scp root@192.168.122.56:/root/linux/module.dwarf tools/linux/
		zip ./volatility/plugins/overlays/linux/master ./tools/linux/module.dwarf /boot/System.map-4.4.0-116-generic
		##将python-libvmi下的vmi.py文件复制到volatility
			cp ../python-libvmi/volatility/vmi.py ./volatility/plugins/addrspaces/
		##查看profile信息
			python vol.py --info|grep Linux
		##运行
			python vol.py -l vmi://master --profile=Linuxmasterx64 linux_pslist
	##测试windows虚拟机：到https://github.com/volatilityfoundation/volatility/wiki/2.6-Win-Profiles
	上查看对应windows版本的profile名称，可忽略以下三步：
			libvmi/examples# ./vmi-dump-memory win win.dd
			libvmi/examples# mv ./win.dd ../../volatility/
			查看建议的profile信息：
			vol.py -f win.dd imageinfo
		python vol.py --profile=Win7SP1x64 -l vmi://win pslist
		
	
++++++++++++++++++++++++++++++++++++获取windows虚拟机内存使用需要安装virtio-win驱动++++++++++++++++++++++++++++++++++++++++++++++++++++++
1、下载包含驱动的ISO
	https://fedorapeople.org/groups/virt/virtio-win/direct-downloads/archive-virtio/virtio-win-0.1.141-1/virtio-win-0.1.141.iso

2、挂载到windows光驱
	#1、挂载（如不能直接挂载见步骤##3）：
		virsh attach-disk win /root/virtio-win-0.1.141.iso hdc --type cdrom --mode readonly --live
	#2、弹出（本质是更新为一个空的）
		virsh attach-disk win "" hdc --type cdrom --mode readonly --live
	##3、不能直接挂载，则需要添加一个设备：
		参考链接：https://docs-old.fedoraproject.org/en-US/Fedora/18/html/Virtualization_Administration_Guide/sect-Attaching_and_updating_a_device_with_virsh.html
		vim /etc/libvirt/qemu/win-device.xml
		==================paste=========================
			<disk type='block' device='cdrom'>
			  <driver name='qemu' type='raw'/>
			  <target dev='hdc' bus='ide'/>
			  <readonly/>
			  <alias name='ide0-1-0'/>
			  <address type='drive' controller='0' bus='1' unit='0'/>
			</disk>
		===========================================
		virsh attach-device win /etc/libvirt/qemu/win-device.xml
3、挂载到虚拟机内部之后，进入虚拟机安装相应的驱动，见文档"windows balloon驱动安装.doc"
	参考链接：https://blog.csdn.net/signmem/article/details/78282624
4、测试是否成功
	virsh dommemstat win
	#示例结果：
		actual 2097152
		swap_in 710892
		swap_out 52224
		major_fault 22801
		minor_fault 786807
		unused 1571184
		available 2096752
		rss 2125968
	
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

=================================libvirt qemu-kvm相关 开始========================
#查看自带网桥是否启动，若否则启动libvirt自带的网桥virbr0
	ifconfig virbr0
	virsh net-start default

#添加网桥（libvirt自带的virbr0启动失败时，自己创建网桥br0，临时生效）
	brctl addbr br0
	ifconfig br0 自定义ip netmask 255.255.255.0
	#或者（永久）/etc/network/interfaces
		auto eth1
		iface eth1 inet manual

		auto br0
		iface br0 inet static
		address 192.168.75.135
		network 192.168.75.0
		netmask 255.255.255.0
		broadcast 192.168.75.255
		#gateway 192.168.75.2
		dns-nameservers 114.114.114.114 8.8.8.8 8.8.4.4
		bridge_ports eth1
		bridge_stp on
		bridge_fd 0
		bridge_maxwait 0
		bridge_maxage 12
#使用相关
	虚拟机配置文件默认路径：/etc/libvirt/qemu/
	磁盘文件默认路径：/var/lib/libvirt/images
		qemu-img create -f qcow2 /var/lib/libvirt/images/master.img 8G
	#启动libvirtd：
		libvirtd -d
		virtlogd -d
	#查看vnc端口：
		virsh vncdisplay master
	#定义虚拟机
		virsh define master.xml
	#启动：
		virsh start master
	#关机
		virsh shutdown master
	#dump内存
		virsh dump master master.dd --memory-only --live
	#查看客户机win的log
		cat /var/log/libvirt/qemu/win.log
#安装虚拟机，使用之前的xml文件,见本目录下：
	qemu-img create -f qcow2 /var/lib/libvirt/images/master.img 8G
	virsh define /etc/libvirt/qemu/master.xml
	virsh start master
	##注意，安装过程中需要选择:"Guided - use entire disk"
	
	qemu-img create -f qcow2 /var/lib/libvirt/images/win.img 20G
	virsh define /etc/libvirt/qemu/win.xml
	virsh start win

	
#kvm内核模块查看、卸载与挂载（宿主机需要打开虚拟化支持，如果宿主机是虚拟机，需要在VMware中设置CPU（虚拟化Intel VT-x/EPT））
	lsmod |grep kvm
	modprobe -r kvm_intel
	modprobe kvm_intel
#卸载qemu
	find / -name qemu
	rm -r /usr/local/include/qemu /usr/local/share/qemu
	rm /usr/local/bin/qemu*
=================================libvirt qemu相关 结束========================


----------------遇到的错误及解决方式--------------------

1、Traceback (most recent call last):
	  File "/usr/bin/pip", line 9, in <module>
		from pip import main
	ImportError: cannot import name main
	解决：https://stackoverflow.com/questions/28210269/importerror-cannot-import-name-main-when-running-pip-version-command-in-windo
	
2、若：virsh: /usr/lib/x86_64-linux-gnu/libvirt.so.0: version `LIBVIRT_PRIVATE_4.4.0' not found (required by virsh)
	vim /etc/ld.so.conf.d/libc.conf
	添加在第二行
	/usr/lib






