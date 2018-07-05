echo "====================create ubuntu========================="
sleep 2
qemu-img create -f qcow2 /var/lib/libvirt/images/master.img 8G
cp master.xml /etc/libvirt/qemu/master.xml
virsh define /etc/libvirt/qemu/master.xml

