loadedInterfaceName = "heavybox";
interfaceOrientation = "landscape";

oscManager.delegate = {
    processOSC : function(oscAddress, typetags, args) {
        switch(oscAddress) {
            case "/p":
                control.changePage('previous');
                break;
			case "/n":
                control.changePage('next');
                break;
            case "/c":
                control.changePage(args[0]);
                break;
        }
    }
}

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
];

pages = [
/********** TUNING *************/
[
	{
		"name": "infoText",
		"type": "Label",
		"x": 0.01, "y": 0,
		"width": 1, "height": 0.5,
		"value": "TUNING",
		"align": "left",
		"oninit" : "infoText.label.style.fontSize = '48px'",
		"verticalCenter": false,
	},
	{
		 "name" : "tuner",
		 "type" : "MultiSlider",
		 "width" : .5, "height" : .2,
		 "x" : 0.3, "y" : 0,
		 "numberOfSliders" : 6,
		 "isVertical" : true,
		 "address": "/tuner",
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
/********** RACK 1 *************/
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
],



];
