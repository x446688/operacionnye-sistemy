FROM ubuntu:latest
RUN useradd user
RUN apt -y update && apt -y install\
    gcc\
    nano\
    libncurses5-dev\
    libncursesw5-dev\
    gdb
USER user
WORKDIR /home/user
CMD ["bash"]
