var express = require("express");
var bodyParser = require("body-parser");
var mysql = require("mysql");
const cors = require("cors");
var app = express();
app.use(cors());
app.use(express.static("public"));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

const PORT = process.env.PORT || 5555;
app.listen(PORT, function () {
  console.log(`Demo baochay at: ${PORT}!`);
}); 

var con = mysql.createConnection({
  host: "localhost",
  port: "3306",
  user: "root",
  password: "1234",
  insecureAuth: true,
  database: "baochay",
});
//step 1
con.connect(function (err) {
  if (err) throw err;
  console.log("Connected!!!");
  var sql = "SELECT * FROM baochay.nhietdo";
  con.query(sql, function (err, results) {
    if (err) throw err;
    console.log(results);
  });
});

app.get("/getAllData", function (req, res) {
  var sql = "SELECT * FROM baochay.nhietdo";
  con.query(sql, function (err, results) {
    if (err) throw err;
    res.send(results);
  });
});

app.post("/addData", function (req, res) {
  const { device, temperature, humid} = req.body;
  console.log('device', device);
  var sql = "insert nhietdo (device, temperature,humid) values ('dht11',"+temperature+", "+humid+")";
  console.log(sql);
  con.query(sql, function (err, results) {
    if (err) throw err;
    res.send("Add ok");
  });
});
//RESTFull API put --> cap nhap
// app.put("/getBookput", function (req, res) {
//   const { id, books, author } = req.body;
//   var sql =
//     "UPDATE books SET books='" +
//     books +
//     "' WHERE author='" +
//     author +
//     "'";
//   con.query(sql, function (err, results) {
//     if (err) throw err;
//     res.send("CapNhap");
//   });
// });
//RESTFull API delete --> xoa
app.delete("/getDelete", function (req, res) {
  const { id } = req.body;
  var sql = "DELETE FROM nhietdo WHERE id = 5";
  con.query(sql, function (err, results) {
    if (err) throw err;
    res.send("xoathanhcong");
  });
});
