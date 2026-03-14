FROM ghcr.io/primordialomegazero/recursive-autonomous-system/base:latest

WORKDIR /app
COPY . .
RUN mkdir -p build && cd build && \
    cmake .. && \
    make -j$(nproc)

EXPOSE 4724 8080 33000
CMD ["./build/appium_recursive"]
