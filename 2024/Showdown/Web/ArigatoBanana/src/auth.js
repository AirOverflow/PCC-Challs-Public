const jwt = require("jsonwebtoken");
const { User } = require("./db");
const crypto = require("crypto");

const SECRET = crypto.randomBytes(32).toString("hex");

const sign = (username) => {
  return jwt.sign({ username }, SECRET);
};

const auth = async (req, res, next) => {
  try {
    const token = req.headers["authorization"].split(" ")[1];
    const decoded = jwt.verify(token, SECRET);
    const user = await User.findOne({ username: decoded.username });
    if (!user) {
      res.send({ message: "unauthorized" });
      return;
    }
    req.user = user;
    next();
  } catch (e) {
    res.status(403).send({ message: "unauthorized" });
  }
};

module.exports = {
  auth,
  sign,
};
