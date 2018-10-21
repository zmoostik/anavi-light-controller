var DefaultWidget = function(client) {
	this.client = client;
	this.node = document.createElement("nav");
	this.nodeSave = document.createElement("button");
	this.nodeRestore = document.createElement("button");
	this.init();
}

DefaultWidget.prototype.init = function() {
	var self = this;

	this.nodeSave.appendChild(document.createTextNode("Save"));
	this.nodeSave.addEventListener("click", function() {
		self.save();
		return false;
	});
	this.node.appendChild(this.nodeSave);

	this.nodeRestore.appendChild(document.createTextNode("Restore"));
	this.nodeRestore.addEventListener("click", function() {
		self.restore();
		return false;
	});
	this.node.appendChild(this.nodeRestore);
};

DefaultWidget.prototype.save = function() {
	var power = this.client.powerWidget.get();
	var brightness = this.client.brightnessWidget.get();
	var color = this.client.colorWidget.get();
	var d65 = this.client.d65Widget.get();
	var led = this.client.ledWidget.get();
	var args =
		"&color.r=" + color.r + 
		"&color.g=" + color.g +
		"&color.b=" + color.b +
		"&d65.r=" + d63.r + 
		"&d65.g=" + d63.g +
		"&d65.b=" + d63.b +
		"&brightness=" + brightness +
		"&power=" + (power ? "true" : "false") +
		"&led=" + (led ? "true" : "false");

	this.client.loadJSON("/default", function() {}, "put", args);
};

DefaultWidget.prototype.restore = function() {
	var self = this;
	this.client.loadJSON("/default", function(arr) {
		self.client.powerWidget.set(arr.power);
		self.client.powerWidget.apiPut();

		self.client.brightnessWidget.set(arr.brightness);
		self.client.brightnessWidget.apiPut();

		self.client.colorWidget.set(arr.color.r, arr.color.g, arr.color.b);
		self.client.colorWidget.apiPut();

		self.client.ledWidget.set(arr.led);
		self.client.ledWidget.apiPut();
	}, "get");
};
