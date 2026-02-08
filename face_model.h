#ifndef FACE_MODEL_H
#define FACE_MODEL_H

#include <dlib/dnn.h>

using namespace dlib;

// ----------------------------------------------------------------------------------------
// THE "OFFICIAL" ARCHITECTURE (Matches your file hash 55533...)
// ----------------------------------------------------------------------------------------

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N,BN,1,tag1<SUBNET>>>;

template <template <int,template<typename>class,int,typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2,2,2,2,skip1<tag2<block<N,BN,2,tag1<SUBNET>>>>>>;

template <int N, template<typename>class BN, int stride, typename SUBNET> 
using block  = BN<con<N,3,3,1,1,relu<BN<con<N,3,3,stride,stride,SUBNET>>>>>;

template <int N, typename SUBNET> using ares      = relu<residual<block,N,affine,SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block,N,affine,SUBNET>>;

// The standard v1 model level definitions
template <typename SUBNET> using alevel0 = ares_down<256,SUBNET>;
template <typename SUBNET> using alevel1 = ares<256,ares<256,ares_down<256,SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128,ares<128,ares_down<128,SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64,ares<64,ares<64,ares_down<64,SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32,ares<32,ares<32,SUBNET>>>;

// ----------------------------------------------------------------------------------------
// THE INPUT: pyramid_down<6> (Matches Standard V1 Model)
// ----------------------------------------------------------------------------------------

using net_input = max_pool<3,3,2,2,relu<affine<con<32,7,7,2,2,input_rgb_image_sized<150>>>>>;

// ----------------------------------------------------------------------------------------
// DECOMPOSED STACK (No brackets error)
// ----------------------------------------------------------------------------------------

using net_L4 = alevel4<net_input>;
using net_L3 = alevel3<net_L4>;
using net_L2 = alevel2<net_L3>;
using net_L1 = alevel1<net_L2>;
using net_L0 = alevel0<net_L1>;

using net_output = fc_no_bias<128,avg_pool_everything<net_L0>>;

using anet_type = loss_metric<net_output>;

#endif // FACE_MODEL_H
