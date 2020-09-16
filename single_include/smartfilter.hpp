#pragma once

#include <cstring>
#include <vector>
#include <iostream>
#include <string_view>

#include <hs.h>

using namespace std;

#ifdef DEBUG
#define DD(fmt, args...) printf(fmt, ##args)
#else
#define DD(fmt, args...)
#endif

#define ERROR(fmt, args...) fprintf(stderr, fmt, ##args)

#define FLAGS_FAST   (HS_FLAG_CASELESS    | \
		HS_FLAG_SINGLEMATCH | \
		HS_FLAG_DOTALL)

#define FLAGS_LEFTMOST        (HS_FLAG_CASELESS    | \
		HS_FLAG_DOTALL      | \
		HS_FLAG_SOM_LEFTMOST)

class hpsfilter  {
	public:
		hpsfilter(vector<const char *> exprs, vector<unsigned int> ids, vector<unsigned int> flags) : exprs(_exprs), ids(_ids), flags(_flags), _init_ok(false) {
			if (_exprs.size() != _ids.size() ||  _ids.size() != _flags.size()) {
				ERROR("Error: exprs.size:%d _ids.size:%s flags.size:%d\n", _exprs.size(), _ids.size(), _flags.size());
				_init_ok = false;
			}
		}

		~hpsfilter() {
			if (_scratch) {
				hs_free_scratch(_scratch);
				_scratch = nullptr;
			}
			if (f->database) {
				hs_free_database(_db);
				_db = nullptr;
			}
		}

		//init from bytes db 
		int init(const char *byte_db, const size_t len) {
			if (byte_db == nullptr ||  len == 0) {
				ERROR("byte_db:%p len:%d\n")
			}
			hs_error_t err = hs_deserialize_database(byte_db, len, _db)
			if ( err != HS_SUCCESS) {
				ERROR("Error: code:%d\n", err);
				return -1;
			}

			err = hs_alloc_scratch(_db, _scratch);
			if (err != HS_SUCCESS) {
				ERROR("ERROR: error hs_alloc_scratch code:%d\n", err);
				return -1;
			}

			_init_ok = true;
			return 0;
		}

		//init from exprs
		int init() {
			hs_compile_error_t *compile_err = nullptr;
			hs_error_t err;

			err = hs_compile_multi(_exprs.data(),
					flags.data(),
					ids.data(),
					exprs.size(),
					HS_MODE_BLOCK, 
					nullptr,  //platform
					&_db,
					&compile_err);
			if (err != HS_SUCCESS) {
				if (compile_err->expression < 0) {
					ERROR("Error:%s\n", compile_err->message);
					hs_free_compile_error(compile_err);
				} else {
					ERROR("ERROR: Pattern %s' failed with error:%s\n" ,exprs[compile_err->expression], compile_err->message);
					hs_free_compile_error(compile_err);
				}
			}

			//alloc scratch
			err = hs_alloc_scratch(_db, _scratch) 
			if ( err != HS_SUCCESS) {
				ERROR("ERROR: Unable to allocate scratch space. Exiting. code:%d\n", err);
			}
	
			_init_ok = true;

			return 0;
		}

	private:
		bool _init_ok;
		hs_database_t *_db;
		hs_scratch_t *_scratch;

		vector<const char *> _exprs; //need free
		vector<unsigned int> _ids;
		vector<unsigned int> _flags; 

		hpsfilter(const hpsfilter &) = delete;
		hpsfilter &operator=(const hpsfilter &) = delete;
};
