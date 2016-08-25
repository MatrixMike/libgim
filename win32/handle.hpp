/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#include <windows.h>

namespace util { namespace win32 {
    struct handle {
        handle ();
        explicit handle (HANDLE&&);
        handle (handle&&);
        handle (const handle&) = delete;
        ~handle ();

        operator HANDLE& (void) &; 
        HANDLE& native (void) &;
        const HANDLE& native (void) const &;

        void reset (HANDLE);
        void reset (handle&&);

        static handle current_process (void);

    private:
        HANDLE m_native;
    };
} }
