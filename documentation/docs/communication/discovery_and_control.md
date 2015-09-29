###MQTT DISCOVERY
---

All the switch boards will subscribe to following topic

```
switch-case/find             ---> //discovery request
```

and publish to

```
switch-case/$MAC_ADDR/find-reply
```


All the phones will subscribe to
```
switch-case/+/find-reply     ---> //discovery reply. (with wild card)
```

mobile clients will publish to `switch-case/find` with message `hi`. Switch board will respond with below message on topic `switch-case/$MAC_ADDR/find-reply`

```
hello 
//May be some more data in proper json. Number of devices actually connected? TBD.

```

Since all the switch boards will subscibe to same topic `switch-case/find`, all of them will receive discovery requests and since all the mobile phones subscribe with wildcard, all of them will receive discovery replys.

**CASE 1: NEW PHONE**: With 1 publish to `switch-case/find`, all the switch boards will be found

**CASE 2: NEW BOARD**: Any 1 phone publishing to `switch-case/find`, all the phones will receive the reply

#####NOTES

* Mobile client will add the discovered switch board to its `known devices` list.

* Only switch boards which are not added to `known-devices` of the client will be visible during discovery.


###MQTT CONTROL
---

In case of control, `control request` from a phone should be directed to a particualar switch board and `control reply` from the switch board should be received by all the phones

All the boards will subscribe to its `MAC ADDRESS` topic. Mobile phones will know of this `MAC` during discovery.

Boards subscribe to 
```
switch-case/$MAC_ADDR/request
```

and publish replys to 
```
switch-case/$MAC_ADDR/reply
```

mobile phones will subscribe to 
```
switch-case/+/reply  (observe the wild card)
```

Now all the mobile phones will receive the reply. Mac address in the reply topic can be used for debugging


#####NOTES

* Switch board GUI Will be highlighted only when switchboards are connected to local broker. Broker will keep note of connected switch boards

###SECURITY


Only clients in local network will be able to communicate with switch boards. 

Two levels of security here.

1. Wifi network authentication
2. Mqtt broker authentication
