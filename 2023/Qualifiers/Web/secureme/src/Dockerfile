FROM node:20-bullseye-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    libglib2.0-0 libnss3 libatk1.0-0 libatk-bridge2.0-0 libcups2 libdrm2 \
    libxkbcommon0 libxcomposite1 libxdamage1 libxrandr2 libgbm1 libgtk-3-0 \
    libasound2 libxshmfence1 libx11-xcb1

WORKDIR /app
COPY .puppeteerrc.cjs package.json package-lock.json ./
RUN npm i
RUN npm i dotenv

# ENV FLAG 'PCC{FLAG_HERE}'

COPY index.js index.html ./
COPY views/ ./views/
EXPOSE 8000
RUN touch /app/.env && chmod 777 /app/.env

# CMD [ "node", "index.js" ]
# For setting up the dynamic flagging.
COPY docker-entrypoint.sh /tmp/docker-entrypoint.sh
RUN chmod +x /tmp/docker-entrypoint.sh && chown node:node /tmp/docker-entrypoint.sh
USER node
ENTRYPOINT [ "/tmp/docker-entrypoint.sh" ]
