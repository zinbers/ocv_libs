#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP

#include <Eigen/Dense>

#include <opencv2/core.hpp>

/*! \file activation.hpp
    \brief collection of various activation algorithm
*/

/*!
 *  \addtogroup ocv
 *  @{
 */
namespace ocv{

/*!
 *  \addtogroup ml
 *  @{
 */
namespace ml{

struct dsigmoid
{
    void operator()(cv::Mat &inout) const
    {
        cv::multiply(1.0 - inout, inout, inout);
    }

    void operator()(cv::Mat const &input,
                    cv::Mat &output) const
    {
        cv::multiply(1.0 - input, input, output);
    }
};

struct sigmoid
{
    void operator()(cv::Mat &inout) const
    {
        operator()(inout, inout);
    }

    void operator()(cv::Mat const &input,
                    cv::Mat &output) const;

    template<typename Derived>
    void operator()(Eigen::MatrixBase<Derived> &inout) const
    {
        inout = 1.0 / (1.0 + (-1.0 * inout.array()).exp());
    }
};

} /*! @} End of Doxygen Groups*/

} /*! @} End of Doxygen Groups*/


#endif // ACTIVATION_HPP

