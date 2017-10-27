FROM seastar:latest

RUN dnf install -y python3 ninja-build
RUN pip3 install meson

RUN mkdir app
COPY * app/

WORKDIR /app

RUN make release
RUN make

CMD ["/app/build/seastar_demo"]

