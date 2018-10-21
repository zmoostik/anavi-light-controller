var TemperatureWidget = function(client) {
	this.client = client;
	this.node = document.createElement("div");
	this.nodeInput = document.createElement("input");
	this.nodeValue = document.createElement("span");
	this.init();
};

TemperatureWidget.prototype.init = function() {
	var self = this;

	this.node.className = "temperatureWidget";

	this.node.appendChild(this.nodeValue);
	this.nodeValue.appendChild(document.createTextNode(""));

	this.nodeInput.setAttribute("type", "range");
	this.nodeInput.setAttribute("min", "1000");
	this.nodeInput.setAttribute("max", "8000");
	this.nodeInput.setAttribute("step", "100");
	this.node.appendChild(this.nodeInput);

	this.update();
	this.on("input", function() { self.update(); self.apiPut() });
};

TemperatureWidget.prototype.update = function() {
	this.nodeValue.firstChild.nodeValue = this.nodeInput.value + " °K";
};

TemperatureWidget.prototype.set = function(value) {
	this.nodeInput.value = value;
	this.update();
};

TemperatureWidget.prototype.get = function() {
	return this.nodeInput.value;
};

TemperatureWidget.prototype.on = function(name, fct) {
	this.nodeInput.addEventListener(name, fct);
};

TemperatureWidget.prototype.apiGet = function() {
	var self = this;
	this.client.loadJSON("/temperature", function(value) {
		self.set(value);
	}, "get");
};

TemperatureWidget.prototype.apiPut = function() {
	var args = "&value=" + this.get();
	this.client.loadJSON("/temperature", function() {}, "put", args);
};

