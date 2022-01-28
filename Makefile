###
###    macOS
###
#CFLAGS = -DMacOSX -fPIC
#USBCOM = usbcom_mac.o
#LIBS=-framework CoreFoundation -framework IOKit
#ICONV=-liconv

###
###    libusb-0.1 installed in system directory. 
###
#CFLAGS = -DLIBUSB01 -fPIC
#USBCOM = usbcom_libusb01.o
#LIBS=-lusb
#ICONV=

###
###    libusb-0.1 installed from source code.
###
###    https://sourceforge.net/projects/libusb/files/libusb-0.1%20%28LEGACY%29/0.1.12/
###    -- This can still be compied in RHEL8 (without -Werror option to gcc).
###
CFLAGS = -DLIBUSB01 -fPIC
USBCOM = usbcom_libusb01.o
LIBS=-Wl,-rpath /usr/local/lib -L/usr/local/lib -lusb
ICONV=

###
###    libusb-1.0 installed in system directory.
###
#CFLAGS = -DLIBUSB10 -fPIC
#USBCOM = usbcom_libusb10.o
#LIBS=-lusb-1.0
#ICONV=


all : libcp2130.a libcp2130-config test_cp2130 test_cp2130ek 

libcp2130-config : gen-libcp2130-config.sh
	sh ./gen-libcp2130-config.sh "$(USBCOM)" "$(LIBS)" > libcp2130-config
	chmod +x libcp2130-config

check_cp2130 : test_cp2130
	./test_cp2130

test_cp2130ek : test_cp2130ek.o cp2130.o
	cc -o test_cp2130ek test_cp2130ek.o cp2130.o print_dev_info.o $(USBCOM) $(LIBS) $(ICONV)

test_cp2130 : cp2130.c  print_dev_info.o $(USBCOM)
	cc -o test_cp2130 -DCP2130_MAIN cp2130.c print_dev_info.o $(USBCOM) $(LIBS) $(ICONV)

libcp2130.a : $(USBCOM) cp2130.o
	rm -f  libcp2130.a
	ar cru libcp2130.a $(USBCOM) cp2130.o
	ranlib libcp2130.a

libusbcom.a : $(USBCOM)
	rm -f  libusbcom.a
	ar cru libusbcom.a $(USBCOM)
	ranlib libusbcom.a

cp2130.o : cp2130.c
print_dev_info.o : print_dev_info.c
usbcom_mac.o      : usbcom_mac.c      usbcom.h
usbcom_libusb01.o : usbcom_libusb01.c usbcom.h
usbcom_libusb10.o : usbcom_libusb10.c usbcom.h

clean:
	-rm -f libusbcom.a libcp2130.a libcp2130-config *.o *~ fo_libusb01 listdevs test_cp2130 test_cp2130ek
