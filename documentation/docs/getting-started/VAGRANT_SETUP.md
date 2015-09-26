#Vagrant Setup

###CREATING  A VAGRANT ENVIRONMENT

```
* vagrant box add ubuntu/trusty64     #downloads the virtualbox image
* vagrant init ubuntu/trusty64        #creates vagrant config file
* vagrant up                          #boots the virtual machine. Do this where vagrant config file is present
* vagrant ssh                         #ssh into vagrant machine
* vagrant halt                        #shutdown the virtual machine
```


###BUILDING ESP-OPEN-SDK

* use ubuntu/trusty64 in vagrant file
* Increase ram size during installation of esp-open-sdk
```
config.vm.provider "virtualbox" do |v|
    v.memory = 1024
end
```
* Follow the instructions [here](https://github.com/pfalcon/esp-open-sdk#requirements-and-dependencies)
* Disable step 2 in Vagrantfile

######TROUBLESHOOT:
Follow these only if there is a failure in build after following above steps

* Add below extra repos to `/etc/apt/sources.list
`  if you aren't able to install every dependecy in the above step
```
deb http://ftp.us.debian.org/debian/ jessie main
deb http://ftp.us.debian.org/debian/ wheezy non-free
```
