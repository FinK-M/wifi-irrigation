//

var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
connection.onopen = function () {
  connection.send('Connect ' + new Date());
  buttons_disabled(false);
};
connection.onerror = function (error) {
  console.log('WebSocket Error ', error);
};
connection.onmessage = function (e) {
  console.log('Server: ', e.data);
};
connection.onclose = function () {
  console.log('WebSocket connection closed');
};

function toggle (num) {
	console.log(`Toggling solenoid ${num}`)
    connection.send(`SOL:${num}`);
}

function buttons_disabled(state){
	var buttons = document.getElementsByClassName("button");
	var i;
	for (i = 0; i < buttons.length; i++){
		buttons[i].disbled = state;
	}
}