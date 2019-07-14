###
### libusb-0.1
###
CFLAGS = -DLIBUSB01
USBCOM = usbcom_libusb01.o
LIBS=-lusb

###
### libusb-1.0
###
#CFLAGS = -DLIBUSB10
#USBCOM = usbcom_libusb10.o
#LIBS=-lusb-1.0

###
###   MacOSX
###
#CFLAGS = -DMacOSX
#USBCOM = usbcom_mac.o
#LIBS=-framework CoreFoundation -framework IOKit

all : test_cp2130

check_cp2130 : test_cp2130
	./test_cp2130

test_cp2130 : cp2130.c $(USBCOM)
	cc -o test_cp2130 -DCP2130_MAIN cp2130.c $(USBCOM) $(LIBS) -liconv

libusbcom.a : $(USBCOM)
	ar cru libusbcom.a $(USBCOM)
	ranlib libusbcom.a

cp2130.o : cp2130.c
usbcom_mac.o      : usbcom_mac.c      usbcom.h
usbcom_libusb01.o : usbcom_libusb01.c usbcom.h
usbcom_libusb10.o : usbcom_libusb10.c usbcom.h

clean:
	-rm -f libusbcom.a *.o *~ fo_libusb01 listdevs test_cp2130
