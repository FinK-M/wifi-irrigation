//

var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
connection.onopen = function () {
  connection.send('Connect ' + new Date());
  connection.send("?STATE");
  buttons_disabled(false);
};
connection.onerror = function (error) {
  console.log('WebSocket Error ', error);
};
connection.onmessage = function (e) {
  console.log('Server: ', e.data);
  var message = e.data.split(':');
  if(message[0] == "SOL"){
  	var num = Number(message[1]);
  	var state = Number(message[2]);
  	var button = document.getElementById(`sol_button_${num}`);
  	if(state){ button.style.backgroundColor = "#00E640"; }
  	else{ button.style.backgroundColor = "#CF000F"; }
  }
  else if("TIME" == message[0]){
  	var hour = Number(message[2]);
  	var minute = Number(message[3]);
  	var second = Number(message[4]);
  	var d = new Date(0, 0, 0, hour, minute, second);
    if ("UTC" == message[1]){
      var id = "utc_text";  
    }
    else{
      var id = "local_text";
    }
  	document.getElementById(id).innerHTML = d.toLocaleTimeString();
  }
};
connection.onclose = function () {
  console.log('WebSocket connection closed');
};

function toggle (num) {
	console.log(`Toggling solenoid ${num}`)
    connection.send(`SOL:${num}`);
}

function buttons_disabled(state){
	console.log(`Buttons disabled: ${state}`)
	var buttons = document.getElementsByClassName("button");
	var i;
	for (i = 0; i < buttons.length; i++){
		buttons[i].disabled = state;
	}
}