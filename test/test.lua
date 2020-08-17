local ffi=require"ffi"

local myffi=ffi.load("libsmartfilter")

ffi.cdef[[

#define RESULT_SET_MAX			8

#define HS_FLAG_CASELESS        1
#define HS_FLAG_DOTALL          2
#define HS_FLAG_SINGLEMATCH     8
#define HS_FLAG_SOM_LEFTMOST    256

#define RULES_HS_FLAGS   (HS_FLAG_CASELESS    | \
		HS_FLAG_SINGLEMATCH | \
		HS_FLAG_DOTALL)

#define RULES_HS_FLAGS_LEFTMOST        (HS_FLAG_CASELESS    | \
		HS_FLAG_DOTALL      | \
		HS_FLAG_SOM_LEFTMOST)


typedef struct {
	unsigned int id;
	unsigned long long from;
	unsigned long long to;
} result_t;

typedef struct {
	unsigned int cursor;
	result_t results[RESULT_SET_MAX]; //new
} result_set_t;

void *filter_new(const char *name, const char **patterns, unsigned int *flags, unsigned int *ids, size_t size);
void filter_delete(void *f);
result_set_t * filter_match(void *filter, const char *inputData, size_t dlen);
void filter_result_set_delete(result_set_t *result_set);

void hello();
void printArray(int *a, size_t size);
void printStrArray(const char **a, size_t size);
]];

local lua_patterns = {"1001", "1002", "1003"}
local lua_flags = { 2, 2, 2 }
local lua_ids = {101, 102, 103}

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
local rset = myffi.filter_match(filter, buf, #buf)

print("rset->cursor:", rset.cursor)

filter_result_set_delete(rset)

myffi.filter_delete(filter);


