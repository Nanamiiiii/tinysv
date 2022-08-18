FROM debian:bullseye as builder
RUN apt update && apt upgrade -y
RUN apt install -y build-essential libexpat1-dev wget
RUN wget https://github.com/Kitware/CMake/releases/download/v3.24.1/cmake-3.24.1-linux-x86_64.sh
RUN chmod +x cmake-3.24.1-linux-x86_64.sh && \
    ./cmake-3.24.1-linux-x86_64.sh --skip-license
WORKDIR /tinysv
COPY . .
RUN mkdir build && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release .. && make 

FROM debian:bullseye as runner
RUN apt update && apt install -y libexpat1
COPY --from=builder /tinysv/build/tinysv /tinysv
WORKDIR /tinysv
EXPOSE 5000
ENTRYPOINT ["/tinysv/tinysv"]

