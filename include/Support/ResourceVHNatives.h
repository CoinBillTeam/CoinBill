#ifndef COINBILL_SUPPORT_RESOURCE_VH_NATIVES
#define COINBILL_SUPPORT_RESOURCE_VH_NATIVES

#include <string>

namespace CoinBill {
    namespace ResourceVHNatives {
		typedef void* __NATIVE_HANDLE;
		typedef __NATIVE_HANDLE FILE_HANDLE;
		typedef __NATIVE_HANDLE VIEW_HANDLE;

		bool CreateFileHandle(FILE_HANDLE& fHandle, const std::string& filename);
		bool DeleteFileHandle(FILE_HANDLE& fHandle);
		bool CreateMappedFileView(void **pView, void* pOffset, FILE_HANDLE& fHandle);
		bool DeleteMappedFileView(void **pView);
    }
}

#endif