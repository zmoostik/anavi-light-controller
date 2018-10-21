var Client = function() {
	this.node = document.createElement("div");
	this.powerWidget = new ToggleWidget(this, "/power");
	this.brightnessWidget = new BrightnessWidget(this);
	this.temperatureWidget = new TemperatureWidget(this);
	this.colorWidget = new ColorWidget(this, "/color");
	this.d65Widget = new ColorWidget(this, "/d65");
	this.ledWidget = new ToggleWidget(this, "/led");
	this.defaultWidget = new DefaultWidget(this);
	this.setupWidget = new SetupWidget();
	this.wifiWidget = new WifiWidget(this);
	this.init();
};

Client.prototype.createRow = function(title, node) {
	var div = document.createElement("div");

	var h2 = document.createElement("h2");
	h2.appendChild(document.createTextNode(title));
	div.appendChild(h2);

	div.appendChild(node);

	return div;
};

Client.prototype.init = function() {
	var self = this;
	this.node.className = "client";

	//power
	this.node.appendChild(this.createRow("Power :", this.powerWidget.node));
	this.powerWidget.apiGet();

	//brightness
	this.node.appendChild(this.createRow("Brightness :", this.brightnessWidget.node));
	this.brightnessWidget.apiGet();

	//color
	this.node.appendChild(this.createRow("Color :", this.colorWidget.node));
	this.colorWidget.apiGet();

	//temperature
	this.node.appendChild(this.createRow("White Balance :", this.temperatureWidget.node));
	this.temperatureWidget.apiGet();

	//d65
	this.node.appendChild(this.createRow("D65 Calibrate :", this.d65Widget.node));
	this.d65Widget.apiGet();

	//led
	this.node.appendChild(this.createRow("Led :", this.ledWidget.node));
	this.ledWidget.apiGet();

	//default manager
	this.node.appendChild(this.createRow("Defaults :", this.defaultWidget.node));

	//wifi manager
	this.node.appendChild(this.setupWidget.node);
	this.setupWidget.append(this.wifiWidget.node);
	this.wifiWidget.apiGet();
};

Client.prototype.loadUrl = function(url, fct, method, params) {
	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		if (this.readyState == 4) {
			if (this.status == 200 || this.status == 0)
				fct(this.responseText);
			else 
				console.log("HTTP Error:", this.status);
		}
	};
	xhttp.open(method ? method : "GET", url, true);
	xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	xhttp.send(params);
};

Client.prototype.loadJSON = function(url, fct, method, params) {
	this.loadUrl(url, function(text) {
		var arr = JSON.parse(text);
		if (arr) fct(arr);
	}, method, params);
};

Client.prototype.clear = function() {
	while (this.node.firstChild) this.node.removeChild(this.node.firstChild);
};

Client.prototype.msg = function(text) {
	this.clear();
	var p = document.createElement("p");
	p.className = "message";
	p.appendChild(document.createTextNode(text));
	this.node.appendChild(p);	
};
