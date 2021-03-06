#pragma once

#include <cuda_runtime.h>

/**
 * Class which allows straight forward access to a GPU buffer in cuda code.
 */
template<typename T>
class element_accessor {
public:
    element_accessor(T* buffer_data, size_t const pitch)
        : m_buffer_data(buffer_data)
        , m_pitch(pitch) {}

    inline __device__ T& at(size_t const x, size_t const y) {
        return *(reinterpret_cast<T*>(reinterpret_cast<char*>(m_buffer_data) + y * m_pitch) + x);
    }

    inline __device__ T const& at(size_t const x, size_t const y) const {
        return *(reinterpret_cast<T*>(reinterpret_cast<char*>(m_buffer_data) + y * m_pitch) + x);
    }

private:
    T*     m_buffer_data;
    size_t m_pitch;
};

/**
 * Representation of two-dimensional data in GPU memory.
 */
template<typename T>
class gpu_buffer {
public:
    using value_type = T;

    gpu_buffer(size_t const width, size_t const height)
        : m_width(width)
        , m_height(height) {}

    virtual ~gpu_buffer() = default;

    virtual cudaError_t clear(int value) = 0;

    virtual T* data() = 0;

    virtual T const* data() const = 0;

    virtual element_accessor<T> accessor() = 0;

    virtual element_accessor<T> const accessor() const = 0;

    size_t width() const {
        return m_width;
    }

    size_t height() const {
        return m_height;
    }

private:
    size_t m_width;
    size_t m_height;
};