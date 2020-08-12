

--Usage: ./src/luajit test.lua 
--
local ffi = require("ffi")
ffi.cdef[[
int printf(const char *format, ...);
]]

ffi.C.printf("Hello %s!", "world\n")

