#!/bin/sh

if [ $(which gauche-config) ]; then
    prefix=$(gauche-config --prefix)
else
    prefix=/usr/local
fi

USBCOM="$1"
LIBS="$2"

cat <<EOF
#!/bin/sh

case \$1 in
prefix) echo "${prefix}";;
usbcom) echo "${USBCOM}";;
libs)   echo "${LIBS}";;
esac

EOF
