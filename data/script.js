var cta = document.querySelector(".cta");
var check = 0;
var arrowUp = '<svg xmlns="http://www.w3.org/2000/svg" width="15" height="15"><path d="M 0.26899589,10.327634 1.9684684,12.027707 7.48019,6.4174652 12.987106,12.027707 14.686578,10.327634 7.48019,3.0167177 Z" class="arrow"/></svg>'
var arrowDown = '<svg width="15" height="15" xmlns="http://www.w3.org/2000/svg" fill-rule="evenodd" clip-rule="evenodd"><path d="M 0.26899589,4.716791 1.9684684,3.0167177 7.48019,8.6269595 12.987106,3.0167177 14.686578,4.716791 7.48019,12.027707 Z" class="arrow"/></svg>'
cta.addEventListener('click', function(e){
    var text = e.target.nextElementSibling;
    var loginText = e.target.parentElement;
    text.classList.toggle('show-hide');
    loginText.classList.toggle('expand');
    if(check == 0)
    {
        cta.innerHTML = arrowUp;
        check++;
        document.querySelector('.call-text').style.filter = 'blur(10px)';
    }
    else
    {
        cta.innerHTML = arrowDown;
        check = 0;
        document.querySelector('.call-text').setAttribute('style', 'filter: blur(0px)');
    }
})

/* Modal*/
var modal = document.querySelector("#modal");
var modalOverlay = document.querySelector("#modal-overlay");
var closeButton = document.querySelector("#close-button");
var openButton = document.querySelector("#signup-btn");

closeButton.addEventListener("click", function() {
  modal.classList.toggle("closed");
  modalOverlay.classList.toggle("closed");
});


openButton.addEventListener("click", function() {
  modal.classList.toggle("closed");
  modalOverlay.classList.toggle("closed");
});

/* Check form valid*/
var inputs = document.querySelectorAll('#form input');
var vssid = document.getElementById('ssid');
var vpwd = document.getElementById('pwd');
var vhost = document.getElementById('host');
var vtoken = document.getElementById('token');
var formbtn = document.getElementById('signup-btn');

// var val_pwd = ()=> {return vpwd.value ==''}
// var val_host = ()=> {return vhost.value ==''}
// var val_token = ()=> {return vtoken.value ==''}
var check_input = function() {
    console.log(vpwd.value=='' && vhost.value=='');
    if (vpwd.value!='' && vhost.value!='' && vtoken.value!='') { // 
        formbtn.disabled = false;
        formbtn.style.backgroundColor = "#000";
    }else {
        formbtn.disabled = true;
        formbtn.style.backgroundColor = null;
    }
}
vpwd.addEventListener('change', check_input);
vhost.addEventListener('change', check_input);
vtoken.addEventListener('change', check_input);
// for(i in inputs){
//     inputs[i].addEventListener('change', check_input);
//     inputs[i].addEventListener('keyup', check_input);
//     console.log(inputs[i]);
// }

/* Show hide password */
var eye_close = '<path class="eye" d="M50.6 30l-.9.3c-.4 0-.8-.2-1-.5L45 25a24.8 24.8 0 01-6 4.5l2 5.5c.3.8 0 1.7-.7 2h-.6c-.5 0-1-.3-1.2-.8l-2.1-5.6c-2.5 1-5.5 1.7-8.9 1.8v6c0 .8-.6 1.4-1.4 1.4-.7 0-1.4-.6-1.4-1.5v-5.9a28 28 0 01-8.2-1.5l-2 5.3c-.2.5-.7.9-1.2.9l-.5-.1c-.8-.3-1-1.2-.8-2l2-5.2a24.7 24.7 0 01-6.3-4.4l-3.3 4.4c-.3.3-.7.5-1 .5-.4 0-.7-.1-1-.4-.6-.5-.6-1.5-.1-2.1l3.4-4.5A23 23 0 012 17.5c-.3-.8 0-1.7.8-2 .7-.3 1.5 0 1.8.8 0 .2 1.2 2.8 3.8 5.7l.3.3a22 22 0 007.5 5.2h.1c2.7 1.2 6 1.9 9.8 1.9 16.2 0 21.5-13 21.6-13.1.3-.8 1-1.1 1.8-.8.7.3 1 1.2.7 2 0 .2-1 2.6-3.2 5.3l3.8 5c.5.6.4 1.6-.2 2.1z"/>';
var eye_up = '<path d="M48.6 26.83c.24-.43.24-.96 0-1.4a25.69 25.69 0 00-45.18 0c-.24.44-.24.97 0 1.4a25.69 25.69 0 0045.19 0zM26.02 37a10.85 10.85 0 110-21.71 10.85 10.85 0 010 21.7z"/><path d="M26.01 20.6c-.45 0-.9.06-1.32.16a2.87 2.87 0 01-4.05 4.05 5.54 5.54 0 005.37 6.85 5.53 5.53 0 000-11.06z"/>';
function showpwd() {
  var x = document.getElementById("pwd");
  var y = document.getElementById("eye");
  if (x.type === "password") {
    x.type = "text";
    y.innerHTML = eye_up;
    console.log(y);
  } else {
    x.type = "password";
    y.innerHTML = eye_close;
  }
}
