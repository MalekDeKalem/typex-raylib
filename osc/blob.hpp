#ifndef OSC_BLOB_HPP_INCLUDED
#define OSC_BLOB_HPP_INCLUDED

#include <cstddef>


namespace OSC {

  class Blob {

    public:

      Blob() 
        : m_size(0), m_data(nullptr)
      {}

      Blob(size_t size, const void* data)
        : m_size(size), m_data(data)
      {}


      size_t getSize() const {
        return m_size;
      } 

      const void* getData() const {
        return m_data;
      }

    private:
      size_t m_size;
      const void* m_data;
  };

}









#endif
