###
###    macOS
###
#CFLAGS = -DMacOSX
#USBCOM = usbcom_mac.o
#LIBS=-framework CoreFoundation -framework IOKit

###
###    libusb-0.1
###
CFLAGS = -DLIBUSB01
USBCOM = usbcom_libusb01.o
LIBS=-lusb

###
###    libusb-1.0
###
#CFLAGS = -DLIBUSB10
#USBCOM = usbcom_libusb10.o
#LIBS=-lusb-1.0

##

all : test_cp2130 test_cp2130ek

check_cp2130 : test_cp2130
	./test_cp2130

test_cp2130ek : test_cp2130ek.o cp2130.o
	cc -o test_cp2130ek test_cp2130ek.o cp2130.o print_dev_info.o $(USBCOM) $(LIBS) -liconv

test_cp2130 : cp2130.c  print_dev_info.o $(USBCOM)
	cc -o test_cp2130 -DCP2130_MAIN cp2130.c print_dev_info.o $(USBCOM) $(LIBS) -liconv

libcp2130.a : $(USBCOM) cp2130.o
	ar cru libcp2130.a $(USBCOM) cp2130.o
	ranlib libcp2130.a

libusbcom.a : $(USBCOM)
	ar cru libusbcom.a $(USBCOM)
	ranlib libusbcom.a

cp2130.o : cp2130.c
print_dev_info.o : print_dev_info.c
usbcom_mac.o      : usbcom_mac.c      usbcom.h
usbcom_libusb01.o : usbcom_libusb01.c usbcom.h
usbcom_libusb10.o : usbcom_libusb10.c usbcom.h

clean:
	-rm -f libusbcom.a *.o *~ fo_libusb01 listdevs test_cp2130 test_cp2130ek
