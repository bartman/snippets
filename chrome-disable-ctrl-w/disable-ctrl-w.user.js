// ==UserScript==
// @name       disable ctrl+w
// @namespace  http://use.i.E.your.homepage/
// @version    0.1
// @description  disable ctrl+w
// @match      http://*/*
//
// https://superuser.com/a/735189
//
// ==/UserScript==

document.addEventListener('keydown', function(evt){

    // NOTE: ctrl key is sent here, but ctrl+W is not
    if (evt.ctrlKey) {

        var stopEvilCtrlW = function(e) {
            return "Oopsies, Chrome!";
        },  clearEvilCtrlW = function() {
            window.removeEventListener('beforeunload', stopEvilCtrlW, false);  
        };

        setTimeout(clearEvilCtrlW, 1000);
        window.addEventListener('beforeunload', stopEvilCtrlW, false);
    }

}, false);

