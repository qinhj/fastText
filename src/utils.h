/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// ===============================================================
// History:
// 2020/11/03   reviewed by qinhj@lsec.cc.ac.cn
// ---------------------------------------------------------------
// Note:
// 1) To use lambda function and std::chrono, we need: -std=c++11.
// ===============================================================

#pragma once

/* user includes */
#include "real.h"
/* c++ includes */
#include <algorithm>// for: std::find_if
#include <chrono>   // for: std::chrono
#include <fstream>  // for: std::ifstream
#include <ostream>  // for: std::ostream
#include <vector>   // for: std::vector

#if defined(__clang__) || defined(__GNUC__)
#define FASTTEXT_DEPRECATED(msg) __attribute__((__deprecated__(msg)))
#elif defined(_MSC_VER)
#define FASTTEXT_DEPRECATED(msg) __declspec(deprecated(msg))
#else   /* unknown */
#define FASTTEXT_DEPRECATED(msg)
#endif  /* compiler */

namespace fasttext {

    using Predictions = std::vector<std::pair<real, int32_t>>;

    namespace utils {

        // @brief:  get input file stream size
        int64_t size(std::ifstream&);

        // @brief:  set stream to input position
        void seek(std::ifstream&, int64_t);

        // @brief:  check if input value of type T is contained or not
        template <typename T>
        bool contains(const std::vector<T>& container, const T& value) {
            // ========================================================
            // Note: bool operator==(const T &rhs) const; is required
            // ========================================================
            return std::find(container.begin(), container.end(), value) !=
                container.end();
        }
        // @brief:  check if input value of type T2 is contained or not
        template <typename T1, typename T2>
        bool containsSecond(
            const std::vector<std::pair<T1, T2>>& container,
            const T2& value) {
            return std::find_if(
                container.begin(),
                container.end(),
                // ====================================================
                // Note: lambda function with input reference of value,
                // and parameter "const std::pair<T1, T2>& item".
                // ====================================================
                [&value](const std::pair<T1, T2>& item)
            {
                return item.second == value;
            }) != container.end();
        }

        // @brief:  get duration(seconds) between input time points
        // @example:
        //  using namespace std::chrono;
        //  steady_clock::time_point t1 = steady_clock::now();
        //  for (int i = 0; i<1000; ++i) std::cout << "*";
        //  steady_clock::time_point t2 = steady_clock::now();
        //  std::cout << "It took me " << getDuration(t1, t2) << " seconds.";
        double getDuration(
            // ========================================================
            // Note: For more details, one can visit the following url
            // http://www.cplusplus.com/reference/chrono/steady_clock
            // ========================================================
            const std::chrono::steady_clock::time_point& start,
            const std::chrono::steady_clock::time_point& end);

        class ClockPrint {
        public:
            explicit ClockPrint(int32_t duration);
            friend std::ostream& operator<<(std::ostream& out, const ClockPrint& me);

        private:
            int32_t duration_;
        };

        bool compareFirstLess(const std::pair<double, double>& l, const double& r);

    } // namespace utils

} // namespace fasttext
