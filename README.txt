===========��������Ubuntu-16.04.4-server-amd64 �ں˰汾��4.4.0-62-generic==========
===========�������Ubuntu-16.04.4-server-amd64 �ں˰汾��4.4.0-116-generic==========
===========Win7��cn_windows_7_enterprise_with_sp1_x64_dvd_620654.iso ===============
##ѧ��github
https://github.com/lixingchen12138/libvmi-volatility-master
https://github.com/lixingchen12138/monitor_server
##����ubuntuԴ��/etc/apt/sources.list  https://mirrors.tuna.tsinghua.edu.cn/help/ubuntu/
##root�û���

mkdir sq
cd sq
������
	�����ر�Ҫ�⣬��װlibvirt��qemu��libvmi��volatility����������������в���libvmi���ԡ�volatility����
	./download.sh && ./install_libvirt_qemu.sh && ./install_libvmi.sh ./install_pyvmi.sh
	
	�����������./create_master.sh && ./create_win.sh
	
	��ȡlinuxƫ�ƣ�	./ssh_withoutpwd.sh VM_ip && ./get_offset_linux.sh VM_ip
	
	��ȡwindowsƫ�ƣ�./get_offset_windows domain_name
	
	����volatility��./install_volatility.sh && linux��./make_dwarf.sh VM_ip
	
	
#���أ�������: ./download.sh��
wget https://libvirt.org/sources/libvirt-1.3.1.tar.gz
git clone https://github.com/moyix/pdbparse.git
git clone https://github.com/json-c/json-c.git
git clone https://github.com/FreddieWitherden/libforensic1394.git
wget https://github.com/VirusTotal/yara/archive/v3.7.1.tar.gz
wget http://effbot.org/media/downloads/Imaging-1.1.7.tar.gz
git clone https://github.com/volatilityfoundation/volatility.git

#1����װlibvirt-1.3.1��qemu��(���У�./install_libvirt_qemu.sh)
##����
	apt-get install check ebtables dnsmasq bridge-utils libnl-route-3-dev libnl-3-dev libxml2-dev libyajl-dev libxml2-utils xsltproc libdevmapper-dev libpciaccess-dev automake libxml++2.6-2v5 libxml++2.6-dev libxml-xpath-perl libglib2.0-dev libtool autoconf automake flex bison libfuse-dev libjson-c-dev python-dev libncurses5-dev python3-dev libfuse-dev libxc1 libffi-dev cmake libcurl4-openssl-dev autoconf libtool bison flex libpixman-1-dev zlib1g-dev libglib2.0-dev libsnappy-dev libgtk-3-dev libsdl2-dev libjpeg-turbo8-dev libspice-server-dev cabextract mscompress python-pefile
##����libvirt��
	tar zxvf libvirt-1.3.1.tar.gz && cd libvirt-1.3.1 && ./configure --prefix=/usr --localstatedir=/var --sysconfdir=/etc
	make
	make install
##��װqemu
	apt-get install qemu-kvm
#3����װlibvmi https://github.com/libvmi/libvmi  (���У�./install_libvmi.sh)
##��װpdbparse
	git clone https://github.com/moyix/pdbparse.git
	cd pdbparse && python setup.py install && cd ..
##��װjson-c��https://github.com/json-c/json-c
	git clone https://github.com/json-c/json-c.git
	cd json-c && sh autogen.sh && ./configure && make && make install && make check && cd ..
	
##����libvmi
	cd libvmi && ./autogen.sh && ./configure -build=x86_64-linux-gnu --disable-xen
	make && make install && ldconfig && cd ..
##��װlibvmi-python�����У�./install_pyvmi.sh��
	apt install zip python-pip
	pip install --upgrade setuptools pip wheel 
	pip install enum construct==2.5.5-reupload future
	cd libvmi/tools/pyvmi && cp ../../libvmi/*.h /usr/local/include/libvmi && python setup.py build && python setup.py install && cd ../../../
	
###�����Ƿ�ɹ� ��	
	python 
	import libvmi
###����libvmi�Ƿ�װ�ɹ�
###����linux�����(��ȡƫ������: ssh_withoutpwd.sh VM_ip && ./get_offset_linux.sh VM_ip)
	1���������������½�libvmi�������ļ���/etc/libvmi.conf
	
	2����libvmi/tools/linux-offset-finder�ļ��и��Ƶ������master�����������������Զ��ִ������ɹ��������master��root �û��±��룬����ʱ������Ҫ��װ4.9�������ϵ�gcc����
		cd libvmi
		scp -r ./tools/linux-offset-finder root@192.168.122.56:/root
		ssh root@192.168.122.56 "apt-get update && apt-get install gcc make && cd linux-offset-finder && make && insmod findoffsets.ko && rmmod findoffsets.ko && dmesg"
		���Կ�����ص���Ϣ,
			ostype = "Linux";
			sysmap = "[insert path here]";
			linux_name = 0x600;
			linux_tasks = 0x350;
			linux_mm = 0x3a0;
			linux_pid = 0x448;
			linux_pgd = 0x40;
		sysmap��Ҫ��ls /boot �˴�Ϊ��/boot/System.map-4.4.0-116-generic
	3��������������libvmi.conf����Ϊ��
			master {
				ostype = "Linux";
				sysmap = "/boot/System.map-4.4.0-116-generic";
				linux_name = 0x600;
				linux_tasks = 0x350;
				linux_mm = 0x3a0;
				linux_pid = 0x448;
				linux_pgd = 0x40;
			}
	4����������µ�/boot/System.map-4.4.0-116-generic���Ƶ���������/bootĿ¼�£�����������Ŀ¼���д��ں˰汾�ģ���ɺ��Դ˲���
		scp root@192.168.122.56:/boot/System.map-4.4.0-116-generic /boot
	5������linux��libvmi/examples# ./vmi-process-list master
	
	6������windows����װ��windows����������У�./get_offset_windows.sh domain_name����
		libvmi/examples# ./dump-memory win win.dd
		mv win.dd ../tools/windows-offset-finder/
		cd ../tools/windows-offset-finder
		chmod u+x *.sh
		./getoffset.sh domain_name
	֮�󽫻�ȡ����ƫ�ư���linux�ĸ�ʽ��ӣ�����:libvmi/examples# ./vmi-process-list domain_name
		win {
			ostype = "Windows";
			win_tasks   = 0x188;
			win_pdbase  = 0x28;
			win_pid     = 0x180;
			win_pname   = 0x2e0;
			//�����������ֶ��ǵ�һ������./vmi-process-list winʱ�Ƽ����õģ��ٶȻ�ܿ�
			win_ntoskrnl= 0x3a16000;
			win_kdbg    = 0x1f10a0;
			win_kdvb=0xfffff80003c070a0;
		}

#4����װvolatility(��װ������./install_volatility.sh������linux��./make_dwarf.sh VM_ip)
##��װ����
	pip install PyCrypto Distorm3 ujson openpyxl IPython && easy_install --upgrade pytz
	git clone https://github.com/FreddieWitherden/libforensic1394.git
	cd libforensic1394 && mkdir build && cd build && cmake -G"Unix Makefiles" ../ && make && make install && cd ../../
    #����yara
	wget https://github.com/VirusTotal/yara/archive/v3.7.1.tar.gz
	tar zxvf v3.7.1.tar.gz && cd yara-3.7.1 && ./bootstrap.sh && ./configure && make && make install && ldconfig && cd ..
	#����PIL��
	wget http://effbot.org/media/downloads/Imaging-1.1.7.tar.gz
	tar zxvf Imaging-1.1.7.tar.gz && cd Imaging-1.1.7 && python setup.py install && cd ..

	git clone https://github.com/volatilityfoundation/volatility.git			
	cd volatility
	##����
	##����linux�������
		#��volatility�е��ļ��и��Ƶ������master�У����������������dwarf�������俽������������
			scp -r tools/linux/ root@192.168.122.56:/root
			ssh root@192.168.122.56 "apt-get install dwarfdump && cd linux && make"
			scp root@192.168.122.56:/root/linux/module.dwarf tools/linux/
		zip ./volatility/plugins/overlays/linux/master ./tools/linux/module.dwarf /boot/System.map-4.4.0-116-generic
		##��python-libvmi�µ�vmi.py�ļ����Ƶ�volatility
			cp ../python-libvmi/volatility/vmi.py ./volatility/plugins/addrspaces/
		##�鿴profile��Ϣ
			python vol.py --info|grep Linux
		##����
			python vol.py -l vmi://master --profile=Linuxmasterx64 linux_pslist
	##����windows���������https://github.com/volatilityfoundation/volatility/wiki/2.6-Win-Profiles
	�ϲ鿴��Ӧwindows�汾��profile���ƣ��ɺ�������������
			libvmi/examples# ./vmi-dump-memory win win.dd
			libvmi/examples# mv ./win.dd ../../volatility/
			�鿴�����profile��Ϣ��
			vol.py -f win.dd imageinfo
		python vol.py --profile=Win7SP1x64 -l vmi://win pslist
		
	
++++++++++++++++++++++++++++++++++++��ȡwindows������ڴ�ʹ����Ҫ��װvirtio-win����++++++++++++++++++++++++++++++++++++++++++++++++++++++
1�����ذ���������ISO
	https://fedorapeople.org/groups/virt/virtio-win/direct-downloads/archive-virtio/virtio-win-0.1.141-1/virtio-win-0.1.141.iso

2�����ص�windows����
	#1�����أ��粻��ֱ�ӹ��ؼ�����##3����
		virsh attach-disk win /root/virtio-win-0.1.141.iso hdc --type cdrom --mode readonly --live
	#2�������������Ǹ���Ϊһ���յģ�
		virsh attach-disk win "" hdc --type cdrom --mode readonly --live
	##3������ֱ�ӹ��أ�����Ҫ���һ���豸��
		�ο����ӣ�https://docs-old.fedoraproject.org/en-US/Fedora/18/html/Virtualization_Administration_Guide/sect-Attaching_and_updating_a_device_with_virsh.html
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
3�����ص�������ڲ�֮�󣬽����������װ��Ӧ�����������ĵ�"windows balloon������װ.doc"
	�ο����ӣ�https://blog.csdn.net/signmem/article/details/78282624
4�������Ƿ�ɹ�
	virsh dommemstat win
	#ʾ�������
		actual 2097152
		swap_in 710892
		swap_out 52224
		major_fault 22801
		minor_fault 786807
		unused 1571184
		available 2096752
		rss 2125968
	
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

=================================libvirt qemu-kvm��� ��ʼ========================
#�鿴�Դ������Ƿ�����������������libvirt�Դ�������virbr0
	ifconfig virbr0
	virsh net-start default

#������ţ�libvirt�Դ���virbr0����ʧ��ʱ���Լ���������br0����ʱ��Ч��
	brctl addbr br0
	ifconfig br0 �Զ���ip netmask 255.255.255.0
	#���ߣ����ã�/etc/network/interfaces
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
#ʹ�����
	����������ļ�Ĭ��·����/etc/libvirt/qemu/
	�����ļ�Ĭ��·����/var/lib/libvirt/images
		qemu-img create -f qcow2 /var/lib/libvirt/images/master.img 8G
	#����libvirtd��
		libvirtd -d
		virtlogd -d
	#�鿴vnc�˿ڣ�
		virsh vncdisplay master
	#���������
		virsh define master.xml
	#������
		virsh start master
	#�ػ�
		virsh shutdown master
	#dump�ڴ�
		virsh dump master master.dd --memory-only --live
	#�鿴�ͻ���win��log
		cat /var/log/libvirt/qemu/win.log
#��װ�������ʹ��֮ǰ��xml�ļ�,����Ŀ¼�£�
	qemu-img create -f qcow2 /var/lib/libvirt/images/master.img 8G
	virsh define /etc/libvirt/qemu/master.xml
	virsh start master
	##ע�⣬��װ��������Ҫѡ��:"Guided - use entire disk"
	
	qemu-img create -f qcow2 /var/lib/libvirt/images/win.img 20G
	virsh define /etc/libvirt/qemu/win.xml
	virsh start win

	
#kvm�ں�ģ��鿴��ж������أ���������Ҫ�����⻯֧�֣���������������������Ҫ��VMware������CPU�����⻯Intel VT-x/EPT����
	lsmod |grep kvm
	modprobe -r kvm_intel
	modprobe kvm_intel
#ж��qemu
	find / -name qemu
	rm -r /usr/local/include/qemu /usr/local/share/qemu
	rm /usr/local/bin/qemu*
=================================libvirt qemu��� ����========================


----------------�����Ĵ��󼰽����ʽ--------------------

1��Traceback (most recent call last):
	  File "/usr/bin/pip", line 9, in <module>
		from pip import main
	ImportError: cannot import name main
	�����https://stackoverflow.com/questions/28210269/importerror-cannot-import-name-main-when-running-pip-version-command-in-windo
	
2������virsh: /usr/lib/x86_64-linux-gnu/libvirt.so.0: version `LIBVIRT_PRIVATE_4.4.0' not found (required by virsh)
	vim /etc/ld.so.conf.d/libc.conf
	����ڵڶ���
	/usr/lib






