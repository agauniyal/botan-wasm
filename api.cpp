#include <string>

#include <botan/auto_rng.h>
#include <botan/system_rng.h>
#include <botan/rsa.h>
#include <botan/pkcs8.h>

#include <emscripten/bind.h>

using namespace emscripten;

// see https://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/embind.html#embind

std::string privateKey(const int t)
{
  return Botan::PKCS8::PEM_encode(Botan::RSA_PrivateKey(Botan::system_rng(), 4096));
}

EMSCRIPTEN_BINDINGS(my_module)
{
  function("generatePrivateKey", &privateKey);
}
