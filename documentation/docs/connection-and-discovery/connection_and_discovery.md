###MQTT CONNECTION

1. How to connect local mqtt broker to wifi network ?

2. How to connect all the switch board to above local mqtt broker? Its ip will be dynamically assigned by the router.

3. How to connect client to local mqtt broker ?



###MQTT DISCOVERY
---

All the switch boards will subscribe to following topic

```
discovery-request/hello
```

All the clients will subscribe to
```
discovery-reply/hi
```

Only clients in local network will be able to communicate with switch boards. 

Two levels of security here.

1. Wifi network authentication
2. Mqtt broker authentication


And clients will publish to `discovery-request/hello` with message `ping`. Switch board will respond with message `MAC=$MAC_ADDRESS` on topic `discovery-reply`

#####NOTES:

* Mobile client will add the discovered switch board to its `known devices` list.

* Only switch boards which are not added to `known-devices` of the client will be visible during discovery.

* Switch board GUI Will be highlighted only when switchboards are connected to local broker. Broker will keep note of connected switch boards




