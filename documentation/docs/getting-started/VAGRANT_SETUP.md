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





##Vagrant Initial Setup

NOTE: You don't need to follow these steps. These are use to create a vagrant box

###CREATING  A VAGRANT ENVIRONMENT

```
* vagrant init ubuntu/trusty64  #creates vagrant config file
* vagrant up                    #(downloads)boots the virtual machine. Do this where vagrant config file is present
* vagrant ssh                                         #ssh into vagrant machine
* vagrant halt                                        #shutdown the virtual machine
* vagrant package --output package.box                #create a vagrant box of your env
* vagrant box add --name ubuntu/trusty64 package.box  #create virtual machine from local box

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
