echo "============================begin install pyvmi============================"
sleep 2

apt install zip python-pip
pip install --upgrade setuptools pip wheel

patch /usr/bin/pip -p0 < pip.patch
pip install enum construct==2.5.5-reupload future
cd libvmi/tools/pyvmi
python setup.py build
python setup.py install
cd ../../../

echo "============================pyvmi install end============================"
sleep 2
