FROM ubuntu:latest
RUN <<EOF
    apt update -y
    apt install -y gcc make tree gdb libjson-c-dev pkg-config systemd tmux debhelper
EOF
WORKDIR /root
CMD ["/bin/bash"]
