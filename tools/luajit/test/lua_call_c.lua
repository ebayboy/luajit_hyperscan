local ffi=require"ffi"
local myffi=ffi.load("libmyclib")

--lua 数组映射到C层
ffi.cdef[[
int fast(int *t,int n,int m);
]]  

local t={55,29,11,33,222,77,20,55,90,0}
local len=#t
local t_c=ffi.new("int[?]",len,t)
local re=myffi.fast(t_c,0,len-1)

--因为得到的t_c为cdata数据，需要转换回lua数据
local sorted_t={}
for i=1,len do
	sorted_t[i]=t_c[i-1]
end

print("origin t: ",table.concat(t,","))
print("sorted t: ",table.concat(sorted_t,","))
