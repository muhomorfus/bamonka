FROM gcc

COPY . /build
WORKDIR /build
RUN gcc *.c -o /opt/govnx

WORKDIR /static
ENTRYPOINT ["/opt/govnx"]