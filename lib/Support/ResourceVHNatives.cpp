#include <Support/ResourceVHNatives.h>
#include <Support/BasicUtility.h>
#include <winnt.h>
#include <memoryapi.h>


namespace CoinBill {
	namespace ResourceVHNatives {

#ifdef COINBILL_WINDOWS
#include <Windows.h>
	    bool CreateFileHandle(FILE_HANDLE& fHandle, const std::string& filename) {
            return !!(fHandle = (FILE_HANDLE)OpenFile(filename.c_str(), nullptr, OF_READWRITE));
	    }

	    bool DeleteFileHandle(FILE_HANDLE& fHandle) {
            return !!CloseHandle(fHandle);
	    }

	    bool CreateMappedFileView(void** pView, size_t pOffset, size_t pSize, FILE_HANDLE& fHandle) {
            return !!(*pView = MapViewOfFile(fHandle, FILE_MAP_ALL_ACCESS, pOffset, round_up<4096>(pSize), (size_t)pSize));
	    }

	    bool DeleteMappedFileView(void** pView) {
	    }
#endif // COINBILL_WINDOWS
	}
}
