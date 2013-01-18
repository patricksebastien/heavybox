loadedInterfaceName = "heavybox";
interfaceOrientation = "landscape";



constants = [
{
    "name": "nextBtn",
    "type": "Button",
    "bounds": [0,.9,.2,.1], 
    "label": "<-",
    "mode": "contact",    
    "ontouchstart": "control.changePage('previous');",
    "stroke": "#aaa",    
},
{
    "name": "page2Btn",
    "type": "Button",
    "bounds": [.2,.9,.2,.1], 
    "label": "->",
    "mode": "contact",    
    "ontouchstart": "control.changePage('next');",
    "stroke": "#aaa",    
},
{
    "name": "refresh",
    "type": "Button",
    "bounds": [.6, .9, .2, .1],
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
];

pages = [

[
{
    "name": "infoText",
    "type": "Label",
    "x": 0.01, "y": 0,
    "width": 1, "height": 0.5,
    "value": "RACK 1",
	"align": "left",
	"oninit" : "infoText.label.style.fontSize = '48px'",
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
],
/********** PAGE 2 *************/
[
    {
        "name":"page2Label",
        "type":"Label",
        "value":"PAGE 2",
        "size": "40",   
        "bounds": [0,0,1,.5]             
    }

],



];
