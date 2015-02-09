#!/usr/bin/lua

local function main()
	rserial=io.open("/dev/ttyUSB0","r")
	while true do
		line = nil
		while line==nil do
			line=rserial:read()
			rserial:flush()
			line = nil
		end
	end
end

main()

