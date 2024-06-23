#ifndef FORMATTING_HPP_INCLUDED
#define FORMATTING_HPP_INCLUDED

#include <sycl/sycl.hpp>
#include <format>
#include <string>
#include <string_view>

namespace std {
  // Formatting support for SYCL vectors.
  template<class T, size_t N>
  struct formatter<sycl::vec<T, N>> : public std::formatter<std::string_view> {
    auto format(const sycl::vec<T, N>& vec, std::format_context& ctx) const {
      std::string buffer;
      std::format_to(std::back_inserter(buffer), "[{}", vec[0]);
      for (size_t i = 1; i < N; i++) {
        std::format_to(std::back_inserter(buffer), ", {}", vec[i]);
      }
      buffer.push_back(']');

      return std::formatter<std::string_view>::format(buffer, ctx);
    }
  };
}

#endif