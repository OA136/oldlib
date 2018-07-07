apt-get install autoconf-archive check ebtables dnsmasq bridge-utils libnl-route-3-dev libnl-3-dev libxml2-dev libyajl-dev libxml2-utils xsltproc libdevmapper-dev libpciaccess-dev automake libxml++2.6-2v5 libxml++2.6-dev libxml-xpath-perl libglib2.0-dev libtool autoconf automake flex bison libfuse-dev libjson-c-dev python-dev libncurses5-dev python3-dev libfuse-dev libxc1 libffi-dev cmake libcurl4-openssl-dev autoconf libtool bison flex libpixman-1-dev zlib1g-dev libglib2.0-dev libsnappy-dev libgtk-3-dev libsdl2-dev libjpeg-turbo8-dev libspice-server-dev cabextract mscompress python-pefile

echo "==================================apt-get install end============================="
sleep 2


echo "============================begin install libvirt============================"
sleep 2
tar zxvf libvirt-1.3.1.tar.gz && cd libvirt-1.3.1 && ./configure --prefix=/usr --localstatedir=/var --sysconfdir=/etc && make && make install && cd ..
echo "============================libvirt install end============================"
sleep 2

echo "============================begin install qemu============================"
sleep 2
apt-get install qemu-kvm
libvirtd -d
virtlogd -d
virsh version
echo "============================qemu install end============================"
sleep 2
