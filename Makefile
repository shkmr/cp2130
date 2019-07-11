###
### libusb-0.1
###
#CFLAGS = -DLIBUSB
#OFILES = usbcom_libusb.o

###
### libusb-1.0
###
#CFLAGS = -DLIBUSB10
#OFILES = usbcom_libusb10.o

###
###   MacOSX
###
#CFLAGS = -DMacOSX
#OFILES = usbcom_mac.o

all : fo_libusb01 listdevs

fo_libusb01 : fo_libusb01.c
	cc -o fo_libusb01 fo_libusb01.c -lusb

listdevs : listdevs.c
	cc -o listdevs listdevs.c -lusb-1.0

xxx : libusbcom.a

libusbcom.a : $(OFILES)
	ar cru libusbcom.a $(OFILES)
	ranlib libusbcom.a

usbcom_mac.o      : usbcom_mac.c      usbcom.h
usbcom_libusb.o   : usbcom_libusb.c   usbcom.h
usbcom_libusb10.o : usbcom_libusb10.c usbcom.h

clean:
	-rm -f libusbcom.a *.o *~ fo_libusb01 listdevs
