#ifndef COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER
#define COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER

#include <Support/BasicUtility.h>
#include <Support/ResourceVHNatives.h>

#include <string>
#include <fstream>
#include <bitset>

namespace CoinBill {
    // The basic resource value handler type.
    template <class Ty>
    class ResourceVH {
    protected:
        Ty* pResource;

    public:
        virtual ~ResourceVH() = default;

        // Default operator overrides for resource handler.
        // recommend not overriding this overrides.
        // you can basically accessing it using opeartors.
        virtual Ty* operator->() { return pResource;  }
        virtual Ty& operator* () { return *pResource; }
    };

    template <class Ty, unsigned int Index = 1>
    class ResourceMappedVH : public ResourceVH<Ty> {
        ResourceVHNatives::FILE_HANDLE fileHandle;
        unsigned int ViewGlobalOffset;

        bool isSizeSmallerThanPage;
        bool isReMappingNeedsForBigSize;

        bool isCurrentViewOnFirst;
        unsigned int currentViewIndex;
        unsigned int currentViewSize;
       

        bool createNewView(unsigned int Offset, unsigned int Size) {
            if(&ResourceVH<Ty>::pResource != nullptr)
                ResourceVHNatives::DeleteMappedFileView(&ResourceVH<Ty>::pResource);

            return ResourceVHNatives::CreateMappedFileView(
                &ResourceVH<Ty>::pResource,             // Binding on pResource.
                Offset + ViewGlobalOffset,              // Offset that we starting from.
                Size,                                   // Size that we are going to map.
                fileHandle                              // Use file for current selected file.
            );
        }

        size_t getFileSize(const std::string fileName) {
            std::ifstream fStream(fileName, std::ifstream::ate | std::ifstream::binary);
            return fStream.tellg();
        }

    public:
        ResourceMappedVH(const std::string& fileName, size_t GlobOffset, size_t Size = 0) : 
                isSizeSmallerThanPage       (false), 
                isReMappingNeedsForBigSize  (false),
                isCurrentViewOnFirst        (false),
                currentViewIndex            (0), 
                currentViewSize             (0),
                ViewGlobalOffset(GlobOffset) {
            // We are precalutating a file size before we mapping.
            // usually, we have a big chunk of file so we will need to make sure is that mapping all is needed.
            const size_t fileSize = Size ? Size : getFileSize(fileName);
            if(fileSize <= 4096) {
                isSizeSmallerThanPage       = true;
            } else if(fileSize >= 4096 * 4) {
                isReMappingNeedsForBigSize  = true;
            }

            // Opening a file as native os handle
            // we have to make it as a os managed handle because we are opening it as mapped file.
            ResourceVHNatives::CreateFileHandle(fileHandle, fileName);
            COINBILL_ASSERT_DEBUG(fileHandle != nullptr);

            // Now we are creating a native mapped file view.
            // that means we can accessing it directly, as that we want.
            if(!createNewView(0, Size)) {
                ResourceVH<Ty>::pResource = nullptr;
            }

            COINBILL_ASSERT_DEBUG(ResourceVH<Ty>::pResource != nullptr);
        }

        virtual ~ResourceMappedVH() {
            ResourceVHNatives::DeleteMappedFileView(&ResourceVH<Ty>::pResource);
            ResourceVHNatives::DeleteFileHandle(fileHandle);
        }

        Ty* operator->() override {
            if (!isSizeSmallerThanPage) {
                if (!isCurrentViewOnFirst) {
                    currentViewIndex            = 0;
                    currentViewSize             = sizeof(Ty);
                    isCurrentViewOnFirst        = true;

                    createNewView(0, sizeof(Ty));
                }
            }

            return ResourceVH<Ty>::pResource;
        }

        Ty& operator*() override {
            return *(operator->());
        }

        Ty& operator[](unsigned int index) {
            if (!isSizeSmallerThanPage) {
                isCurrentViewOnFirst = false;

                if (currentViewIndex != index) {
                    currentViewIndex            = index;
                    currentViewSize             = sizeof(Ty);

                    createNewView(index * sizeof(Ty), sizeof(Ty));
                }
                return ResourceVH<Ty>::pResource;
            }
            return ResourceVH<Ty>::pResource[index];
        }
    };
}


#endif // end define COINBILL_SUPPORT_RESOURCE_VALUE_HANDLER