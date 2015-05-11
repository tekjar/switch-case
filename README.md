# hng-switchboard

###PROJECT GOALS

- [ ] Smart switch board controllable throuhg mobile phone and home automation hubs
- [ ] REST APIs
- [ ] Save state changes
- [ ] Proximity awareness
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



