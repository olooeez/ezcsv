FROM gcc:latest

WORKDIR /app

COPY . /app

RUN make

RUN make install
