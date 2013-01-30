------------------------------------------------------------------------
USB PERMISSION FOR HEAVYBOX HICU
cat /etc/udev/rules.d/60-avrisp.rules 
SUBSYSTEM!="usb_device", ACTION!="add", GOTO="avrisp_end"  
ATTRS{idVendor}=="16c0", ATTRS{idProduct}=="05dc", GROUP="psc", MODE="0666"
ATTR {idVendor} == "16c0", ATTR {idProduct} == "05dc", GROUP = "psc", MODE = "0666"
LABEL="avrisp_end"

------------------------------------------------------------------------
