#include <Veritas/GPU/BufferBinding.h>

#include <Veritas/GPU/Definitions.h>

using namespace Veritas;
using namespace GPU;

BufferBinding::BufferBinding(uint32 location, const Buffer &buffer)
    : BindingPoint(
          [&buffer, location] {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, buffer.getID());
          },
          [location] {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, 0);
          })
{}
