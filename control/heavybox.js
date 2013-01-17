loadedInterfaceName = "heavybox";

interfaceOrientation = "landscape";

pages = [

[
{
    "name": "infoText",
    "type": "Label",
    "x": 0.01, "y": 0,
    "width": 0.1, "height": 0.1,
    "value": "RACK 1",
	"align": "left",
},
{
    "name" : "mySlider",
    "type" : "Slider",
    "x" : 0.01, "y" : 0.1,
    "width" : .25, "height" : .55,
    "address" : "/vol",
},
{
     "name" : "myMultiSlider",
     "type" : "MultiSlider",
     "x" : 0.03, "y" : 0,
     "width" : .8, "height" : .5,
     "numberOfSliders" : 6,
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