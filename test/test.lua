local ffi=require"ffi"

local myffi=ffi.load("libsmartfilter")

ffi.cdef[[
void *filter_new(const char *name, const char **patterns, unsigned int *flags, unsigned int *ids, size_t size);
int filter_match(void *filter, const char *inputData, size_t dlen);
void *filter_delete(void *f);
void hello();
void printArray(int *a, size_t size);
void printStrArray(const char **a, size_t size);
]]  

local lua_patterns = {"1001", "1002"}
local lua_flags = { 2, 2 }
local lua_ids = {101, 102}

--lua数组下表从1开始， C数组下标从0开始

-- set patterns
local patterns = ffi.new(ffi.typeof('const char *[?]'), #lua_patterns)
for i = 1, #lua_patterns, 1 do
	patterns[i-1] = ffi.new('char[?]', #lua_patterns[i], lua_patterns[i])
end

-- set flags
local flags = ffi.new('unsigned int [?]', #lua_flags, lua_flags)

-- set ids
local ids = ffi.new('unsigned int [?]', #lua_ids, lua_ids)

--set name
local name = ffi.new('const char [?]', #"@url", "@url")

local filter = myffi.filter_new(name, patterns, flags, ids, #lua_ids)

local buf = "asdfasf1001adfa1002sadfaf"
myffi.filter_match(filter, buf, #buf)

myffi.filter_delete(filter);
