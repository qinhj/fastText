/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// 2020/11/03   reviewed by qinhj@lsec.cc.ac.cn

#include "utils.h"

#include <iomanip>  // for: std::setw
#include <ios>      // for: ios::end

namespace fasttext {

    namespace utils {

        // @brief:  get input file stream size
        int64_t size(std::ifstream& ifs) {
            // =========================================
            // Note:
            //  streampos: absolute position of stream;
            //  streamoff: relative offset of stream;
            // =========================================
            ifs.seekg(std::streamoff(0), std::ios::end);
            // return the distance between start and end posistions of stream
            return ifs.tellg();
        }

        // @brief:  set stream to input position
        void seek(std::ifstream& ifs, int64_t pos) {
            ifs.clear();
            ifs.seekg(std::streampos(pos));
        }

        // @brief:  get duration(seconds) between input time points
        double getDuration(
            const std::chrono::steady_clock::time_point& start,
            const std::chrono::steady_clock::time_point& end) {
            return std::chrono::duration_cast<std::chrono::duration<double>>(end - start)
                .count();
        }

        ClockPrint::ClockPrint(int32_t duration) : duration_(duration) {
        }

        // operator overload: "<<"
        std::ostream& operator<<(std::ostream& out, const ClockPrint& me) {
            int32_t etah = me.duration_ / 3600;
            int32_t etam = (me.duration_ % 3600) / 60;
            int32_t etas = (me.duration_ % 3600) % 60;

            out << std::setw(3) << etah << "h" << std::setw(2) << etam << "m";
            out << std::setw(2) << etas << "s";
            return out;
        }

        bool compareFirstLess(const std::pair<double, double>& l, const double& r) {
            return l.first < r;
        }

    } // namespace utils

} // namespace fasttext
