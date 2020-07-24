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
 */

#ifndef AUDIO_TACTILE_SDL_WINDOW_ICON_H_
#define AUDIO_TACTILE_SDL_WINDOW_ICON_H_

#include "SDL2/SDL.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Sets a nice window icon on an SDL window. The default icon is otherwise an
 * unfortunate "canceled" icon, making the application appear broken.
 */
void SetWindowIcon(SDL_Window* window);

#ifdef __cplusplus
}  /* extern "C" */
#endif
#endif /* AUDIO_TACTILE_SDL_WINDOW_ICON_H_ */
