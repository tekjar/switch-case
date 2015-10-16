FROM ubuntu:latest


# Last base packages update
LABEL BASE_PACKAGES_UPDATE="2015-07-11"
# ------------------------------------------------------------------------------
# Install base
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get clean && apt-get update && apt-get install -y build-essential g++ curl libssl-dev apache2-utils git libxml2-dev sshfs make autoconf automake libtool gcc g++ gperf flex bison texinfo gawk ncurses-dev libexpat-dev python sed python-serial srecord bc wget llvm-3.4 libclang1 libclang-dev mc vim screen

# ------------------------------------------------------------------------------
# Install sshd
RUN apt-get install -y openssh-server
RUN mkdir /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin without-password/PermitRootLogin yes/' /etc/ssh/sshd_config
RUN sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd
ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile


# ------------------------------------------------------------------------------
# Versions
ENV VERSION="1.2"
ENV SDK_VERSION "1.2"
ENV SPIFFY_VERSION "1.0.4"

LABEL description="version: ${VERSION}\nsdk: ${SDK_VERSION}\nspiffy: ${SPIFFY_VERSION}"

# ------------------------------------------------------------------------------
# Install spiffy
WORKDIR /tmp/
RUN wget https://bintray.com/artifact/download/kireevco/generic/spiffy-${SPIFFY_VERSION}-linux-x86_64.tar.gz && tar -zxf spiffy-${SPIFFY_VERSION}-linux-x86_64.tar.gz && mv spiffy /usr/local/bin/ && chmod +rx /usr/local/bin/spiffy


# ------------------------------------------------------------------------------
# Install esp-open-sdk
WORKDIR /tmp/
RUN mkdir -p /opt/esp-open-sdk
RUN wget https://bintray.com/artifact/download/kireevco/generic/esp-open-sdk-${SDK_VERSION}-linux-x86_64.tar.gz && tar -zxf esp-open-sdk-${SDK_VERSION}-linux-x86_64.tar.gz -C /opt/esp-open-sdk
RUN chmod +rx /opt/esp-open-sdk/sdk/tools/gen_appbin.py

# ------------------------------------------------------------------------------
# Install patched esptool
WORKDIR /opt/esp-open-sdk/esptool
RUN mv esptool.py esptool.py_orig
RUN wget https://raw.githubusercontent.com/nodemcu/nodemcu-firmware/master/tools/esptool.py
RUN chmod +rx esptool.py


# ------------------------------------------------------------------------------
# Clone Sming Core
RUN git clone https://github.com/SmingHub/Sming.git /opt/sming

# ------------------------------------------------------------------------------
# Clone ESPTOOL2
RUN git clone https://github.com/raburton/esp8266.git /opt/rbutron-esp8266
WORKDIR /opt/rbutron-esp8266/esptool2
RUN make

# ------------------------------------------------------------------------------
# Enviromnent settings
ENV ESP_HOME="/opt/esp-open-sdk"
ENV SMING_HOME="/opt/sming/Sming"
ENV CXX="/opt/esp-open-sdk/xtensa-lx106-elf/bin/xtensa-lx106-elf-g++"
ENV CC="/opt/esp-open-sdk/xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc"
ENV PATH="/opt/esp-open-sdk/xtensa-lx106-elf/bin:${PATH}"
ENV ESPTOOL2="/opt/rbutron-esp8266/esptool2"
ENV RBOOT_ENABLED=1

ENV COM_SPEED_ESPTOOL=230400

RUN env > /etc/environment

# ------------------------------------------------------------------------------
# Expose ports.
EXPOSE 22
EXPOSE 80
EXPOSE 10000

WORKDIR /home

CMD /bin/bash
