FROM alpine:3.22 AS build
RUN apk add --no-cache build-base cmake lua5.4-dev openssl-dev curl-dev pkgconf
WORKDIR /src
COPY . .
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build && ctest --test-dir build
FROM alpine:3.22
RUN apk add --no-cache lua5.4-libs libssl3 libcrypto3 libcurl ca-certificates && adduser -D -H luca
COPY --from=build /src/build/luca /usr/local/bin/luca
COPY scripts /opt/luca/scripts
USER luca
ENTRYPOINT ["luca"]
