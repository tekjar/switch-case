# hng-switchboard

###PROJECT GOALS

- [ ] Smart switch board controllable throuhg mobile phone and home automation hubs
- [ ] REST APIs
- [ ] Save state changes
- [ ] Proximity awareness
- [ ] Power consumption
- [ ] Hackable
- [ ] Beautiful, Led backlit,  Ambient light sensor + IR transreceiver with learning mode (for selected models)

### SOFTWARE FEATURES

- [ ] Manual control, Mobile Control, Contextual, Security

###WIFI vs BLE vs ZIGBEE

1 -> WIFI, 2 -> BLE, 3 -> ZIGBEE

#####A. CONNECTION
1. Requires Gateway. So, phone should be connected to network
2. Peer to peer
3. Peer to peer

#####B. SIGNAL RANGE
1. Better range which can be extended with repeaters
2. Short range. Can be increased if devkits support MESH
3. Very good range due to mesh

#####C. PROXIMITY AWARENESS
1. No. Need to add some BLE central devices to home
2. Yes
3. No. Need to add some BLE central devices to home

#####D. HARDWARE AVAILABILITY
1. Tons of development boards available
2. Need devkits with BLE central support for integrating BLE proximity feature into switch board
3. Not present in any modern cellphones. Need to route through a hub with zigbee (can rule out zigbee due to this reason)

#####E. SOFTWARE EFFORT
1. Super easy due to IP stack
2. -
3. -

#####F. COMMUNITY SUPPORT
1. Excellent community support
2. Low community support
3. Low community support

#####G. CLOUD SERVER 

On and off of electrical devices is not the goal.
Collecting and analyzing data  providing valuable insights and creating actions is the key .

For such task we need a pc in internet which can 1)talk to our device gather data . 
2)act as the backend for apps by providing apis to query and give data as response .
3) have excellent analytics .
4) extendable: it collects data . but the developer should be able to write code to draw insights and act on them .

How to solve this ?

Ad our primary focus is not tocreate a new cliud software . we use existing ones 

1) xively 
2) sparks now particle are few examples .

Even IBM is providing blue mix . but its more like an enterprise solution.

Will update on how these are evolved .


