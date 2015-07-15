#ifndef BASICIMAGEALGO_HPP
#define BASICIMAGEALGO_HPP

#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

#include <opencv2/core/core.hpp>

namespace ocv
{

/**
 *@brief easy function for compare_channels, user should make sure T is the correct
 *channel type of src1.
 */
template<typename T, typename UnaryFunc>
typename std::enable_if<!std::is_same<UnaryFunc, cv::Mat>::value, bool>::type
compare_channels(cv::Mat const &src, UnaryFunc func)
{        
    int rows = src.rows;
    int cols = src.cols * src.channels();

    if(src.isContinuous()){
        rows = 1;
        cols = src.total() * src.channels();
    }

    for(int row = 0; row != rows; ++row){
        T const *src_ptr = src.ptr<T>(row);
        for(int col = 0; col != cols; ++col){
            if(!func(src_ptr[col])){
                return false;
            }            
        }
    }

    return true;
}

/**
 *@brief : easy function for compare_channels, user should make sure T is the correct
 *channel type of src1 and src2
 */
template<typename T, typename BiFunc = std::equal_to<T> >
bool compare_channels(cv::Mat const &src1, cv::Mat const &src2, BiFunc func = std::equal_to<T>())
{    
    if(src1.rows != src2.rows || src1.cols != src2.cols || src1.type() != src2.type()){
        return false;
    }

    if(src1.isContinuous() && src2.isContinuous()){
        return std::equal(src1.ptr<T>(0), src1.ptr<T>(0) + src1.total() * src1.channels(), src2.ptr<T>(0), func);
    }

    int const rows = src1.rows;
    int const pixels_per_row = src1.cols * src1.channels();
    for(int row = 0; row != rows; ++row){
        T const *src1_ptr = src1.ptr<T>(row);
        T const *src2_ptr = src2.ptr<T>(row);
        for(int col = 0; col != pixels_per_row; ++col){
            if(!func(src1_ptr[col], src2_ptr[col])){
                return false;
            }            
        }
    }

    return true;
}

/**
 * @brief: copy src to dst if their rows, cols or type are different
 */
inline 
void copy_if_not_same(cv::Mat const &src, cv::Mat &dst)
{
    if(src.data != dst.data){
        src.copyTo(dst);
    }
}

inline 
void create_mat(cv::Mat const &src, cv::Mat &dst)
{
    dst.create(src.rows, src.cols, src.type());
}

/**
 * @brief : experimental version for cv::Mat, try to alleviate the problem
 * of code bloat.User should make sure the space of begin point to
 * have enough of spaces.
 */
template<typename T, typename InputIter>
void copy_to_one_dim_array(cv::Mat const &src, InputIter begin)
{       
    if(src.isContinuous()){
        auto ptr = src.ptr<T>(0);
        std::copy(ptr, ptr + src.total() * src.channels(), begin);
        return;
    }

    size_t const pixel_per_row = src.cols * src.channels();
    for(int row = 0; row != src.rows; ++row){
        auto ptr = src.ptr<T>(row);
        std::copy(ptr, ptr + pixel_per_row, begin);
        begin += pixel_per_row;
    }
}

template<typename T>
std::vector<T> copy_to_one_dim_array(cv::Mat const &src)
{
    std::vector<T> result(src.total() * src.channels());
    copy_to_one_dim_array<T>(src, std::begin(result));

    return result;
}

/**
 * @brief experimental version for cv::Mat, try to alleviate the problem
 * of code bloat.User should make sure the space of begin point to
 * have enough of spaces.
 */
template<typename T, typename InputIter>
void copy_to_one_dim_array_ch(cv::Mat const &src, InputIter begin, int channel)
{
    int const channel_number = src.channels();
    if(channel_number <= channel || channel < 0){
        throw std::runtime_error("channel value is invalid\n" + std::string(__FUNCTION__) +
                                 "\n" + std::string(__FILE__));
    }

    for(int row = 0; row != src.rows; ++row){
        auto ptr = src.ptr<T>(row) + channel;
        for(int col = 0; col != src.cols; ++col){
            *begin = *ptr;
            ++begin;
            ptr += channel_number;
        }
    }
}

template<typename T>
std::vector<T> copy_to_one_dim_array_ch(cv::Mat const &src, int channel)
{
    std::vector<T> result(src.total());
    copy_to_one_dim_array_ch<T>(src, std::begin(result), channel);

    return result;
}

}

#endif // BASIC_HPP