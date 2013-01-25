loadedInterfaceName = "heavybox";
interfaceOrientation = "landscape";

/*red: ff1b1b
green: 50d228
orange: ff863d
blue: 3dafff
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
			case "/ta":
				var w = control.getWidgetWithName(args[0]);
				w.setValue("fff");
				break;
			default:
                oscManager.processOSC(oscAddress, typetags, args);
                break;
        }
    }
}


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
		"value": "NOTE",
		"align": "left",
		"oninit" : "noteText.label.style.fontSize = '48px'",
		"verticalCenter": false,
	},
	{
		"name": "eqText",
		"type": "Label",
		"x": 0.01, "y": 0.5,
		"width": 1, "height": 0.5,
		"value": "EQ",
		"align": "left",
		"oninit" : "eqText.label.style.fontSize = '24px'",
		"verticalCenter": false,
	},
	{
		 "name" : "volume",
		 "type" : "MultiSlider",
		 "width" : .4, "height" : .2,
		 "x" : 0.01, "y" : 0.3,
		 "numberOfSliders" : 3,
		 "isVertical" : true,
		 "address": "/v",
	},
],
/********** RACK 1 *************/
[
	{
		"name": "infoText",
		"type": "Label",
		"x": 0.01, "y": 0,
		"width": 1, "height": 0.5,
		"value": "RACK 1d",
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
