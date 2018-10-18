var WifiWidget = function(client) {
	this.client = client;
	this.node = document.createElement("form");
	this.nodeSSID = document.createElement("input");
	this.nodePasswd = document.createElement("input");
	this.nodeEther = document.createElement("div");
	this.nodeStatus = document.createElement("div");
	this.nodeSave = document.createElement("button");
	this.nodeCancel = document.createElement("button");
	this.init();
};

WifiWidget.prototype.createRow = function(title, node, id) {
	var tr = document.createElement("tr");

	var th = document.createElement("th");
	var label = document.createElement("label");
	label.appendChild(document.createTextNode(title));
	label.setAttribute("for", id);
	th.appendChild(label);
	tr.appendChild(th);

	var td = document.createElement("td");
	td.appendChild(node);
	tr.appendChild(td);
	node.id = id;

	return tr;
};

WifiWidget.prototype.init = function() {
	var self = this;

	this.node.className = "wifiWidget";


	var h2 = document.createElement("h2");
	h2.appendChild(document.createTextNode("Wifi :"));
	this.node.appendChild(h2);

	var table = document.createElement("table");
	table.appendChild(this.createRow("MAC Address :", this.nodeEther, "mac"));
	table.appendChild(this.createRow("Status :", this.nodeStatus, "status"));
	table.appendChild(this.createRow("SSID :", this.nodeSSID, "ssid"));
	table.appendChild(this.createRow("Password :", this.nodePasswd, "passwd"));
	this.node.appendChild(table);

	this.nodeEther.appendChild(document.createTextNode(""));
	this.nodeStatus.appendChild(document.createTextNode(""));
	this.nodePasswd.setAttribute("type", "password");

	var tr = document.createElement("tr");
	table.appendChild(tr);

	var td = document.createElement("td");
	td.className = "actions";
	td.setAttribute("colspan", "2");
	tr.appendChild(td); 

	this.nodeCancel.appendChild(document.createTextNode("Cancel"));
	this.nodeCancel.setAttribute("type", "button");
	this.nodeCancel.addEventListener("click", function() { self.cancel(); } );
	td.appendChild(this.nodeCancel);

	this.nodeSave.appendChild(document.createTextNode("Save"));
	td.appendChild(this.nodeSave);

	this.node.onsubmit = function() { self.save(); return false; };
};

WifiWidget.prototype.get = function() {
	return {
		"ssid": this.nodeSSID.value,
		"password": this.nodePasswd.value,
		"status": this.nodeStatus.firstChild.nodeValue,
		"ether": this.nodeEther.firstChild.nodeValue
	};
};

WifiWidget.prototype.set = function(ssid, password, ether, status) {
	this.nodeSSID.value = ssid;
	this.nodePasswd.value = password;
	this.nodeStatus.firstChild.nodeValue = status;
	this.nodeEther.firstChild.nodeValue = ether;
};

WifiWidget.prototype.apiGet = function() {
	var self = this;
	this.client.loadJSON("/wifi", function(value) {
		self.set(value.ssid, "", value.ether, value.status);
	}, "get");
};

WifiWidget.prototype.apiPut = function(fct) {
	var value = this.get();
	var args =
		"&ssid=" + encodeURIComponent(value.ssid) +
		"&password=" + encodeURIComponent(value.password);

	console.log(args);
	this.client.loadJSON("/wifi", fct, "put", args);
};

WifiWidget.prototype.save = function() {
	var self = this;
	this.apiPut(function(value) {
		self.client.msg("The device will be disconnected.");
	});
};

WifiWidget.prototype.cancel = function() {
	this.client.setupWidget.hide();
};
