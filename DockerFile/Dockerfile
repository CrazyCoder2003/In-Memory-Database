FROM ubuntu:22.04 AS builder

RUN apt-get update && \
    apt-get install -y build-essential cmake clang

WORKDIR /app
COPY . .
RUN mkdir build && cd build && \
    cmake .. && \
    make -j4

FROM ubuntu:22.04
RUN apt-get update && apt-get install -y libstdc++6
COPY --from=builder /app/build/src/main /app/in-memory-db

EXPOSE 8080
CMD ["/app/in-memory-db"]