#ifndef OSC_MESSAGE_HPP_INCLUDED
#define OSC_MESSAGE_HPP_INCLUDED


#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <variant>


using ArgType = std::variant<std::string, int32_t, float>;

namespace OSC 
{

  class Message 
  {

    Message(const std::string& addr = "") : m_addr(addr) {}

    Message(const std::string& addr, const std::string& tags, const std::vector<ArgType>& args) 
      : m_addr(addr), m_tags(tags), m_args(args)
    {
      
      for (int i = 0; i < tags.size(); i++)
      {
        char tag = tags.at(i);
        const ArgType& arg = args[i];
        bool match = false;

        switch (tag) 
        {
          case 's':
            match = std::holds_alternative<std::string>(arg);
            break;
          case 'i': 
            match = std::holds_alternative<int32_t>(arg);
            break;
          case 'f':
            match = std::holds_alternative<float>(arg);
            break;
          default:
            std::cerr << "Unknown tag: " << tag << " at index: " << i << '\n';
        }

        if (!match) 
        {
          std::cerr << "Type mismatch: " << "tags string does not match the arguments" << '\n';
        }
          
      }
    }
      

    void setAddr(const std::string& addr) 
    {
      m_addr = addr;
    }

    void setTags(const std::string& tags) 
    {
      m_tags = tags;
    }

    void setArgs(const std::vector<ArgType>& args) 
    {
      m_args = args;
    }

    void addArg(const ArgType& arg) 
    {
      m_args.push_back(arg);
    }

    const std::string& getAddr() const
    {
      return m_addr;
    }

    const std::string& getTags() const
    {
      return m_tags;
    }

    const std::vector<ArgType>& getArgs() const
    {
      return m_args;
    }


    private:
      std::string m_addr;
      std::string m_tags;
      std::vector<ArgType> m_args;
  };

}

#endif
