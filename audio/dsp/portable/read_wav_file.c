/* Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * For details on the WAV file format, see for instance
 * http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html.
 */

#include "audio/dsp/portable/read_wav_file.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "audio/dsp/portable/logging.h"

static size_t ReadBytes(void* bytes, size_t num_bytes, void* io_ptr) {
  return fread(bytes, 1, num_bytes, (FILE*)io_ptr);
}

static int Seek(size_t num_bytes, void* io_ptr) {
  return fseek((FILE*)io_ptr, num_bytes, SEEK_CUR);
}

static int EndOfFile(void* io_ptr) {
  return feof((FILE*)io_ptr);
}

static WavReader WavReaderLocal(FILE* f) {
  WavReader w;
  w.read_fun = ReadBytes;
  w.seek_fun = Seek;
  w.eof_fun = EndOfFile;
  w.custom_chunk_fun = NULL;
  w.io_ptr = f;
  return w;
}

int ReadWavHeader(FILE* f, ReadWavInfo* info) {
  WavReader w = WavReaderLocal(f);
  return ReadWavHeaderGeneric(&w, info);
}

size_t Read16BitWavSamples(FILE* f, ReadWavInfo* info,
                           int16_t* samples, size_t num_samples) {
  WavReader w = WavReaderLocal(f);
  return Read16BitWavSamplesGeneric(&w, info, samples, num_samples);
}

size_t ReadWavSamples(FILE* f, ReadWavInfo* info,
                      int32_t* samples, size_t num_samples) {
  WavReader w = WavReaderLocal(f);
  return ReadWavSamplesGeneric(&w, info, samples, num_samples);
}

int16_t* Read16BitWavFile(const char* file_name, size_t* num_samples,
                          int* num_channels, int* sample_rate_hz) {
  if (file_name == NULL || num_samples == NULL ||
      num_channels == NULL || sample_rate_hz == NULL) {
    return NULL;
  }

  FILE* f = fopen(file_name, "rb");
  WavReader w = WavReaderLocal(f);
  int16_t* samples = NULL;
  ReadWavInfo info;

  if (!f) {
    LOG_ERROR("Error: Failed to open \"%s\" for reading.\n", file_name);
    return NULL;
  }
  if (!ReadWavHeaderGeneric(&w, &info)) {
    goto fail;
  }

  *num_channels = info.num_channels;
  *sample_rate_hz = info.sample_rate_hz;

  samples = (int16_t*) malloc(info.remaining_samples * sizeof(int16_t));
  if (samples == NULL) {
    LOG_ERROR("Error: Allocation of %zu bytes for WAV samples failed.\n",
              sizeof(int16_t) * info.remaining_samples);
    goto fail;
  }
  *num_samples = Read16BitWavSamplesGeneric(&w, &info, samples,
                                            info.remaining_samples);
  if (fclose(f)) { goto fail; }
  return samples;

fail:  /* Clean up on failure. */
  if (samples != NULL) { free(samples); }
  *num_samples = 0;
  *num_channels = 0;
  *sample_rate_hz = 0;
  fclose(f);
  w.has_error = 1;  /* Set the error flag, just in case the caller checks. */
  return NULL;
}

static void InPlaceFloatToInt32Conversion(size_t num_elements, void* input) {
  CHECK(sizeof(int32_t) == 4);
  CHECK(sizeof(float) == 4);

  const float kLowest = INT32_MIN;
  const float kMax = INT32_MAX;
  const float normalizer = -kLowest;
  unsigned char* bytes = input;
  int i;
  for (i = 0; i < num_elements; ++i, bytes += 4) {
    float input_float = 0;
    memcpy(&input_float, bytes, 4);
    float normalized = input_float * normalizer;
    float clipped = normalized > kMax ? kMax : normalized;
    clipped = clipped < kLowest ? kLowest : clipped;
    if (clipped != clipped /* isnan(clipped) */) { clipped = 0; }
    int32_t input_int = (int32_t)clipped;
    memcpy(bytes, &input_int, 4);
  }
}

int32_t* ReadWavFile(const char* file_name, size_t* num_samples,
                     int* num_channels, int* sample_rate_hz) {
  if (file_name == NULL || num_samples == NULL ||
      num_channels == NULL || sample_rate_hz == NULL) {
    return NULL;
  }

  FILE* f = fopen(file_name, "rb");
  WavReader w = WavReaderLocal(f);
  int32_t* samples = NULL;
  ReadWavInfo info;

  if (!f) {
    LOG_ERROR("Error: Failed to open \"%s\" for reading.\n", file_name);
    return NULL;
  }
  if (!ReadWavHeaderGeneric(&w, &info)) {
    goto fail;
  }

  *num_channels = info.num_channels;
  *sample_rate_hz = info.sample_rate_hz;

  if (info.sample_format == kInt16) {
    /* Upgrade from 16-bit samples to 32-bit samples. */
    info.destination_alignment_bytes = 4;
    info.sample_format = kInt32;
  }
  /* This allocation is fine even if the sample type is float because the size
   * of a float is almost always 32 bits. */
  CHECK(sizeof(float) == 4);  /* Assumes IEEE 754. */
  samples = (int32_t*) malloc(info.remaining_samples * sizeof(int32_t));

  if (samples == NULL) {
    LOG_ERROR("Error: Allocation of %zu bytes for WAV samples failed.\n",
            sizeof(int) * info.remaining_samples);
    goto fail;
  }

  *num_samples = ReadWavSamplesGeneric(&w, &info, samples,
                                       info.remaining_samples);

  /* Do an in-place conversion to int32 when sample type is float. */
  if (info.sample_format == kFloat) {
    InPlaceFloatToInt32Conversion(*num_samples, samples);
  }

  if (fclose(f)) { goto fail; }
  return samples;

fail:  /* Clean up on failure. */
  if (samples != NULL) { free(samples); }
  *num_samples = 0;
  *num_channels = 0;
  *sample_rate_hz = 0;
  fclose(f);
  w.has_error = 1;  /* Set the error flag, just in case the caller checks. */
  return NULL;
}
