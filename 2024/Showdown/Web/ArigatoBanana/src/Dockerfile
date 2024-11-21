FROM node:20

WORKDIR /app

COPY --chmod=550 --chown=root:root --from=theflash2k/showdown-submitter:latest /app/submitter /
COPY package.json /app
RUN npm install

COPY . /app

EXPOSE 1337

CMD ["npm", "start"]