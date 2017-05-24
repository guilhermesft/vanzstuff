FROM debian:jessie

RUN apt-get update -y
RUN apt-get install -y libxmlsec1 libxmlsec1-dev libxmlsec1-openssl \
	python build-essential

# compile the nodejs 4.4.4
ADD http://nodejs.org/dist/v4.4.4/node-v4.4.4.tar.gz /
RUN tar -xvf node-v4.4.4.tar.gz
RUN rm node-v4.4.4.tar.gz
WORKDIR /node-v4.4.4
RUN python configure
RUN make
RUN make install

RUN npm install -g node-gyp restify

COPY  bugtls.js /
COPY native/ /native

WORKDIR /native
RUN node-gyp configure build
RUN cp /native/build/Release/xmlsecnode.node /usr/local/lib/node_modules/

WORKDIR /

ENV NODE_PATH="/usr/local/lib/node_modules/"

CMD ["bash"]

