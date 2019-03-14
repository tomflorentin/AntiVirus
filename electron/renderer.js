// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.


var $ = require("jquery");

$(document).ready(function() {
    $.ajax({
        url: "http://127.0.0.1:6890/getInfo",
        method: "POST",
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        data: JSON.stringify(),
        timeout: 10000,
        success: function (data) {
            let infoData = data;
            changeLog(infoData);
        },
        error: function () {
            alert("cé nul");
        }
    });
    setInterval(function () {
        $.ajax({
            url: "http://127.0.0.1:6890/getInfo",
            method: "POST",
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            data: JSON.stringify(),
            timeout: 10000,
            success: function (data) {
                let infoData = data;
                changeLog(infoData);
            },
            error: function () {
                alert("cé nul");
            }
        });
    }, 500);

})

function changeLog(infoData)
{
    console.log(infoData);
    $("#lastScan").html(infoData.last);
    $("#nextScan").html(infoData.next);
    $("#update").html(infoData.update);
    $("#itemScanned").html(infoData.item);
    $("#scanDetection").html(infoData.detection);
    $("#runpeDetection").html(infoData.timeDetection);
    if (infoData.web == "true") {
        $("#webProtection").attr("src", "img/ok.png")
    } else {
        $("#webProtection").attr("src", "img/ko.png")
    }
    if (infoData.exploit == "true") {
        $("#exploitProtection").attr("src", "img/ok.png")
    } else {
        $("#exploitProtection").attr("src", "img/ko.png")
    }
    if (infoData.malware == "true") {
        $("#malwareProtection").attr("src", "img/ok.png")
    } else {
        $("#malwareProtection").attr("src", "img/ko.png")
    }
}

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

