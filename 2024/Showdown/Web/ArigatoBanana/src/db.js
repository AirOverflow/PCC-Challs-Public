const mongoose = require("mongoose");
const fs = require("fs");
const crypto = require("crypto");
const { Schema } = mongoose;

const supersecret = crypto.randomBytes(32).toString("hex");

const userSchema = new mongoose.Schema(
  {
    username: { type: String, unique: true },
    password: String,
    balance: Number,
  },
  {
    versionKey: false,
  }
);

const productSchema = new mongoose.Schema(
  {
    id: { type: Number, unique: true },
    name: String,
    code: String,
    price: Number,
  },
  {
    versionKey: false,
  }
);

const cartSchema = new mongoose.Schema(
  {
    user: { type: mongoose.Schema.Types.ObjectId, unique: true },
    products: [
      {id: { type: Number }, quantity: Schema.Types.Mixed,},
    ],
  },
  {
    versionKey: false,
  }
);

const User = mongoose.model("User", userSchema);
const Product = mongoose.model("Product", productSchema);
const Cart = mongoose.model("Cart", cartSchema);

const bootstrapDatabase = async () => {
  // Connect to the database on separate running docker
  await mongoose.connect('mongodb://mongodb:27017/pwned', { useNewUrlParser: true, useUnifiedTopology: true });
  await Product.deleteMany();
  await User.deleteMany();
  await Cart.deleteMany();
  await Product.insertMany([
    {
      id: 0,
      name: "hash3liZer",
      code: "0xFF",
      price: 100,
    },
    {
      id: 1,
      name: "hashish_lizer",
      code: "0x00",
      price: 400,
    },
    {
      id: 2,
      name: "arigato",
      code: "0x01",
      price: 600,
    },
    {
      id: 3,
      name: "banana",
      code: "0x02",
      price: 800,
    },
    {
      id: 4,
      name: "ryuk",
      code: "0x03",
      price: 600,
    },
    {
      id: 5,
      name: "shibari",
      code: "0xF1",
      price: 500,
    },
    {
      id: 6,
      name: "Secret",
      code: supersecret,
      price: 2000,
    },
  ]);
};

module.exports = {
  bootstrapDatabase,
  User,
  Product,
  Cart,
};
