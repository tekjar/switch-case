##SWITCH-CASE VAGRANT SETUP
---

* Install [vagrant](https://www.vagrantup.com/), virtual box and get switch-case clone
```
git clone --recursive https://github.com/kteza1/switch-case
```
* Download, start and ssh into box
```
cd switch-case/Vagrant
vagrant up
vagrant ssh
```

#####TROUBLESHOOT:

- [x] Unable to find `/dev/ttyUSB0` when esp is plugged in.
1. Do `lsusb` in virtual machaing with and without esp plugged in. If don't see an extra interface when plugged in, make sure that you find usb-serial connection in your host machine when you do `VBoxManage list usbhost` and enter `product id` and `vendor id` in yml config file and do `vagrant halt` and `vagrant up`.
2. Open virtual box app and see if multiple virtual machines are running. Stop unnecessary running virtual machines

- [x] Unable to `connect` to USB serial.
1. Do `esp-console 115200`. If you see below message, some other resource is using the port
```
SerialException: device reports readiness to read but returned no data (device disconnected?)
```
* This seems to be a problem with below kernel
```
uname -r
3.13.0-63-generic
```



##Vagrant Initial Setup

NOTE: You don't need to follow these steps. These are used to create a vagrant box

###CREATING  A VAGRANT ENVIRONMENT

```
* vagrant init ubuntu/trusty64  #creates vagrant config file
* vagrant up                    #(downloads)boots the virtual machine. Do this where vagrant config file is present
* vagrant ssh                                         #ssh into vagrant machine
* vagrant halt                                        #shutdown the virtual machine
* vagrant package --output package.box                #create a vagrant box of your env
* vagrant box add --name kteza1/trusty64 package.box  #create virtual machine with a name from local box

```


###BUILDING ESP-OPEN-SDK

* use ubuntu/trusty64 in vagrant file
* Increase ram size during installation of esp-open-sdk
```
config.vm.provider "virtualbox" do |v|
    v.memory = "1024"
end
```
* Follow the instructions [here](https://github.com/pfalcon/esp-open-sdk#requirements-and-dependencies) plus
```
    sudo apt-get install unzip
    
    # This package contains usbserial kernel drivers
    # see http://ubuntu.5.x6.nabble.com/PATCH-0-2-linux-image-extra-support-td365008.html
    sudo apt-get -y install linux-image-extra-virtual
    
    # Give the vagrant user permissions for the serial ports
    sudo adduser vagrant dialout
```

* Set RAM to "256" in step 2 and restart the virtal machine 


######TROUBLESHOOT:
Follow these only if there is a failure in build after following above steps

* Add below extra repos to `/etc/apt/sources.list
`  if you aren't able to install every dependecy in the above step
```
deb http://ftp.us.debian.org/debian/ jessie main
deb http://ftp.us.debian.org/debian/ wheezy non-free
```
