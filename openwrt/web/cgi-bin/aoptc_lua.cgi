#!/usr/bin/lua
local function main()
    print("Content-Type: text/json")
    print("")
    qs = os.getenv'QUERY_STRING'
	i, j = string.find(qs, "&")
	cmd = string.sub(qs, 1, j - 1)
	wserial=io.open("/dev/ttyUSB0","w")
  	wserial:write(cmd)
	wserial:flush()
	wserial:close()
	print("{")
	print("cmd: " .. cmd)
	print("}")
end

main()
