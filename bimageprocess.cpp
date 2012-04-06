#include "bimageprocess.h"

BImageProcess::BImageProcess()
{
}

void BImageProcess::ForEach(Image *source, Image * destiny, Procesor procesor) {
    int pixels = source->ImageData.GetAllocatedDataSize() / sizeof(RGBA);
    std::shared_ptr<char> imageData = source->ImageData.GetAllocatedMemory();
    RGBA * rgbaData = reinterpret_cast<RGBA *>(imageData.get());

    std::shared_ptr<char> destinyData = destiny->ImageData.GetAllocatedMemory();
    RGBA * oRgbaData = reinterpret_cast<RGBA *>(destinyData.get());

    // Paralel
    for (int position = 0; position < pixels; ++position) {
        oRgbaData[position] = procesor(rgbaData[position]);

    }
}

void BImageProcess::ForEach(ImageRow & imageRow, ColProcesor colProcesor) {
    for (int col = 0; col < imageRow.width; ++col) {
        colProcesor(col, imageRow.rowData[col]);
    }
}

void BImageProcess::ForEachRow(Image *destiny, RowProcesor rowProcesor) {
    std::shared_ptr<char> destinyMemory = destiny->ImageData.GetAllocatedMemory();
    RGBA * destinyData = reinterpret_cast<RGBA *>(destinyMemory.get());

    for (int row = 0; row < destiny->GetHeight(); ++row) {
        RGBA * rowDataPointer = destinyData + row*(destiny->GetWidth());
        ImageRow rowData(rowDataPointer, destiny->GetWidth());
        rowProcesor(row, rowData);
    }
}
