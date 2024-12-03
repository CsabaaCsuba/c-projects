### avra is a different type of assembler
sudo apt-get install gcc build-essential gcc-avr binutils-avr avr-libc gdb-avr avrdude libusb-dev avra

#disable brltty (it can block the avr-reader usb) [it's a background program for blind people's braille keyboard]
systemctl stop brltty-udev.service
sudo systemctl mask brltty-udev.service
systemctl stop brltty.service
systemctl disable brltty.service