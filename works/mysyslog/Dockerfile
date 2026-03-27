FROM ubuntu:latest
COPY --parents * /root/
RUN <<EOF
    apt update -y
    apt install -y gcc make tree
EOF
WORKDIR /root
RUN make
CMD ["/bin/bash"]
