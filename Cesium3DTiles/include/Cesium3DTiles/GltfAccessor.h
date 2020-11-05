#pragma once

#include "Gltf.h"
#include <stdexcept>

namespace Cesium3DTiles {

	template <class T>
	class GltfAccessor {
	private:
		const tinygltf::Buffer* _pGltfBuffer;
		const tinygltf::BufferView* _pGltfBufferView;
		const tinygltf::Accessor* _pGltfAccessor;
		const unsigned char* _pBufferViewData;
		size_t _stride;
		size_t _offset;
		size_t _size;

	public:
		GltfAccessor(const tinygltf::Model& model, size_t accessorID)
		{
			const tinygltf::Accessor& accessor = model.accessors[accessorID];
			const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
			const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

			const std::vector<unsigned char>& data = buffer.data;
			size_t bufferBytes = data.size();
			if (bufferView.byteOffset + bufferView.byteLength > bufferBytes)
			{
				throw std::runtime_error("bufferView does not fit in buffer.");
			}

			int accessorByteStride = accessor.ByteStride(bufferView);
			if (accessorByteStride == -1)
			{
				throw std::runtime_error("cannot compute accessor byteStride.");
			}

			int32_t accessorComponentElements = tinygltf::GetNumComponentsInType(accessor.type);
			int32_t accessorComponentBytes = tinygltf::GetComponentSizeInBytes(accessor.componentType);
			int32_t accessorBytesPerStride = accessorComponentElements * accessorComponentBytes;

			if (sizeof(T) != accessorBytesPerStride)
			{
				throw std::runtime_error("sizeof(T) does not much accessor bytes.");
			}

			size_t accessorBytes = accessorByteStride * accessor.count;
			size_t bytesRemainingInBufferView = bufferView.byteLength - (accessor.byteOffset + accessorByteStride * (accessor.count - 1) + accessorBytesPerStride);
			if (accessorBytes > bufferView.byteLength || bytesRemainingInBufferView < 0)
			{
				throw std::runtime_error("accessor does not fit in bufferView.");
			}

			const unsigned char* pBufferData = &data[0];
			const unsigned char* pBufferViewData = pBufferData + bufferView.byteOffset;

			this->_pGltfBuffer = &buffer;
			this->_pGltfBufferView = &bufferView;
			this->_pGltfAccessor = &accessor;
			this->_stride = accessorByteStride;
			this->_offset = accessor.byteOffset;
			this->_size = accessor.count;
			this->_pBufferViewData = pBufferViewData;
		}

		const T& operator[](size_t i) const
		{
			if (i < 0 || i >= this->_size)
			{
				throw std::range_error("index out of range");
			}

			return *reinterpret_cast<const T*>(this->_pBufferViewData + i * this->_stride + this->_offset);
		}

		size_t size() const
		{
			return this->_size;
		}

		const tinygltf::Buffer& gltfBuffer() const
		{
			return *this->_pGltfBuffer;
		}

		const tinygltf::BufferView& gltfBufferView() const
		{
			return *this->_pGltfBufferView;
		}

		const tinygltf::Accessor& gltfAccessor() const
		{
			return *this->_pGltfAccessor;
		}
	};

}