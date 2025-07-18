/**
 * MegaMol
 * Copyright (c) 2020, MegaMol Dev Team
 * All rights reserved.
 */

#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <png.h>

#include "RuntimeInfo.h"

namespace megamol::core::utility::graphics {

class ScreenShotComments {
public:
    typedef std::map<std::string, std::string> comments_storage_map;
    typedef std::vector<png_text> png_comments;

    /**
     * Instantiate this class with a number of pre-defined comments. You can pass additional keys/values to the
     * constructor and later get out the png_text you need for feeding libpng. Note that the returned png_text array
     * is only valid as long as the ScreenShotComments instance is in scope!
     */
    ScreenShotComments(std::string const& project_configuration, megamol::frontend_resources::RuntimeInfo const* ri,
        const std::optional<comments_storage_map>& additional_comments = std::nullopt);

    png_comments GetComments() const;

    /**
     * Returns the project lua contained in the exif data of a PNG file.
     *
     * @param filename the png file name
     *
     * @return the lua project
     */
    static std::string GetProjectFromPNG(std::filesystem::path filename);

private:
    comments_storage_map the_comments;
    png_comments the_vector;
};

} // namespace megamol::core::utility::graphics
