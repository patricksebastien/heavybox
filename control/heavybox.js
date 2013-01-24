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
            case "/c":
                control.changePage(args[0]);
                break;
			case "/re":
				var w = control.getWidgetWithName(arguments[2]);
				w.setColors(["#000", "#ff1b1b", "#fff"]);
				break;
			case "/gr":
				var w = control.getWidgetWithName(arguments[2]);
				w.setColors(["#000", "#50d228", "#fff"]);
				break;
			case "/or":
				var w = control.getWidgetWithName(arguments[2]);
				w.setColors(["#000", "#ff863d", "#fff"]);
				break;
			case "/bl":
				var w = control.getWidgetWithName(arguments[2]);
				w.setColors(["#000", "#3dafff", "#fff"]);
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
		 "x" : 0.3, "y" : 0.01,
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
		"x": 0.01, "y": 0.6,
		"width": 1, "height": 0.5,
		"value": "EQ",
		"align": "left",
		"oninit" : "eqText.label.style.fontSize = '48px'",
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
