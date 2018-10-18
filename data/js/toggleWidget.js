var ToggleWidget = function(client, url) {
	this.client = client;
	this.url = url;
	this.node = document.createElement("label");
	this.nodeCheckbox = document.createElement("input");
	this.nodeSpan = document.createElement("span");
	this.init();
};

ToggleWidget.prototype.init = function() {
	var self = this;

	this.node.className = "toggleWidget";

	this.nodeCheckbox.setAttribute("type", "checkbox");
	this.node.appendChild(this.nodeCheckbox);

	this.nodeSpan.className = "slider";
	this.node.appendChild(this.nodeSpan);

	this.on("change", function() { self.apiPut() } );
};

ToggleWidget.prototype.set = function(value) {
	this.nodeCheckbox.checked = value;
};

ToggleWidget.prototype.get = function() {
	return this.nodeCheckbox.checked;
};

ToggleWidget.prototype.on = function(name, fct) {
	this.nodeCheckbox.addEventListener(name, fct);
};

ToggleWidget.prototype.apiGet = function() {
	var self = this;
	this.client.loadJSON(this.url, function(value) {
		self.set(value);
	}, "get");
};

ToggleWidget.prototype.apiPut = function() {
	var args = "&value=" + (this.get() ? "true" : "false") ;
	this.client.loadJSON(this.url, function() {}, "put", args);
};

