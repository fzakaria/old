FROM ubuntu:22.04
# docker build -t old .
RUN apt-get update && apt-get install -y curl gnupg
RUN curl https://bazel.build/bazel-release.pub.gpg | apt-key add - && \
    echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | tee /etc/apt/sources.list.d/bazel.list
RUN apt-get update && apt-get install -y bazel
WORKDIR /code
COPY . /code
RUN bazel build src/main:libold.so
