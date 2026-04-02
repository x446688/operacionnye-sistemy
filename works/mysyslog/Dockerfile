FROM ubuntu:latest
RUN <<EOF
    apt update -y
    apt install -y gcc make tree libjson-c-dev pkg-config
EOF
WORKDIR /root
CMD ["/bin/bash"]
