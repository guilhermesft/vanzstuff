FROM node:4.4.4

RUN apt-get update -y
RUN apt-get install -y libxmlsec1 libxmlsec1-dev libxmlsec1-openssl

COPY . /node

CMD ["node", "/node/bugtls.js"]

