echo "===========================volatility install begin==================================================="
sleep 2
tar zxvf 2.6.tar.gz && mv volatility-2.6 volatility
pip install PyCrypto Distorm3 ujson openpyxl IPython && easy_install --upgrade pytz
cd libforensic1394 && mkdir build && cd build && cmake -G"Unix Makefiles" ../ && make && make install && cd ../../
tar zxvf v3.7.1.tar.gz && cd yara-3.7.1 && ./bootstrap.sh && ./configure && make && make install && ldconfig && cd ..
tar zxvf Imaging-1.1.7.tar.gz && cd Imaging-1.1.7 && python setup.py install && cd ..
