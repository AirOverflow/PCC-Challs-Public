FROM node:latest

WORKDIR /app

COPY . .

COPY --chmod=550 --chown=root:root --from=theflash2k/showdown-submitter:latest /app/submitter /
RUN npm install

EXPOSE 1337

ENTRYPOINT [ "node", "index.js" ]
