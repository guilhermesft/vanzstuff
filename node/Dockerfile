FROM node:4

RUN apt-get update -y
RUN apt-get install -y libxmlsec1 libxmlsec1-dev libxmlsec1-openssl \
	python build-essential libxslt1.1 libxslt1-dev

RUN npm install -g node-gyp restify

COPY  bugtls.js /
COPY native/ /native

WORKDIR /native
RUN node-gyp configure build
RUN cp /native/build/Release/xmlsecnode.node /usr/local/lib/node_modules/

WORKDIR /

ENV NODE_PATH="/usr/local/lib/node_modules/"

CMD ["node", "bugtls.js"]

