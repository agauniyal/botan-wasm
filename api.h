#include <stdint.h>
#include <stddef.h>

extern "C" {
  int botan_base64_decode(const char* base64_str, size_t in_len,
                                              uint8_t* out, size_t* out_len);

  /**
  * Perform base64 encoding
  */
  int botan_base64_encode(const uint8_t* x, size_t len, char* out, size_t* out_len);
}