#pragma once

#include <iostream>
#include <string_view>
#include <tuple>
#include <vector>
#include <memory>

#include <cstring>

#include <hs.h>

using namespace std;

#ifdef DEBUG

#define DD(fmt, ...) do  { fprintf(stdout, fmt, ##__VA_ARGS__); } while(0)
#else
#define DD(fmt, args...)
#endif

#define ERROR(fmt, ...) do  { fprintf(stderr, fmt, ##__VA_ARGS__); } while(0)

//FLAGS
#define FLAGS_FAST   (HS_FLAG_CASELESS    | \
		HS_FLAG_SINGLEMATCH | \
		HS_FLAG_DOTALL)

#define FLAGS_LEFTMOST        (HS_FLAG_CASELESS    | \
		HS_FLAG_DOTALL      | \
		HS_FLAG_SOM_LEFTMOST)

#define FLAGS_LIT_FAST  \
	(HS_FLAG_CASELESS | HS_FLAG_SINGLEMATCH)

#define FLAGS_LIT_LEFTMOST  \
	(HS_FLAG_CASELESS | HS_FLAG_SOM_LEFTMOST)

class HPSFilter  {
	public:
		HPSFilter(vector<const char *> exprs, vector<unsigned int> ids, vector<unsigned int> flags) : _exprs(exprs), _ids(ids), _flags(flags) {
			if (_exprs.size() != _ids.size() ||  _ids.size() != _flags.size()) {
				ERROR("Error: exprs.size:%lu _ids.size:%lu flags.size:%lu\n", _exprs.size(), _ids.size(), _flags.size());
				_init_ok = false;
			}
		}

		~HPSFilter() {
			if (_scratch) {
				hs_free_scratch(_scratch);
				_scratch = nullptr;
			}
			if (_db) {
				hs_free_database(_db);
				_db = nullptr;
			}
		}

		//init from bytes db 
		int Init(const char *byte_db, const size_t len) {
			if (byte_db == nullptr ||  len == 0) {
				ERROR("byte_db:%p len:%lu\n", byte_db, len);
			}
			hs_error_t err = hs_deserialize_database(byte_db, len, &_db);
			if ( err != HS_SUCCESS) {
				ERROR("Error: code:%d\n", err);
				return -1;
			}

			err = hs_alloc_scratch(_db, &_scratch);
			if (err != HS_SUCCESS) {
				ERROR("ERROR: error hs_alloc_scratch code:%d\n", err);
				return -1;
			}

			_init_ok = true;
			return 0;
		}

		//init from exprs
		int Init() {
			hs_compile_error_t *compile_err = nullptr;
			hs_error_t err;

			err = hs_compile_multi(_exprs.data(),
					_flags.data(),
					_ids.data(),
					_exprs.size(),
					HS_MODE_BLOCK, 
					nullptr,  //platform
					&_db,
					&compile_err);
			if (err != HS_SUCCESS) {
				if (compile_err->expression < 0) {
					ERROR("Error:%s\n", compile_err->message);
					hs_free_compile_error(compile_err);
				} else {
					ERROR("ERROR: Pattern %s' failed with error:%s\n" , _exprs[compile_err->expression], compile_err->message);
					hs_free_compile_error(compile_err);
				}
			}

			//alloc scratch
			err = hs_alloc_scratch(_db, &_scratch);
			if ( err != HS_SUCCESS) {
				ERROR("ERROR: Unable to allocate scratch space. Exiting. code:%d\n", err);
			}
	
			_init_ok = true;

			return 0;
		}

		//init from exprs
		int InitLit() {
			hs_compile_error_t *compile_err = nullptr;
			hs_error_t err;
			vector <size_t> expr_lens;

			for (auto && ex : _exprs) {
				expr_lens.push_back(strlen(ex));
			}

			err = hs_compile_lit_multi(_exprs.data(),
					_flags.data(),
					_ids.data(),
					expr_lens.data(),
					_exprs.size(),
					HS_MODE_BLOCK, 
					0,
					&_db,
					&compile_err);
			if (err != HS_SUCCESS) {
				if (compile_err->expression < 0) {
					ERROR("Error:%s\n", compile_err->message);
					hs_free_compile_error(compile_err);
				} else {
					ERROR("ERROR: Pattern %s' failed with error:%s\n" , _exprs[compile_err->expression], compile_err->message);
					hs_free_compile_error(compile_err);
				}
			}

			//alloc scratch
			err = hs_alloc_scratch(_db, &_scratch);
			if ( err != HS_SUCCESS) {
				ERROR("ERROR: Unable to allocate scratch space. Exiting. code:%d\n", err);
			}
	
			_init_ok = true;

			return 0;
		}

		int Match(string_view sv, vector<tuple<unsigned int, unsigned long long , unsigned long long >> &vec_res){
			hs_error_t err;
			if (!_init_ok)	{
				ERROR("ERROR: _init_ok: false!");
				return -1;
			}
			if (_db == nullptr || _scratch == nullptr) {
				ERROR("ERROR: _db:%p _scratch:%p\n", _db, _scratch);
				return -1;
			}
			
			err = hs_scan(_db, sv.data(), sv.size(), 0, _scratch, _on_match, std::addressof(vec_res));
			if (err != HS_SUCCESS) {
				ERROR("ERROR: err:%d\n", err);
				return -1;
			}

			return 0;
		}

		int DumpDB(std::shared_ptr<char> &sp_data, size_t &len) {
			char *data = nullptr;
			hs_error_t err;
			if ((err = hs_serialize_database(_db, &data, &len) != HS_SUCCESS)) {
				ERROR("ERROR: error:%d\n", err);
				return -1;
			}

			shared_ptr<char> sp(data);
			sp_data = sp;

			return 0;
		}

	private:
		static int _on_match (unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *ctx_vec){
			auto vec = static_cast<vector<tuple<unsigned int, unsigned long long, unsigned long long>> *>(ctx_vec); //ok
			vec->push_back(std::tuple<int, char, double>(id, from, to));

			//DD("id:%u flags:%d", id, flags);

			return 0;
		}

		bool _init_ok  = false;
		hs_database_t *_db;
		hs_scratch_t *_scratch;

		vector<const char *> _exprs; //need free
		vector<unsigned int> _ids;
		vector<unsigned int> _flags; 
		
		//nocopyable
		HPSFilter(const HPSFilter &) = delete;
		HPSFilter &operator=(const HPSFilter &) = delete;
};
