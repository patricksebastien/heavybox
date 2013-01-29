loadedInterfaceName = "heavybox";
interfaceOrientation = "landscape";

/*
adb push D:\dropbox\Dropbox\github\heavybox\control\heavybox.js /mnt/sdcard/control
*/

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
			case "/cr":
				var w = control.getWidgetWithName(arguments[2]);
				w.setColors(["#000", "#ff1b1b", "#fff"]);
				break;
			case "/cg":
				var w = control.getWidgetWithName(arguments[2]);
				w.setColors(["#000", "#50d228", "#fff"]);
				break;
			case "/co":
				var w = control.getWidgetWithName(arguments[2]);
				w.setColors(["#000", "#ff863d", "#fff"]);
				break;
			case "/cb":
				var w = control.getWidgetWithName(arguments[2]);
				w.setColors(["#000", "#3dafff", "#fff"]);
				break;
			default:
                oscManager.processOSC(oscAddress, typetags, args);
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
		 "name" : "tuning",
		 "type" : "Slider",
		 "width" : .7, "height" : .15,
		 "x" : 0.3, "y" : 0.02,
		 "isVertical" : false,
		 "address": "/tuning",
		 "min" : 0, "max" : 1,
	},
	{
		"name": "noteText",
		"type": "Label",
		"x": 0.55, "y": 0,
		"width": 0.45, "height": 0.5,
		"value": "ADC3",
		"align": "left",
		"oninit" : "noteText.label.style.fontSize = '48px'",
		"verticalCenter": false,
	},
	// EQ-----------------------------------------------------
	{
		"name": "eqText",
		"type": "Label",
		"x": 0.01, "y": 0.22,
		"width": 1, "height": 0.5,
		"value": "EQ",
		"align": "left",
		"oninit" : "eqText.label.style.fontSize = '24px'",
		"verticalCenter": false,
	},
	{
		"name": "eqLow",
		"type": "Knob",
		"x": 0.01,
		"y": 0.3,
		"radius": .17,
		"centerZero": true,
		"startingValue": 0,
		"color": "#3dafff",
		"min": -40,
		"max": 40,
		"protocol": "OSC",
		"address": "/eqLow",
	},
	{
		"name": "eqMid",
		"type": "Knob",
		"x": 0.12,
		"y": 0.3,
		"radius": .17,
		"centerZero": true,
		"startingValue": 0,
		"color": "#3dafff",
		"min": -40,
		"max": 40,
		"protocol": "OSC",
		"address": "/eqMid",
	},
	{
		"name": "eqHigh",
		"type": "Knob",
		"x": 0.23,
		"y": 0.3,
		"radius": .17,
		"centerZero": true,
		"startingValue": 0,
		"color": "#3dafff",
		"min": -40,
		"max": 40,
		"protocol": "OSC",
		"address": "/eqHigh",
	},
	// COMPRESSOR-----------------------------------------------------
	{
		"name": "compressorText",
		"type": "Label",
		"x": 0.4, "y": 0.22,
		"width": 0.6, "height": 0.5,
		"value": "COMPRESSOR",
		"align": "left",
		"oninit" : "compressorText.label.style.fontSize = '24px'",
		"verticalCenter": false,
	},
	{
		"name": "compressorGain",
		"type": "Knob",
		"x": 0.4,
		"y": 0.3,
		"radius": .17,
		"centerZero": false,
		"startingValue": 5,
		"color": "#3dafff",
		"min": 0,
		"max": 10,
		"protocol": "OSC",
		"address": "/compressorGain",
	},
	{
		"name": "compressorGainText",
		"type": "Label",
		"x": 0.4, "y": 0.47,
		"value": "Gain",
		"align": "left",
		"oninit" : "compressorGainText.label.style.fontSize = '11px'",
		"verticalCenter": false,
	},
	{
		"name": "compressorThresh",
		"type": "Knob",
		"x": 0.51,
		"y": 0.3,
		"radius": .17,
		"centerZero": false,
		"startingValue": 5,
		"color": "#3dafff",
		"min": 0,
		"max": 10,
		"protocol": "OSC",
		"address": "/compressorThresh",
	},
	{
		"name": "compressorThreshText",
		"type": "Label",
		"x": 0.51, "y": 0.47,
		"value": "Threshold",
		"align": "left",
		"oninit" : "compressorThreshText.label.style.fontSize = '11px'",
		"verticalCenter": false,
	},
	{
		"name": "compressorAttack",
		"type": "Knob",
		"x": 0.62,
		"y": 0.3,
		"radius": .17,
		"centerZero": false,
		"startingValue": 5,
		"color": "#3dafff",
		"min": 0,
		"max": 10,
		"protocol": "OSC",
		"address": "/compressorAttack",
	},
	{
		"name": "compressorAttackText",
		"type": "Label",
		"x": 0.62, "y": 0.47,
		"value": "Attack",
		"align": "left",
		"oninit" : "compressorAttackText.label.style.fontSize = '11px'",
		"verticalCenter": false,
	},
	{
		"name": "compressorRelease",
		"type": "Knob",
		"x": 0.73,
		"y": 0.3,
		"radius": .17,
		"centerZero": false,
		"startingValue": 5,
		"color": "#3dafff",
		"min": 0,
		"max": 10,
		"protocol": "OSC",
		"address": "/compressorRelease",
	},
	{
		"name": "compressorReleaseText",
		"type": "Label",
		"x": 0.73, "y": 0.47,
		"value": "Release",
		"align": "left",
		"oninit" : "compressorReleaseText.label.style.fontSize = '11px'",
		"verticalCenter": false,
	},
	// PAN-----------------------------------------------------
	{
		"name": "panText",
		"type": "Label",
		"x": 0.875, "y": 0.22,
		"width": 0.6, "height": 0.5,
		"value": "PAN",
		"align": "left",
		"oninit" : "panText.label.style.fontSize = '24px'",
		"verticalCenter": false,
	},
	{
		"name": "pan",
		"type": "Knob",
		"x": 0.875,
		"y": 0.3,
		"radius": .17,
		"centerZero": true,
		"startingValue": 0,
		"color": "#3dafff",
		"min": -1,
		"max": 1,
		"protocol": "OSC",
		"address": "/pan",
	},
	// VOLUME-----------------------------------------------------
	{
		"name": "volumeText",
		"type": "Label",
		"x": 0.01, "y": 0.5,
		"width": 0.6, "height": 0.5,
		"value": "ADC 1->4 VOLUME",
		"align": "left",
		"oninit" : "volumeText.label.style.fontSize = '24px'",
		"verticalCenter": false,
	},
	{
		 "name" : "adcVolume",
		 "type" : "MultiSlider",
		 "x" : 0.01, "y" : 0.58,
		 "width" : 0.4, "height" : .3,
		 "numberOfSliders" : 4,
		 "startingValue": 1,
		 "strokeColor": "#ffffff",
		 "color": "#3dafff",
		 "isVertical" : false,
	},
	{
		"name": "clipText",
		"type": "Label",
		"x": 0.41, "y": 0.5,
		"width": 0.6, "height": 0.5,
		"value": "Clip",
		"align": "left",
		"oninit" : "clipText.label.style.fontSize = '11px'",
		"verticalCenter": false,
	},
	{
		 "name" : "adcClip",
		 "type" : "MultiButton",
		 "mode" : "momentary",
		 "x" : 0.41, "y" : 0.58,
		 "width" : .05, "height" : .3,
		 "rows" : 4, "columns" : 1,
	}
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
	{
		"name": "refresh",
		"type": "Button",
		"bounds": [.6, .9, .2, .1],
		"startingValue": 0,
		"isLocal": true,
		"mode": "contact",
		"ontouchstart": "interfaceManager.refreshInterface()",
		"stroke": "#aaa",
		"label": "refrsh",
	},
],



];
