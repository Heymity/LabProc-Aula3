/**
 * @file builtinfiles.h
 * @brief This file is part of the WebServer example for the ESP8266WebServer.
 *
 * This file contains long, multiline text variables for  all builtin resources.
 */

// used for $upload.htm
static const char calcContent[] PROGMEM =
  R"==(
<!doctype html>
<html lang='en'>

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Calculadora</title>
</head>

<body>
  <div style="width: 100%; display: flex; flex-direction: column; justify-content: center; align-items: center;">
    <h1>Calculadora</h1>
  </div>
    <hr>
  <div style="width: 100%; display: flex; flex-direction: column; justify-content: center; align-items: center;"> 
    <input id="a" type="number"></input>
    <fieldset id="op">
      <legend>Operação</legend>
      <input type="radio" id="+" name="op" value="+">
      <label for="+">+</label>
      <input type="radio" id="-" name="op" value="-">
      <label for="-">-</label>
    </fieldset>
    <input id="b" type="number"></input>
    <br/>
    <button id="calc" style="margin: 10px;">Calcular!</button>
    <br/>
    <p id="res"></p>
    <p id="over"></p>
  </div>
  <script>
    let a = document.getElementById('a');
    let b = document.getElementById('b');
    let calc = document.getElementById('calc');
    let res = document.getElementById('res');
    let overflow = document.getElementById('over');

    calc.addEventListener('click', () => {
      let op = document.getElementById('+').checked ? "+" : "-"

      if (op === "+")
        res.textContent = +a.value + +b.value + "(+ Calculado no js!)"
      else 
        res.textContent = +a.value - +b.value + "(- Calculado no js!)"
    
      fetch(`/calc?op=${op === "+" ? "p" : "m"}&a=${+a.value}&b=${+b.value}`, { method: 'GET' }).then(response => {
        response.json().then(json => {
          res.textContent = json.res
          if (json.overflow)
            overflow.textContent = "Houve overflow";
          else 
            overflow.textContent = "";
        })        
      });  
    
     }, false)
  </script>
</body>
)==";

// used for $upload.htm
static const char notFoundContent[] PROGMEM = R"==(
<html>
<head>
  <title>Resource not found</title>
</head>
<body>
  <p>The resource was not found.</p>
  <p><a href="/">Start again</a></p>
</body>
)==";
