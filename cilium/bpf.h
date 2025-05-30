#pragma once

#include <cstdint>
#include <string>

#include "source/common/common/logger.h"

union bpf_attr;

namespace Envoy {
namespace Cilium {

/**
 * Bpf system call interface.
 */
class Bpf : public Logger::Loggable<Logger::Id::filter> {
public:
  /**
   * Create a bpf map object without actually creating or opening a bpf map yet.
   * @param map_type the type of a bpf map to be opened or created, e.g.,
   * BPF_MAP_TYPE_HASH
   * @param key_size the size of the bpf map entry lookup key.
   * @param min_value_size the minimum acceptable size of the bpf map entry value.
   * @param max_value_size the maximum size of the bpf map entry value the caller is buffering for.
   *        Defaults to same as min_value_size.
   */
  Bpf(uint32_t map_type, uint32_t key_size, uint32_t min_value_size, uint32_t max_value_size = 0);
  virtual ~Bpf();

  /**
   * Close the bpf file descriptor, if open.
   */
  void close();

  /**
   * Open an existing bpf map. The bpf map must have the map type and key and
   * value sizes that match with the ones given to the constructor.
   * @param path the file system path to the pinned bpf map.
   * @returns boolean for success of the operation.
   */
  bool open(const std::string& path);

  /**
   * Lookup an entry from the bpf map identified with the key, storing the found
   * value, if any.
   * @param key pointer to the key identifying the entry to be found.
   * @param value pointer at which the value is copied to if the entry is found.
   *        Enough space must be provided for 'max_value_size_' bytes.
   *        The caller should only examine the first 'min_value_size_' bytes.
   * @returns boolean for success of the operation.
   */
  bool lookup(const void* key, void* value);

protected:
  std::string path_;
  int fd_;

public:
  uint32_t map_type_;
  uint32_t key_size_;
  uint32_t min_value_size_;
  uint32_t max_value_size_;
  uint32_t real_value_size_;
};

} // namespace Cilium
} // namespace Envoy
