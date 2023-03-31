// Copyright (c) 2023 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#include "iox/file_name.hpp"

namespace iox
{
namespace details
{
bool file_name_does_contain_invalid_characters(const string<platform::IOX_MAX_FILENAME_LENGTH>& value) noexcept
{
    const auto valueSize = value.size();

    for (uint64_t i{0}; i < valueSize; ++i)
    {
        // AXIVION Next Construct AutosarC++19_03-A3.9.1: Not used as an integer but as actual character
        const char c{value[i]};

        const bool isSmallLetter{'a' <= c && c <= 'z'};
        const bool isCapitalLetter{'A' <= c && c <= 'Z'};
        const bool isNumber{'0' <= c && c <= '9'};
        const bool isSpecialCharacter{c == '-' || c == '.' || c == ':' || c == '_'};

        if ((!isSmallLetter && !isCapitalLetter) && (!isNumber && !isSpecialCharacter))
        {
            return true;
        }
    }

    return false;
}

bool file_name_does_contain_invalid_content(const string<platform::IOX_MAX_FILENAME_LENGTH>& value) noexcept
{
    return (value.empty() || value == "." || value == "..");
}
} // namespace details
} // namespace iox
