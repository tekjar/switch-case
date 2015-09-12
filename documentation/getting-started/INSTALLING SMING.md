####INSTALLING SMING
---
* Clone this repository
```
git clone --recursive https://github.com/kteza1/hng-switchboard.git
```
* Setup prebuilt `esp-open-sdk`
```
cd ~/
curl -L -O https://bintray.com/artifact/download/kireevco/generic/esp-open-sdk-1.2-macos-x86_64.tar.gz
sudo mkdir -p /opt/esp-open-sdk
sudo tar -zxf esp-open-sdk-1.2-macos-x86_64.tar.gz -C /opt/esp-open-sdk
sudo chmod -R 775 /opt/esp-open-sdk
```
* Setup `spiffy`
```
cd spiffy
mkdir build
make
sudo cp build/spiffy /usr/bin/
sudo chmod ugo+x /usr/bin/spiffy
```

* Test an example in `Sming` with `make flash`
