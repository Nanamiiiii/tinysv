FROM ubuntu:jammy
RUN apt update && apt upgrade -y
RUN apt install -y build-essential cmake libexpat1-dev git
ARG UID=1000
ARG USER=docker
ARG PASSWORD=docker
RUN useradd -m --uid ${UID} --groups sudo ${USER} \
    && echo ${USER}:${PASSWORD} | chpasswd
USER ${USER}
WORKDIR /workspace
COPY . .

