echo "============================begin install libvmi============================"
sleep 2
cd pdbparse && python setup.py install && cd ..
cd json-c && chmod u+x *.sh && sh autogen.sh && ./configure && make && make install && cd ..
cd libvmi && chmod u+x *.sh && ./autogen.sh && ./configure -build=x86_64-linux-gnu --disable-xen && make && make install && ldconfig && cd ..

echo "============================libvmi install end============================"
sleep 2
