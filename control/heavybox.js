loadedInterfaceName = "heavybox";
interfaceOrientation = "landscape";

pages = [

[
{
    "name": "infoText",
    "type": "Label",
    "x": 0.01, "y": 0,
    "width": 1, "height": 0.5,
    "value": "RACK 1",
	"align": "left",
	"oninit" : "infoText.label.style.fontSize = '48px'"
	"verticalCenter": false,
},
{
     "name" : "tuner",
     "type" : "MultiSlider",
     "width" : .4, "height" : .2,
     "x" : 0.08, "y" : 0,
     "numberOfSliders" : 6,
     "isVertical" : true,
},
{
     "name" : "volume",
     "type" : "MultiSlider",
     "width" : .4, "height" : .2,
     "x" : 0.01, "y" : 0.3,
     "numberOfSliders" : 3,
     "isVertical" : true,
},
{
    "name": "refresh",
    "type": "Button",
    "bounds": [.6, .85, .2, .1],
    "startingValue": 0,
    "isLocal": true,
    "mode": "contact",
    "ontouchstart": "interfaceManager.refreshInterface()",
    "stroke": "#aaa",
    "label": "refresh",
},
{
    "name": "tabButton",
    "type": "Button",
    "bounds": [.8, .85, .2, .1],
    "mode": "toggle",
    "stroke": "#aaa",
    "isLocal": true,
    "ontouchstart": "if(this.value == this.max) { control.showToolbar(); } else { control.hideToolbar(); }",
    "label": "menu",
},
]




];