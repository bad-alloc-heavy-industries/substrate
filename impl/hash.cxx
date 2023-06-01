#include "substrate/hash"

#if __cplusplus < 201703L
const std::array<const uint32_t, 256> substrate::crc32_t::crcTable;
#endif
