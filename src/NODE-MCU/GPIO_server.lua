-- A simple http server

-- 5 green
-- 6 red
-- 7 blue
red = 6
green = 5
blue = 7


-- -----------------------------------------
-- adding and removing from lua list
function addToSet(set, key)
    set[key] = true
end

function removeFromSet(set, key)
    set[key] = nil
end

function setContains(set, key)
    return set[key] ~= nil
end

function Set (list)
  local set = {}
  for _, l in ipairs(list) do set[l] = true end
  return set
end

function print_table (inst, t)
  for k,v in pairs(t) do
    print(inst, k, v)
  end

end
-----------------------------------------------
--
switch = Set{5}
regulator = Set{7,6}
instances = {}

for k,v in pairs(switch) do
  addToSet(instances, tostring(k))
end

for k,v in pairs(regulator) do
  addToSet(instances, tostring(k))
end

print("instances: ", instances)

-- initialize pin to GPIO mode
-- pin = green -- 5
for gpio_pin,v in pairs(switch) do
  gpio.mode(gpio_pin, gpio.OUTPUT)
end

-- print(gpio.read(pin))

-- set pin to PWM mode and Start PWM. Only 3 pins can be set to PWM mode at the most.
-- API - pwm.setup(pin, clock, duty)
-- pwm_pin = blue -- 7
for pwm_pin,v in pairs(regulator) do
  pwm.setup(pwm_pin, 1, 0)
  pwm.start(pwm_pin)
end

-- dump tables to the console.
function print_r ( t )
    local print_r_cache={}
    local function sub_print_r(t,indent)
        if (print_r_cache[tostring(t)]) then
            print(indent.."*"..tostring(t))
        else
            print_r_cache[tostring(t)]=true
            if (type(t)=="table") then
                for pos,val in pairs(t) do
                    if (type(val)=="table") then
                        print(indent.."["..pos.."] => "..tostring(t).." {")
                        sub_print_r(val,indent..string.rep(" ",string.len(pos)+8))
                        print(indent..string.rep(" ",string.len(pos)+6).."}")
                    elseif (type(val)=="string") then
                        print(indent.."["..pos..'] => "'..val..'"')
                    else
                        print(indent.."["..pos.."] => "..tostring(val))
                    end
                end
            else
                print(indent..tostring(t))
            end
        end
    end
    if (type(t)=="table") then
        print(tostring(t).." {")
        sub_print_r(t,"  ")
        print("}")
    else
        sub_print_r(t,"  ")
    end
    print()
end


-- --------------------------------------------------------------------------------

function decode_encode(pos,val)
   print('$$',pos)
  if ("set" == val.mode) then
    -- TODO: set GPIO pin
    if ( switch[pos] ) then
        if (val.value == 0) then
            gpio.write(pos, gpio.LOW)
        else
            gpio.write(pos, gpio.HIGH)
        end
        val.value = gpio.read(pos)
    elseif (regulator[pos]) then
        if (0 == val.value) then
            pwm.setduty(pos, 0)
        else
            pwm.setclock(pos, val.value)
            pwm.setduty(pos, 512)
        end
        val.value = pwm.getclock(pos)
    else
        return ("ERROR4")
    end
    -- print_table(22,val)
  elseif ("get" == val.mode) then
    if ( switch[pos] ) then
      print_table(33,val)
      val.value = gpio.read(pos)
      print_table(44,val)
    elseif (regulator[pos]) then
      val.value = pwm.getclock(pos)
    else
        return ("ERROR3")
    end

    print("NOT SET")
  end
  return val
end
-- rett = 0
function print_r_test ( t )
    local function sub_print_r(t)
      for pos,val in pairs(t) do
        print('-----',pos,val)
        if (instances[pos]) then
          decode_encode(tonumber(pos),val)

        else
          print('22',pos,val)
          if (type(val)=="table") then
            return(sub_print_r(val))

          else
            return ("ERROR2")
          end
        end
        print("logging")

      end
      return t
    end
    if (type(t)=="table") then
        return(sub_print_r(t))
    else
        return ("ERROR1")
    end

end


srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
  conn:on("receive",function(conn,payload)
    print("---------------------------------------------")
    json_text = string.match(payload,"%b{}")
    print("json_text: ", json_text)
    value = cjson.decode(json_text)
    print("value: ",value)
    for k, v in pairs(value) do print(k,"|aa|",v) end

    print("---------------------------------------------")
    ret = print_r_test( value )
    print_r(value)
    print("---------------------------------------------")
--    query_data = get_http_req (payload)
--    print (query_data["METHOD"] .. " " .. " " .. query_data["User-Agent"])
--    print(#query_data)
--    for k, v in pairs(query_data) do print(k,"...",v) end
    --print
    ret = cjson.encode(ret)
    conn:send("<h1> Hello, NodeMcu.</h1>")
    conn:send(ret)
--    conn:send(payload)
  end)
  conn:on("sent",function(conn) conn:close() end)
end)
