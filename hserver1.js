const express = require('express');

const path = require('path');
const app = express();

//client
//const net = require("net");
//var socket = new net.Socket();

var SerialPort = require('serialport');
var serialPort = new SerialPort('COM4', false);

//var SerialPort2 = require('serialport');
//var serialPort2 = new SerialPort2('COM4', false);
//var SerialPort3 = require('serialport');`
//var serialPort3 = new SerialPort3('COM19', false);


var request = require('request');
var headers = {
    'User-Agent':       'Super Agent/0.0.1',
    'Content-Type':     'application/x-www-form-urlencoded'
}

// Configure the request


//client 종료

var bodyParser = require('body-parser');
var id = "wait";

var ADOXX_STATUS = "NoAccess"; // NoAccess, idle, wait, running, error, exit
var MACHINE_STATUS = "NoAccess"; // NoAccess, idle, wait, running, error, exit

var m = "";
var targetm = "";

var ADOXX_AUTOMOBILE_STATUS = "NoAccess"; // NoAccess, idle, wait, running, error, exit
var MACHINE_AUTOMOBILE_STATUS = "NoAccess"; // NoAccess, idle, wait, running, error, exit

var qs = "";
var exe_time = "";


var timeout = 1000;


serialPort.open(function () {
	console.log('connected...');
});

//serialPort2.open(function () {
//	
//	console.log('connected2...');
//});

//serialPort3.open(function () {
//	console.log('connected3...');
//});

app.listen(10000, () => {
  console.log('Express App on port 10000!');
 // console.log(path);
});

app.use(bodyParser.urlencoded({extended:false}));
app.use(bodyParser.text());

app.use(

function(req, res, next) {
	setTimeout((function () {
	console.log('Request URL:', req.originalUrl);
	console.log('Request Body:', req.body);
	console.log('req.query.id: ', req.query.id);
  if (req.query.id == "adoxx")
  {
		console.log('online adoxx');
		//console.log('req.body type : ', typeof(req.body));
		//console.log('"asd" type : ', typeof("asd"));
		if(typeof(req.body) != typeof("asd")) {
			console.log('Raspberry PI AI speaker');
			req.body = "@" + JSON.stringify(req.body);
			console.log('Request Body:', req.body);
		}
				
		var name = req.query.name
		if (ADOXX_STATUS == "NoAccess")
		{
			ADOXX_STATUS = "idle";
			res.send(ADOXX_STATUS);
		}
		else if (ADOXX_STATUS == "idle")
		{
			// if (typeof(req.query.M1) != "undefined") { m = req.query.m1; targetm = "m1"; ADOXX_STATUS = "wait"; }
			// else if (typeof(req.query.M2) != "undefined") { m = req.query.m2; targetm = "m2"; ADOXX_STATUS = "wait"; }
			// else if (typeof(req.query.M3) != "undefined") { m = req.query.m3; targetm = "m3"; ADOXX_STATUS = "wait"; }
			// else if (typeof(req.query.M4) != "undefined") { m = req.query.m4; targetm = "m4"; ADOXX_STATUS = "wait"; }
			// else if (typeof(req.query.M5) != "undefined") { m = req.query.m5; targetm = "m5"; ADOXX_STATUS = "wait"; }
			// else if (typeof(req.query.M6) != "undefined") { m = req.query.m6; targetm = "m6"; ADOXX_STATUS = "wait"; }
			// else {}
	//		console.log(JSON
			res.send(ADOXX_STATUS);
		}
		else if (ADOXX_STATUS == "wait")
		{
			res.send(ADOXX_STATUS);
		}
		//Client
		if (name == "Automobile01")
		{
			console.log('automobile01');
			var automobile01 = {
				url: 'http://192.168.1.3:10001',
				method: 'GET',
				headers: headers,
				body: req.body
				
			}
				//var SerialPort = require('serialport');
				//var serialPort = new SerialPort('COM4', false);
				
				//serialPort.open(function () {
				//console.log('connected...');
				//serialPort.write(req.body, function(err, results) {});
				//});
				
				//serialPort.close(function(err) {
				//	console.log('port closed', err);
				//});
			serialPort.write(req.body, function(err, results) {});
			console.log('t');

			setTimeout(function() {		//10초후 물건들기
				req.body="@{\"1_t\":\"arm\",\"1_m\":\"M4\",\"1_d\":\"90\",\"2_t\":\"arm\",\"2_m\":\"M3\",\"2_d\":\"0\",\"state\":\"2\"}";
				console.log('Request Body:', req.body);
				serialPort.write(req.body, function(err, results) {});
				setTimeout(function() {		//5초후 물건들고 가기
					req.body="@{\"1_t\":\"car\",\"1_qs\":\"F\",\"2_t\":\"car\",\"2_qs\":\"ST\",\"state\":\"2\"}";
					console.log('Request Body:', req.body);
					serialPort.write(req.body, function(err, results) {});
					setTimeout(function() {		//10초후 물건 내려놓기
						req.body="@{\"1_t\":\"arm\",\"1_m\":\"M3\",\"1_d\":\"180\",\"2_t\":\"arm\",\"2_m\":\"M4\",\"2_d\":\"180\",\"state\":\"2\"}";
						console.log('Request Body:', req.body);
						serialPort.write(req.body, function(err, results) {});
					}, 10000);
				}, 5000);
			}, 10000);
			
			

			
			
		}	
		else if (name == "Automobile02")
		{
			console.log('automobile02');
			var automobile02 = {
				url: 'http://192.168.1.4:10002',
				method: 'GET',
				headers: headers,
				body: req.body
			}	
			serialPort2.write(req.body, function(err, results) {});
		}
		else if (name == "Automobile03")
		{
			var automobile03 = {
				url: 'http://192.168.1.5:10003',
				method: 'GET',
				headers: headers,
				body: req.body
				
			}
		//	serialPort3.write(req.body, function(err, results) {});
		}
		
  }
  
  else if (req.query.id == "robotarm")
  {
		if (MACHINE_STATUS == "NoAccess")
		{

			res.send("@{"
							+	"\"op\":\"idle\""
							+	"}@");
			MACHINE_STATUS = "idle";
			
		}
		else if (MACHINE_STATUS == "idle")
		{
			if (ADOXX_STATUS == "wait")
			{

				res.send("@{"
						+	"\"op\":\"running\","
//						+	"\"index\":\"2\","
//						+	"\"list\":[\"targetm\",\"degree\"]," //리스트 형식이 잘못된듯.. 수정 필요함
						+   "\"targetm\":\""+targetm+"\","
						+   "\"degree\":\""+m+"\""
						+	"}@");

				MACHINE_STATUS = "running";
			}
			else
			{
				res.send("@{"
							+	"\"op\":\"idle\""
							+	"}@");
			}
		}
		else if (MACHINE_STATUS == "running")
		{
			// if (req.query.result == "complete")
			// {
			//		setTimeout(function() {
				
			//		}, 1000);

					res.send("@{"
						+	"\"op\":\"idle\""
						+	"}@");

					MACHINE_STATUS = "idle";
					ADOXX_STATUS = "idle";
					
			// }
			// else
			// {
				// MACHINE_STATUS = "error";
					// ADOXX_STATUS = "error";
					// res.send("@{"
							// +	"\"op\":\"error\""
							// +	"}");
			// }
		}
  }  
  else if (req.query.id == "ado_automobile") 
  {
		if (ADOXX_AUTOMOBILE_STATUS == "NoAccess")
		{
			ADOXX_AUTOMOBILE_STATUS = "idle";
			res.send(ADOXX_AUTOMOBILE_STATUS);
		}
		else if (ADOXX_AUTOMOBILE_STATUS == "idle")
		{
			if (typeof(req.query.QuickStart) != "undefined") { qs = req.query.QuickStart; ADOXX_AUTOMOBILE_STATUS = "wait"; }
			if (typeof(req.query.time) != "undefined") { exe_time = req.query.time; ADOXX_AUTOMOBILE_STATUS = "wait"; }
			
			res.send(ADOXX_AUTOMOBILE_STATUS);
		}
		else if (ADOXX_AUTOMOBILE_STATUS == "wait")
		{
			res.send(ADOXX_AUTOMOBILE_STATUS);
		}
  }
  else if (req.query.id == "mc_automobile")
  {
		if (MACHINE_AUTOMOBILE_STATUS == "NoAccess")
		{

			res.send("@{"
							+	"\"op\":\"idle\""
							+	"}@");
			MACHINE_AUTOMOBILE_STATUS = "idle";
			
		}
		else if (MACHINE_AUTOMOBILE_STATUS == "idle")
		{
			if (ADOXX_AUTOMOBILE_STATUS == "wait")
			{

				res.send("@{"
						+	"\"op\":\"running\","
						+   "\"QuickStart\":\""+qs+"\","
						+   "\"time\":\""+exe_time+"\""
						+	"}@");

				MACHINE_AUTOMOBILE_STATUS = "running";
			}
			else
			{
				res.send("@{"
							+	"\"op\":\"idle\""
							+	"}@");
			}
		}
		else if (MACHINE_AUTOMOBILE_STATUS == "running")
		{
					res.send("@{"
						+	"\"op\":\"idle\""
						+	"}@");

					MACHINE_AUTOMOBILE_STATUS = "idle";
					ADOXX_AUTOMOBILE_STATUS = "idle";
		}
  }
  //console.log('Request URL END:', req.originalUrl);
  console.log('end req');
  next();
 // next();
//  setTimeout(() => next(), 3000);
}),timeout);
}
/*,function(req, res, next) {
  console.log('Request URL:', req.originalUrl);
  next();
}, function (req, res, next) {
  console.log('Request Type:', req.method);
  next();
}, function (req, res, next) {
  console.log('Request a:', req.query.a);
  next();
}, function (req, res, next) {
  console.log('Request var id:', id);
  res.send(id)
  next();
}, function (req, res, next) {
  id = req.query.a
  console.log('Request var id:', id);
  next();
}, function (req, res, next) {
  console.log('Request a:', req.query.a);
  next();
}, function (req, res, next) {
  console.log('Request a:', req.query);
  next();
}
, function (req, res, next) {
  console.log('Request a:', req.params.id);
  next();
}*/

);

