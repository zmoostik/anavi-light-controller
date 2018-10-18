var SetupWidget = function() {
	this.node = document.createElement("div");
	this.nodeIcon = document.createElement("img");
	this.nodeContent = document.createElement("div");
	this.nodePopup = document.createElement("div");
	this.init();
};

SetupWidget.prototype.init = function() {
	var self = this;
	this.node.className = "setupWidget";
	this.node.appendChild(this.nodeIcon);

	this.nodeIcon.className = "icon";
	this.nodeIcon.setAttribute("src", "img/settings_24px.png");
	this.nodeIcon.addEventListener("click", function() { self.onclick(); });

	this.setVisibility(false);
	this.nodePopup.className = "popup";
	this.node.appendChild(this.nodePopup);
};

SetupWidget.prototype.append = function(node) {
	this.nodePopup.appendChild(node);
};

SetupWidget.prototype.hide = function() {
	this.setVisibility(false);
};

SetupWidget.prototype.show = function() {
	this.setVisibility(true);
};

SetupWidget.prototype.setVisibility = function(value) {
	this.nodePopup.style.display = value ? "" : "none";
};

SetupWidget.prototype.getVisibility = function() {
	return this.nodePopup.style.display != "none";
};
SetupWidget.prototype.onclick = function() {
	this.setVisibility(!this.getVisibility());
};

