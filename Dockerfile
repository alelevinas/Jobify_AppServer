# base image Ubuntu
FROM ubuntu

# Install required packages
RUN apt-get update && apt-get install -y \
  autoconf \
  cmake \
  curl \
  git \
  g++ \
  libbz2-dev \
  libcurl4-openssl-dev \
  libgflags-dev \
  libsnappy-dev \
  libssl-dev \
  libtool \
  make \
  openssl \
  python \
  zlib1g-dev


# Clone repository and checkout AppServer
RUN git clone https://github.com/alelevinas/Jobify_AppServer.git

# Install levelDB
RUN apt-get install -y libleveldb-dev

# Install crypto
RUN apt-get install -y libcrypto++9v5 libcrypto++9v5-dbg libcrypto++-dev

# Install libjansson
RUN apt-get install -y libjansson4 libjansson-dev libjansson-dbg

# Install coveralls
RUN apt-get install -y python-pip
RUN pip install cpp-coveralls

# Install cpprest
RUN git clone https://github.com/mrtazz/restclient-cpp.git
RUN cd restclient-cpp; ./autogen.sh; ./configure; make install; export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH;

# Make
RUN cd Jobify_AppServer; mkdir build; cd build; cmake ..; make;

# LDCONFIG
RUN ldconfig

# Expose ports
EXPOSE 8000

# Run tests
#CMD Jobify_AppServer/build/test/run_tests

# Run server
CMD Jobify_AppServer/build/Jobify_AppServer
