#ifndef BLOCK_BINARY_SUM_H
#define BLOCK_BINARY_SUM_H

#include <opencv2/core.hpp>

#include <vector>

/*!
 *  \addtogroup ocv
 *  @{
 */
namespace ocv{

/**
 * Implementation of block binary pixel sum
 *
 */
class block_binary_pixel_sum
{
public:
    block_binary_pixel_sum(cv::Size const &target_size,
                           cv::Size const &block_size);

    std::vector<double> const& describe(cv::Mat const &input);

private:
    cv::Size block_size_;
    std::vector<double> features_;
    cv::Mat gray_mat_;
    cv::Mat resize_mat_;
    cv::Size target_size_;
};

} /*! @} End of Doxygen Groups*/

#endif // BLOCK_BINARY_SUM_H
