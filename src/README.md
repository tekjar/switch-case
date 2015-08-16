It is in early development stage......................

Usage with curl:
curl -X POST -H Content-Type:application/json -d '{"switchboard": {"5": {"mode": "set",  "value": 1,  "ack": "ok"  },"7": {"mode": "set",  "value": 0,  "ack": "ok"  }  } }' <Wifi module ip>:80

Quick ref to get ip from module:(NodeMcu commands)
ip = wifi.sta.getip()
print(ip)