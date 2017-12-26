#ifndef COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER
#define COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER

#include <Support/BasicUtility.h>
#include <Support/ResourceVHNatives.h>

#include <string>
#include <fstream>
#include <bitset>

namespace CoinBill {
    // The basic Resource Value Handler.
    // We can abstractly access to data, such from file, etc.
    template <class Ty>
    class NOVTABLE ResourceVH {
    public:
		virtual Ty* get() = 0;
    };

    // ResourceSecureVH class will help you to handle values more safely.
    // We will update / check crc value every time when you accessing it.
    // Note that will very slow when you accessing.
    // so make sure you are accessing this variable as multiple times.
    template <class Ty>
    class ResourceSecureVH : public ResourceVH<Ty> {


    public:
        ResourceSecureVH() {

        }
		Ty* get() override { return &PV; }
    };

    template <class Ty>
    class ResourceMappedVH : public ResourceVH<Ty> {
		std::string ResourceName;
		void* ResourcePos;
		void* ResourceBuf;

		ResourceVHNatives::FILE_HANDLE FileHandle;

    public:
		ResourceMappedVH(const std::string& filename, size_t position) : 
			ResourceName(filename), 
			ResourcePos((void*)position),
			ResourceBuf(nullptr) { 

			ResourceVHNatives::CreateFileHandle(FileHandle, filename);
			ResourceVHNatives::CreateMappedFileView(&ResourceBuf, ResourcePos, FileHandle);
		}
		~ResourceMappedVH() {
			ResourceVHNatives::DeleteMappedFileView(&ResourceBuf);
			ResourceVHNatives::DeleteFileHandle(FileHandle);
		}

		Ty* get() override { return ResourceBuf; }
    };

	template <class Ty>
	class ResourceVHScope {
		Ty* VVH;
		ResourceVH<Ty>* RVH;

	public:
		void Clear() {
            if (isInitialized()) {
                delete RVH;
                VVH = nullptr;
            }
		}

		bool TryCreateAsSecureVH() {
			if (!isInitialized()) {
				RVH = new ResourceSecureVH<Ty>();
				VVH = RVH->get();
				return true;
			}
			return false;
		}

		bool TryCreateAsMappedVH(const std::string& filename, size_t position) {
			if (!isInitialized()) {
				RVH = new ResourceMappedVH<Ty>(filename, position);
				VVH = RVH->get();
				return true;
			}
			return false;
		}

		bool isInitialized() {
            return VVH != nullptr;
		}

		// Used to handling value nativly.
		Ty* operator  ->() { return  VVH; }
		Ty* operator Ty*() { return  VVH; }
		Ty& operator   &() { return *VVH; }
		Ty& operator Ty&() { return *VVH; }
	};
}


#endif // end define COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER