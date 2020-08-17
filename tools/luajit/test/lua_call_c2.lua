local ffi=require"ffi"
local myffi=ffi.load("libmyclib")

--lua 数组映射到C层
ffi.cdef[[
int printf(const char *format, ...);
]]  

myffi.printf("hello %d\n", 32)

