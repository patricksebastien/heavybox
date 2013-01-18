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
		 "name" : "tuner",
		 "type" : "MultiSlider",
		 "width" : .5, "height" : .2,
		 "x" : 0.3, "y" : 0,
		 "numberOfSliders" : 6,
		 "isVertical" : true,
		 "address": "/t",
		 "min" : 0, "max" : 1000,
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
],



];
