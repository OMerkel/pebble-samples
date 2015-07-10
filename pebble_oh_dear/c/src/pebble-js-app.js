Pebble.addEventListener('ready',
  function(e) {
    console.log('JavaScript app ready and running!');
  }
);

Pebble.addEventListener('showConfiguration',
  function(e) {
    Pebble.openURL('http://omerkel.github.io/pebble-samples/pebble_oh_dear/config/config-page-v1.html');
  }
);

Pebble.addEventListener('webviewclosed',
  function(e) {
    if (e.response) {
      var configuration = JSON.parse(decodeURIComponent(e.response));
      console.log('Configuration window returned: ' +
        JSON.stringify(configuration));

      var message = {};
      message.KEY_ENABLE_SECOND_HAND = 1;
      message.KEY_SWEEP_MINUTE_HAND = 1;

      if ( 'on' == configuration.enable_second_hand ) {
        message.KEY_ENABLE_SECOND_HAND = 1;
        console.log('Configuration enable_second_hand=on');
      } else {
        message.KEY_ENABLE_SECOND_HAND = 0;
        console.log('Configuration enable_second_hand=off');
      }

      if ( 'sweep' == configuration.behavior_minute_hand ) {
        message.KEY_SWEEP_MINUTE_HAND = 1;
        console.log('Configuration sweep_minute_hand=true');
      } else {
        message.KEY_SWEEP_MINUTE_HAND = 0;
        console.log('Configuration sweep_minute_hand=false');
      }

      Pebble.sendAppMessage(message,
        function(e) {
          console.log('Config message successfully sent to Pebble');
        },
        function(e) {
          console.log('Config message failed to send to Pebble');
        }
      );
    } else {
      console.log('Configuration canceled');
    }
  }
);
