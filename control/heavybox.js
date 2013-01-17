loadedInterfaceName = "heavybox";

interfaceOrientation = "landscape";

pages = [

[
{
    "name": "infoText",
    "type": "Label",
    "x": 0, "y": 0,
    "width": 0.1, "height": 0.1,
    "value": "RACK 1",
	"align": "left",
	"backgroundColor": "rgba(1,0,0,1)",
},
{
     "name" : "myButton",
     "type" : "MultiButton",
     "x" : 0.1, "y" : 0,
     "width" : 0.9, "height" : .8,
     "rows" : 1, "columns" : 6,
}
{
    "name": "refresh",
    "type": "Button",
    "bounds": [.6, .8, .2, .1],
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
    "bounds": [.8, .9, .2, .1],
    "mode": "toggle",
    "stroke": "#aaa",
    "isLocal": true,
    "ontouchstart": "if(this.value == this.max) { control.showToolbar(); } else { control.hideToolbar(); }",
    "label": "menu",
},
]




];