// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.


var $ = require("jquery");
$.get('dashbord.html', function(data) {
    $("#middle_col").html(data)
}, "text");
   $("#first_col").click(function () {
       $.get('dashbord.html', function(data) {
           $("#middle_col").text(data)
       }, "text");
    });
     $("#second_col").click(function () {
         $.get('scan.html', function(data) {
             $("#middle_col").html(data)
         }, "text");
    });

    $("#third_col").click(function () {
        $.get('monitoring.html', function(data) {
            $("#middle_col").html(data)
        }, "text");
    });

    $("#quarth_col").click(function () {
        $.get('scanReport.html', function(data) {
            $("#middle_col").html(data)
        }, "text");
    });
    $("#fift_col").click(function () {
        $.get('parameter.html', function(data) {
            $("#middle_col").html(data)
        }, "text");
    });
