/**
 * Welcome to Pebble Chuck Says...
 *
 * Retrieve a Chuck Norris joke from http://api.icndb.com/jokes/random
 * Full documentation on
 *    http://www.icndb.com/api/ - The Internet Chuck Norris Database
 * Display Chuck Norris joke text from AJAX request on Pebble Watch Window
 *
 * Copyright (c), 2015
 *
 * @AUTHOR Predrag Janjetovic, preddi(at)arcor(dot)de.
 * @AUTHOR Oliver Merkel, merkel(dot)oliver(at)web(dot)de.
 * 
 * License: The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ------------------------------------------------------------------
 *
 * Credits to
 *   The Internet Chuck Norris Database for their free offer of jokes and
 *   Robert Reid for the MIT licensed encoder.js
 * Thank you!
 *
 */
var Encoder = require('encoder');
var UI = require('ui');
var Vector2 = require('vector2');
var window = new UI.Window({
  fullscreen: true,
  scrollable: true
});
var message = null;

function showMessage( data, status, request) {
  var text = Encoder.htmlDecode( data.value.joke);
  if( message) window.remove( message);
  message = new UI.Text({
    position: new Vector2(0, 0),
    size: new Vector2(144, 1280),
    font: 'gothic-24-bold',
    text: text,
    textAlign: 'center'
  });
  window.add(message);
  window.show();
}

var ajax = require('ajax');
function getNextMessage( e) {
  ajax(
    {
      url: 'http://api.icndb.com/jokes/random',
      type: 'json'
    },
    showMessage,
    function(error, status, request) {
      console.log('The ajax request failed: ' + error);
    }
  );
}

window.on('click', 'select', getNextMessage);
showMessage({value:{joke:'Chuck says ...'}});
getNextMessage( );
