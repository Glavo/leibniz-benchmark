FROM ubuntu:22.04
COPY . /leibniz
RUN apt update && apt install -y wget g++ gnupg python3 pypy3 nodejs &&\
    wget -q -O - https://download.bell-sw.com/pki/GPG-KEY-bellsoft | apt-key add - &&\
    echo 'deb [arch=amd64] https://apt.bell-sw.com/ stable main' | tee /etc/apt/sources.list.d/bellsoft.list &&\
    apt update && apt install -y bellsoft-java20 &&\
    wget -qO- https://go.dev/dl/go1.21.0.linux-amd64.tar.gz | tar -C /usr/local -xzv &&\
    wget -qO- https://download.oracle.com/graalvm/20/latest/graalvm-jdk-20_linux-x64_bin.tar.gz | tar -C /usr/local -xzv &&\
    /usr/local/graalvm-jdk-20.0.2+9.1/bin/gu install nodejs python
CMD export PATH=$PATH:/usr/local/go/bin &&\
    export GRAALVM_HOME=/usr/local/graalvm-jdk-20.0.2+9.1 &&\
    cd /leibniz && chmod +x run.sh && bash run.sh
