echo "====================create windows========================="
sleep 2
qemu-img create -f qcow2 /var/lib/libvirt/images/win.img 20G
cp win.xml /etc/libvirt/qemu/win.xml
virsh define /etc/libvirt/qemu/win.xml

