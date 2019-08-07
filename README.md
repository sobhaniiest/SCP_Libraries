# SCP_Libraries

# Build and Installation

$ autoreconf -fi
$ ./configure
$ make
$ sudo make install
$ sudo ldconfig

# Testing the library

$ cd test/
$ make
$ ./scp_service_test 'xid' 'device_uri' 'device_id' 'printer_name' 'ppd_file_name'
