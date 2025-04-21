#ifndef OSC_UTIL_HPP_INCLUDED
#define OSC_UTIL_HPP_INCLUDED

#include "message.hpp"
#include <vector>
#include <string>
#include <arpa/inet.h>

namespace OSC
{
  
  inline size_t padBuffer(size_t len)
  {
    return (4 - (len % 4)) % 4;
  }

  inline void writePaddedString(std::vector<uint8_t> buffer, const std::string& str)
  {
    buffer.insert(buffer.end(), str.begin(), str.end()); 
    buffer.push_back('\0');
    size_t pad = padBuffer(str.size()+1);
    buffer.insert(buffer.end(), pad, '\0');
  }

  inline void writeInt32(std::vector<uint8_t>& buffer, int32_t val)
  {
    uint32_t net = htonl(static_cast<uint32_t>(val));
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&net);
    buffer.insert(buffer.end(), ptr, ptr + 4);
  }
  
  inline void writeFloat(std::vector<uint8_t>& buffer, float val)
  {
    uint32_t net;
    std::memcpy(&net, &val, sizeof(float));
    net = htonl(net);
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&net);
    buffer.insert(buffer.end(), ptr, ptr + 4);
  }

  inline void serializeMsg(std::vector<uint8_t>& buffer, const std::vector<ArgType>& args)
  {
    
  }

  inline void deserializeMsg(std::vector<uint8_t>& buffer)
  {
    
  }


}






#endif // OSC_UTIL_HPP_INCLUDED
