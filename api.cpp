#include <string>

#include <botan/base64.h>
#include <botan/mem_ops.h>
#include "api.h"

enum BOTAN_FFI_ERROR {
   BOTAN_FFI_SUCCESS = 0,
   BOTAN_FFI_INVALID_VERIFIER = 1,

   BOTAN_FFI_ERROR_INVALID_INPUT = -1,
   BOTAN_FFI_ERROR_BAD_MAC = -2,

   BOTAN_FFI_ERROR_INSUFFICIENT_BUFFER_SPACE = -10,

   BOTAN_FFI_ERROR_EXCEPTION_THROWN = -20,
   BOTAN_FFI_ERROR_OUT_OF_MEMORY = -21,
   BOTAN_FFI_ERROR_SYSTEM_ERROR = -22,
   BOTAN_FFI_ERROR_INTERNAL_ERROR = -23,

   BOTAN_FFI_ERROR_BAD_FLAG = -30,
   BOTAN_FFI_ERROR_NULL_POINTER = -31,
   BOTAN_FFI_ERROR_BAD_PARAMETER = -32,
   BOTAN_FFI_ERROR_KEY_NOT_SET = -33,
   BOTAN_FFI_ERROR_INVALID_KEY_LENGTH = -34,
   BOTAN_FFI_ERROR_INVALID_OBJECT_STATE = -35,

   BOTAN_FFI_ERROR_NOT_IMPLEMENTED = -40,
   BOTAN_FFI_ERROR_INVALID_OBJECT = -50,

   BOTAN_FFI_ERROR_TLS_ERROR = -75,
   BOTAN_FFI_ERROR_HTTP_ERROR = -76,

   BOTAN_FFI_ERROR_UNKNOWN_ERROR = -100,
};

inline int write_output(uint8_t out[], size_t* out_len, const uint8_t buf[], size_t buf_len)
   {
   if(out_len == nullptr)
      return BOTAN_FFI_ERROR_NULL_POINTER;

   const size_t avail = *out_len;
   *out_len = buf_len;

   if((avail >= buf_len) && (out != nullptr))
      {
      Botan::copy_mem(out, buf, buf_len);
      return BOTAN_FFI_SUCCESS;
      }
   else
      {
      if(out != nullptr)
         {
         Botan::clear_mem(out, avail);
         }
      return BOTAN_FFI_ERROR_INSUFFICIENT_BUFFER_SPACE;
      }
   }

template<typename Alloc>
int write_vec_output(uint8_t out[], size_t* out_len, const std::vector<uint8_t, Alloc>& buf)
   {
   return write_output(out, out_len, buf.data(), buf.size());
   }

inline int write_str_output(uint8_t out[], size_t* out_len, const std::string& str)
   {
   return write_output(out, out_len,
                       Botan::cast_char_ptr_to_uint8(str.data()),
                       str.size() + 1);
   }

inline int write_str_output(char out[], size_t* out_len, const std::string& str)
   {
   return write_str_output(Botan::cast_char_ptr_to_uint8(out), out_len, str);
   }

inline int write_str_output(char out[], size_t* out_len, const std::vector<uint8_t>& str_vec)
   {
   return write_output(Botan::cast_char_ptr_to_uint8(out),
                       out_len,
                       str_vec.data(),
                       str_vec.size());
   }

extern "C" {
  int botan_base64_encode(const uint8_t* in, size_t len, char* out, size_t* out_len)
    {
      const std::string base64 = Botan::base64_encode(in, len);
      return write_str_output(out, out_len, base64);
    }

  int botan_base64_decode(const char* base64_str, size_t in_len,
                          uint8_t* out, size_t* out_len)
    {
      if(*out_len < Botan::base64_decode_max_output(in_len))
          {
          *out_len = Botan::base64_decode_max_output(in_len);
          return BOTAN_FFI_ERROR_INSUFFICIENT_BUFFER_SPACE;
          }

      *out_len = Botan::base64_decode(out, std::string(base64_str, in_len));
      return BOTAN_FFI_SUCCESS;
    }

}