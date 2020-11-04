/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

 // ===============================================================
 // History:
 // 2020/11/04   reviewed by qinhj@lsec.cc.ac.cn
 // ---------------------------------------------------------------
 // Note:
 // 1) To use "&&", we need vs2015 or higher;
 // ===============================================================

#pragma once
/* c++ includes */
#include <cstdint>  // for: int64_t
#include <ostream>  // for: std::ostream
#include <vector>   // for: std::vector
/* user includes */
#include "real.h"   // for: fasttext::real
#include "matrix.h" // for: fasttext::Matrix

namespace fasttext {

    class Matrix;

    class Vector {
    protected:
        std::vector<real> data_;

    public:
        // @brief:  constructor with capacity
        explicit Vector(int64_t);
        Vector(const Vector&) = default;
        Vector(Vector&&) noexcept = default;
        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        inline real* data() {
            return data_.data();
        }
        inline const real* data() const {
            return data_.data();
        }
        inline real& operator[](int64_t i) {
            return data_[i];
        }
        inline const real& operator[](int64_t i) const {
            return data_[i];
        }

        inline int64_t size() const {
            return data_.size();
        }
 
        // @brief:  data_ = 0.0
        void zero();
        // @brief:  data_ *= a
        void mul(real);
        // @brief:  l^2
        real norm() const;
        // @brief:  data_ += source
        void addVector(const Vector& source);
        // @brief:  data_ += s * source
        void addVector(const Vector&, real);
        void addRow(const Matrix&, int64_t);
        void addRow(const Matrix&, int64_t, real);
        // @brief:  data_ = Mat * Vec
        void mul(const Matrix&, const Vector&);
        // @brief:  idx of max element
        int64_t argmax();
    };

    std::ostream& operator<<(std::ostream&, const Vector&);

} // namespace fasttext
