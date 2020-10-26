/* var HOST = location.origin.replace(/^http/, 'ws') */
/* var ws = new WebSocket(HOST); */
var ws = new WebSocket("ws://192.168.4.1:1337/");
var ssid_var;

ws.onopen = function() {
   ws.send('{"name":"hola"}');
}

ws.onclose = function() {
  alert(' websocket closed.. ');
}

function send(){
  if(document.getElementById('ssid').value &&
     document.getElementById('pwd').value &&
     document.getElementById('host').value &&
     document.getElementById('token').value) {

    let param = {
    ssid:"", 
    pwd: "", 
    host: "", 
    token: ""
  };
  param["ssid"] = document.getElementById('ssid').value;
  param["pwd"] = document.getElementById('pwd').value;
  param["host"] = document.getElementById('host').value;
  param["token"] = document.getElementById('token').value;
  console.log(param);
  ws.send(JSON.stringify(param));
  } else {
    alert('Please fill up all box');
  }
}

ws.onmessage = function (event) {
  ssid_var = event.data;
};

function enterFalse(event){
  if (event.keyCode == 13){
    send();
    event.stopPropagation();
    return false;
  }
}

/* Scan SSID */
function scan() {
    var s = document.getElementById("ssid");
    var c = document.createElement("option");
    var t = JSON.parse(ssid_var);
    console.log(t);
    /* Remove list option */
    for(i = s.options.length-1; i >= 0; i--) {
      // s.options.remove(i);
      s.options[i] = null;
      console.log(s.length);
      console.log("loop: " + i );
    }
    /* Add list option */
    for(i in t) {
      s.options[s.options.length] = new Option(t[i], t[i]);
    }
}