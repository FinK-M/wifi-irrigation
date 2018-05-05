//

var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
connection.onopen = function () {
  connection.send('Connect ' + new Date());
  document.getElementById('toggle_button').disbled = false;
  document.getElementById('toggle_button').style.backgroundColor = '#00878F';
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

function toggle () {
    connection.send("SOL");
}

function disable_on_load(){
	document.getElementById('toggle_button').disbled = true;
}