loadedInterfaceName = "heavybox";

interfaceOrientation = "landscape";

pages = [

[
{
    "name": "infoText",
    "type": "Label",
    "x": 0.05, "y": 0,
    "width": 0.1, "height": 0.1,
    "value": "RACK 1",
	"align": "left",
},
{
    "name" : "mySlider",
    "type" : "Slider",
    "x" : 0.05, "y" : 0.2,
    "width" : .25, "height" : .75,
    "address" : "/vol",
}
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