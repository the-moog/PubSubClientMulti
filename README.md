**PubSubClientMulti**

*NOTE:*  Work in progress:

TODO:
 - Update examples
 - Add test cases
 - and test it better

*About:*
This is essentially a fork that reworks [ArduinoPubSubClientTools](https://github.com/dersimn/ArduinoPubSubClientTools), I am not sure about backwards compatability though, hence the name change.

- Altered to make the internals more flexible
- Add authentication
- Use more modern C++ syntax (probably at the expense of less diverse platform compatability)
- Remove the number of needless overloads
- Add unsubscribe method
- Add pause method
- Easier to interface with other libraries through overloads on publish, subscribe etc. 

*Naming:*  The prefix Arduino has been removed to better associate it with a hard dependency on PubSubClient
The suffix Multi has been added to indicate it's one and only function, managing Many<->Many mapping of feeds to callbacks.

*Rationalle:*  The purpose of this project is to make it easier to support and manage subscribe, unsubscribe 
of multiple feeds with multiple callbacks.

*What does it do?*  Essentially this makes it easier to use the single MQTT topic handler callback function provided in PubSubClient and allow the user to provide as many callback functions as they need, limited only by available resources on the target CPU platform.  It also adds the ability to use a secure connection and unsubscribe or pause feeds.

*Platform:*   Though this has been built and tested on an ESP8266/Arduino 3.3.1 using Arduino IDE 1.8.19, though it uses no specific features of that device and should work on any device or platform that has a port of PubSubClient and that has enough resources CPU with toolchain that supports a reasonable implementation of C++11 and upwards.

I used a NodeMCU E12 v1.0 with 4MB ROM and even with HTTP, TLS, WifI, Threading and Secure Websockets it uses about 50% of the resources.

### Example Manifest:
- ESP8266HTTPClient     1.2
- ESP8266WiFi           1.0
- PubSubClient          2.8  (Hard dependency)
- PubSubClientTools     0.6
- WebSocketStreamClient 0.1.0
- ArduinoHttpClient     0.4.0
- ArduinoThread         2.1.1

## Example Applications

*TODO:*   Update existing examples to show new features.

The library comes with a number of example sketches. See File > Examples > PubSubClientTools
within the Arduino application.

## Credits

[PubSubClient](https://github.com/knolleary/pubsubclient)
[ArduinoPubSubClientTools]https://github.com/dersimn/ArduinoPubSubClientTools
