//
// Created by Frederic on 26/03/2017.
//

#ifndef MINI_SOUND_SYSTEM_SingleThreadMediaCodecExtractor_H
#define MINI_SOUND_SYSTEM_SingleThreadMediaCodecExtractor_H

#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <media/NdkMediaCodec.h>
#include <media/NdkMediaExtractor.h>
#include <audio/SoundSystem.h>
#include <utils/time_utils.h>

/**
 * Buffer size to use with RAW (wav) file. Some devices return bigger bufSize but it doesn't work.
 * It seams that quantity of data is still the same even if bugSize change. It's always 32768...
 */
#define RAW_BUFFER_SIZE 32768

typedef struct {
    AMediaExtractor *ex;
    AMediaCodec *codec;

    bool sawInputEOS;
    bool sawOutputEOS;

    SoundSystem *soundSystem;
    uint8_t *extractedData;

    double extractionTimeStart;
    unsigned int extractionPosition;
    unsigned short numberChannel;
    bool isRawFormat;

} workerdata;

static workerdata data = {NULL, NULL, false, false, NULL, NULL, 0, 0, 0, false};

class SingleThreadMediaCodecExtractor {

public:

    SingleThreadMediaCodecExtractor(SoundSystem *soundSystem, const unsigned short frameRate);

    ~SingleThreadMediaCodecExtractor();

    bool extract(const char *filename);

private:

    unsigned int _file_total_frames;
    int32_t _file_sample_rate;
    int32_t _file_number_channels;
    int64_t _file_duration;
    const unsigned short _device_frame_rate;

    void extractMetadata(AMediaFormat *format);

    void startExtractorThread();

    static void *doExtraction(void*);
};

#endif //MINI_SOUND_SYSTEM_SingleThreadMediaCodecExtractor_H