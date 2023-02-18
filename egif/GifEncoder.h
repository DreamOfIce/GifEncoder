//
// Created by xiaozhuai on 2020/12/20.
//

#ifndef GIF_GIFENCODER_H
#define GIF_GIFENCODER_H

#include <string>
#include <vector>

class GifEncoder {
public:
    enum PixelFormat {
        PIXEL_FORMAT_UNKNOWN = 0,
        PIXEL_FORMAT_BGR = 1,
        PIXEL_FORMAT_RGB = 2,
        PIXEL_FORMAT_BGRA = 3,
        PIXEL_FORMAT_RGBA = 4,
    };
public:
    GifEncoder() = default;

    /**
     * create gif file
     *
     * @param[in] file file path
     * @param[in] width gif width
     * @param[in] height gif height
     * @param[in] quality 1..30, 1 is best
     * @param[in] useGlobalColorMap
     * @param[in] loop loop count, 0 is endless
     * @param[in] preAllocSize pre-allocated buffer size
     * @param[in] noloop set to true if you don't want to loop
     * @note If you give the loop parameter a value greater than zero, then the animation will stop after n+1 plays.
     *        If you want the animation to play only once, set the noloop parameter to true
     * @note For better performance, it's suggested to set preAllocSize. If you can't determine it, set to 0.
     *        If use global color map, all frames size must be same, and preAllocSize = width * height * 3 * nFrame
     *        If use local color map, preAllocSize = MAX(width * height) * 3
     * @return
     */
    bool open(const std::string &file, int width, int height,
              int quality, bool useGlobalColorMap, uint16_t loop, int preAllocSize = 0, bool noloop = false);

    /**
     * create a gif builder
     *
     * @param[out] destPtr pointer to the result
     * @param[out] destLength length of the result
     * @param[in] width gif width
     * @param[in] height gif height
     * @param[in] quality 1..30, 1 is best
     * @param[in] useGlobalColorMap
     * @param[in] loop loop count, 0 is endless
     * @param[in] preAllocSize pre-allocated buffer size
     * @param[in] noloop set to true if you don't want to loop
     * @note If you give the loop parameter a value greater than zero, then the animation will stop after n+1 plays.
     *        If you want the animation to play only once, set the noloop parameter to true
     * @note For better performance, it's suggested to set preAllocSize. If you can't determine it, set to 0.
     *        If use global color map, all frames size must be same, and preAllocSize = width * height * 3 * nFrame
     *        If use local color map, preAllocSize = MAX(width * height) * 3
     * @return
     */
    bool open(uint8_t **destPtr, size_t *destLength, int width, int height, int quality, bool useGlobalColorMap, uint16_t loop, int preAllocSize, bool noloop = false);

    /**
     * add frame
     *
     * @param format pixel format
     * @param frame frame data
     * @param width frame width
     * @param height frame height
     * @param delay delay time 0.01s
     * @return
     */
    bool push(PixelFormat format, const uint8_t *frame, int width, int height, int delay);

    /**
     * close gif file
     *
     * @return
     */
    bool close();

private:
    inline bool isFirstFrame() const {
        return m_frameCount == 0;
    }

    inline void reset() {
        if (m_framePixels != nullptr) {
            free(m_framePixels);
            m_framePixels = nullptr;
        }
        m_allocSize = 0;
        std::vector<int>().swap(m_allFrameDelays);
        std::vector<uint8_t>().swap(m_result);
        m_frameCount = 0;
        m_frameWidth = -1;
        m_frameHeight = -1;
    }

    void encodeFrame(int width, int height, int delay, void *colorMap, void *rasterBits);

private:
    void *m_gifFileHandler = nullptr;

    int m_quality = 10;
    bool m_useGlobalColorMap = false;

    uint8_t *m_framePixels = nullptr;
    int m_allocSize = 0;
    std::vector<int> m_allFrameDelays{};
    int m_frameCount = 0;
    int m_frameWidth = -1;
    int m_frameHeight = -1;

    uint8_t **m_destPtr = nullptr;
    size_t *m_destLength = nullptr;
    std::vector<uint8_t> m_result{};
};


#endif //GIF_GIFENCODER_H
