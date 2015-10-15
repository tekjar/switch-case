#### SETUP DOCKER

* docker build -t switch-case .

* Add USB-Serial interface in the virtualbox

* docker run --device=/dev/ttyUSB0 -v ~/Desktop/switch-case:/home/switch-case -it switch-case bash




##### DOCKER COMMANDS

- [ ] docker stop $(docker ps -a -q) --> Stops all containers
- [ ] docker rm $(docker ps -a -q)   --> Removes all containers
