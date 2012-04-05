#include "brender.h"
#include <QGLWidget>
#include <math.h>
#include <assert.h>

BRender::BRender()
{
}

void BRender::SetScreenSize(int width, int height) {
    this->width = width;
    this->height = height;
}

void BRender::Render(Image *image) {
    float ratio =  GetRatio(image);
    ratio = floor(ratio*100)/100;
    qDebug("%f", ratio);
    if (ratio < 1.0) {
        Image * renderImage = AllocateRenderBuffer(image, ratio);
        Scale scaler;
        scaler.ScaleRect(image, renderImage, ratio);
        RenderImage(renderImage);
        delete renderImage;
    } else {
        RenderImage(image);
    }
}

std::pair<float, float> BRender::GetCenterPosition(Image * image) {
    int height = image->GetHeight();
    int width = image->GetWidth();
    float pos_x = (this->width - width) / 2.0;
    float pos_y = this->height - ((this->height - height) / 2.0);
    return std::pair<float, float>(pos_x, pos_y);
}

void BRender::RenderImage(Image *image) {
    int height = image->GetHeight();
    int width = image->GetWidth();
    int format = image->GetPixelFormat();
    std::shared_ptr<char> raw = image->ImageData.GetAllocatedMemory();
    std::pair<float, float> centerPosition = GetCenterPosition(image);
    glRasterPos2i(centerPosition.first, centerPosition.second);
    glDrawPixels(width, height, format, GL_UNSIGNED_BYTE, raw.get());
}

float BRender::GetRatio(Image *image) {
    int i_width = image->GetWidth();
    int i_height = image->GetHeight();
    float ratio = std::min<float>((float) width / i_width, (float) height / i_height);
    return ratio;
}

Image * BRender::AllocateRenderBuffer(Image * image, float ratio) {
    ImageInfo imageInfo = GetNewImageInfo(image, ratio);
    Image * renderImage = new Image(0);
    renderImage->SetImageInfo(imageInfo);
    renderImage->AllocateMemmory();
    return renderImage;
}

ImageInfo BRender::GetNewImageInfo(Image *image, float ratio) {
    ImageInfo imageInfo = image->imageInfo;
    // Bitmapa ma zarovnani radky na nasobky 4 (do 32 bit intu)
    imageInfo.width = ((int) floor(imageInfo.width * ratio) / 4) * 4;
    imageInfo.height = floor(imageInfo.height * ratio);
    imageInfo.imageSize = (imageInfo.width) * (imageInfo.height ) * 3;
    return imageInfo;
}



int round(float num) {
    return (int) floor(num + 0.5);
}
// pAda, nekde spatne pocitam
void Scale::ScaleLine(BGR *src, BGR *dest, int srcWidth, int destWidth) {
   float ratio = srcWidth / (float) destWidth;
   for (int i = 0; i < destWidth; ++i) {
       int colIndex = round(i * ratio);
       dest[i] = src[colIndex];
   }
}


void Scale::ScaleRect(Image *input, Image *output, float ratio2) {
    std::shared_ptr<char> srcMemory = input->ImageData.GetAllocatedMemory();
    BGR * src = reinterpret_cast<BGR *>(srcMemory.get());

    std::shared_ptr<char> destMemory = output->ImageData.GetAllocatedMemory();
    BGR * dest = reinterpret_cast<BGR *>(destMemory.get());

    int srcWidth = input->GetWidth();
    int srcHeight = input->GetHeight();
    int destWidth = output->GetWidth();
    int destHeight = output->GetHeight();

    float ratio = srcHeight / (float) destHeight;

    for (int i = 0; i < destHeight; ++i) {
        int srcRowIndex = round(i*ratio)*srcWidth;
        ScaleLine(&src[srcRowIndex], &dest[i*destWidth], srcWidth, destWidth);
    }
}

