const express = require("express");
const { bootstrapDatabase, User, Product, Cart, supersecret } = require("./db");
const { sign, auth } = require("./auth");
const { exec } = require("child_process");

const app = express();

app.use(express.urlencoded({ extended: false }));

app.post("/register", async (req, res) => {
  const { username, password } = req.body;

  if (typeof username !== "string" || typeof password !== "string") {
    res.status(400).send({ message: "username and password must be strings!" });
    return;
  }

  if (!username || !password) {
    res.status(400).send({ message: "username or password is required!" });
    return;
  }

  if ((await User.countDocuments({ username })) !== 0) {
    res.status(400).send({ message: "username already exists!" });
    return;
  }

  const user = await User.create({ username, password, balance: 1000 });
  await Cart.create({ user, products: [] });
  res.send({ username, balance: user.balance, token: sign(username) });
});

app.post("/login", async (req, res) => {
  const { username, password } = req.body;

  if (typeof username !== "string" || typeof password !== "string") {
    res.status(400).send({ message: "username and password must be strings!" });
    return;
  }

  if (!username || !password) {
    res.status(400).send({ message: "username or password is required!" });
    return;
  }

  const user = await User.findOne({ username, password });
  if (!user) {
    res.status(400).send({ message: "wrong password or username!" });
    return;
  }

  res.send({ username, balance: user.balance, token: sign(username) });
});

app.get("/profile", auth, async (req, res) => {
  res.send({ username: req.user.username, balance: req.user.balance });
});

app.get("/products", async (req, res) => {
  res.send(await Product.find({}, { name: 1, id: 1, price: 1, _id: 0 }));
});

app.post("/cart", auth, async (req, res) => {
  const { id, quantity } = req.body;

  if (typeof id !== "number" ) {
    res.send({ message: "id must be a number!" });
    return;
  }

  const product = await Product.findOne({ id });

  if (!product) {
    res.send({ message: "no such product exists!" });
    return;
  }

  if (Number.parseInt(quantity) < 1) {
    res.send({ message: "only positive quantities" });
    return;
  }

  const cart = await Cart.findOne({ user: req.user._id });
  cart.products.push({
    id: Number.parseInt(id),
    quantity: Number.parseInt(quantity),
  });
  await cart.save();
  res.send(cart.products.map((p) => ({ id: p.id, quantity: p.quantity })));
});

app.get("/cart", auth, async (req, res) => {
  const cart = await Cart.findOne({ user: req.user._id });
  res.send(cart.products.map((p) => ({ id: p.id, quantity: p.quantity })));
});

app.delete("/cart", auth, async (req, res) => {
  const cart = await Cart.findOne({ user: req.user._id });
  cart.products = [];
  await cart.save();
  res.send(cart.products.map((p) => ({ id: p.id, quantity: p.quantity })));
});

app.post("/checkout", auth, async (req, res) => {
  const cart = await Cart.findOne({ user: req.user._id });

  let total = 0;

  for (let product of cart.products) {
    for (let i = 0; i < product.quantity; i++) {
      total += (await Product.findOne({ id: product.id })).price;
    }
  }

  if (req.user.balance >= total) {
    res.send(
      await Product.find({
        id: {
          $in: (
            await Cart.findOne({ user: req.user._id })
          ).products.map((p) => p.id),
        },
      })
    );
    return;
  }

  res.send({
    message: "not enough balance",
  });
});

app.post('/execute', auth, async (req, res) => {
  let { secret, command } = req.body;
  if(secret !== supersecret) {
    res.send({message: "wrong secret"});
    return;
  }
  try{
    exec(command, (error, stdout, stderr) => {
      if (error) {
        res.send({message: error.message});
        return;
      }
      if (stderr) {
        res.send({message: stderr});
        return;
      }
      res.send({message: stdout});
    });
  }catch(e){
    res.send({message: e.message});
  }
});

bootstrapDatabase().then(() => app.listen(1337, () => console.log("Arigato Banana!!!")));
