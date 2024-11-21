import express from "express";
import bodyParser from "body-parser";
import { nanoid } from "nanoid";
import path from "path";
import puppeteer from "puppeteer";

// var dotenv = require('dotenv').config();

const sleep = (ms) => new Promise((res) => setTimeout(res, ms));

const __dirname = path.resolve(path.dirname(""));
const app = express();
const port = 8000;

app.set("view engine", "ejs");
app.use(bodyParser.json());

const browser = puppeteer.launch({
  pipe: true,
  args: [
    "--no-sandbox",
    "--disable-dev-shm-usage"
  ],
});
const resolutions = new Map();

app.get("/", (req, res) => {
  res.sendFile(path.join(__dirname, `/index.html`));
});

app.post("/save", (req, res) => {
  const id = nanoid();
  if (!req.body.name) return res.status(400).send("There has to be a name for submission");
  if (!req.body.description) return res.status(400).send("There has to be a description for submission");
  if (req.body.name.length > 150)
    return res.status(400).send("The provided name is too long");
  if (req.body.description.length > 2000)
    return res.status(400).send("The provided description is too long");

  resolutions.set(id, req.body);
  res.send("Your Resolution ID is: " + id);
});

app.get("/:resolution", (req, res) => {
  const resolution = resolutions.get(req.params.resolution);
  if (!resolution) return res.status(404).send("not found");

  res.render("resolutions", {
    id: req.params.resolution,
    name: encodeURIComponent(resolution.name),
    description: encodeURIComponent(resolution.description),
  });
});

app.get("/view/:resolution", async (req, res) => {
  let ctx;
  try {
    ctx = await (await browser).createIncognitoBrowserContext();
    const visit = async (browser, resolution) => {
      console.log(`Visting ${resolution}`);
      console.log(`ENV set: ${process.env.FLAG}`);
      const page = await browser.newPage();
      await page.goto("http://localhost:8000");
      await page.setCookie({ name: "pcc", value: process.env.FLAG });
      await page.goto(`http://localhost:8000/${resolution}`);
      await page.waitForNetworkIdle({ timeout: 6000 });
      await page.close();
    };

    await Promise.race([visit(ctx, req.params.resolution), sleep(10_000)]);
  } catch (err) {
    console.error("Error thrown", err);
    if (ctx) {
      try {
        await ctx.close();
      } catch (e) {}
    }
    return res.send("Error while visiting the page");
  }
  if (ctx) {
    try {
      await ctx.close();
    } catch (e) {}
  }
  return res.send("Resolution visited");
});

app.listen(port, () => {
  console.log(`App is listening on ${port}`);
});
