var BrightnessWidget = function(client) {
	this.client = client;
	this.node = document.createElement("div");
	this.nodeValue = document.createElement("span");
	this.nodeInput = document.createElement("input");
	this.init();
};

BrightnessWidget.prototype.init = function() {
	var self = this;

	this.node.className = "brightnessWidget";

	this.node.appendChild(this.nodeValue);
	this.nodeValue.appendChild(document.createTextNode(""));

	this.nodeInput.setAttribute("type", "range");
	this.nodeInput.setAttribute("min", "0");
	this.nodeInput.setAttribute("max", "100");
	this.node.appendChild(this.nodeInput);

	this.on("input", function() { self.update(); self.apiPut() });
	this.update();
};

BrightnessWidget.prototype.update = function() {
	this.nodeValue.firstChild.nodeValue = this.nodeInput.value + "%";
};

BrightnessWidget.prototype.set = function(value) {
	this.nodeInput.value = value;
	this.update();
};

BrightnessWidget.prototype.get = function() {
	return this.nodeInput.value;
};

BrightnessWidget.prototype.on = function(name, fct) {
	this.node.addEventListener(name, fct);
};

BrightnessWidget.prototype.apiGet = function() {
	var self = this;
	this.client.loadJSON("/brightness", function(value) {
		self.set(value);
	}, "get");
};

BrightnessWidget.prototype.apiPut = function() {
	var args = "&value=" + this.get();
	this.client.loadJSON("/brightness", function() {}, "put", args);
};

