var ColorWidget = function(client) {
	this.client = client;
	this.node = document.createElement("div");
	this.nodeRGB = document.createElement("div");
	this.nodeTable = document.createElement("table");
	this.nodeR = document.createElement("input");
	this.nodeG = document.createElement("input");
	this.nodeB = document.createElement("input");
	this.init();
};

ColorWidget.prototype.createRow = function(title, range) {
	var tr = document.createElement("tr");

	var th = document.createElement("th");
	th.appendChild(document.createTextNode(title));
	tr.appendChild(th);

	range.setAttribute("type", "range");
	range.setAttribute("min", "0");
	range.setAttribute("max", "255");
	tr.appendChild(range);
	return tr;
};

ColorWidget.prototype.init = function() {
	var self = this;

	this.node.className = "colorWidget";

	this.node.appendChild(this.nodeRGB);
	this.nodeRGB.appendChild(document.createTextNode(""));
	this.nodeRGB.className = "rgb";

	this.nodeTable.appendChild(this.createRow("R", this.nodeR));
	this.nodeTable.appendChild(this.createRow("G", this.nodeG));
	this.nodeTable.appendChild(this.createRow("B", this.nodeB));
	this.node.appendChild(this.nodeTable);

	this.on("input", function() { self.update(); self.apiPut(); });
	this.update();
};

ColorWidget.prototype.get = function() {
	return {
		"r": this.nodeR.value,
		"g": this.nodeG.value,
		"b": this.nodeB.value
	};
};

ColorWidget.prototype.update = function() {
	this.nodeRGB.firstChild.nodeValue = "(" + this.nodeR.value + ", " + this.nodeG.value + ", " + this.nodeB.value + ")";
};

ColorWidget.prototype.set = function(r, g, b) {
	this.nodeR.value = r;
	this.nodeG.value = g;
	this.nodeB.value = b;
	this.update();
};

ColorWidget.prototype.on = function(name, fct) {
	this.nodeR.addEventListener(name, fct);
	this.nodeG.addEventListener(name, fct);
	this.nodeB.addEventListener(name, fct);
};

ColorWidget.prototype.apiGet = function() {
	var self = this;
	this.client.loadJSON("/color", function(value) {
		self.set(value.r, value.g, value.b);
	}, "get");
};

ColorWidget.prototype.apiPut = function() {
	var value = this.get();
	var args = 
		"&r=" + value.r +
		"&g=" + value.g +
		"&b=" + value.b;

	this.client.loadJSON("/color", function() {}, "put", args);
};

