#ifndef CAFFE_CUDNN_LRN_LAYER_HPP_
#define CAFFE_CUDNN_LRN_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

#include "caffe/layers/lrn_layer.hpp"

namespace caffe {

#ifdef USE_ACCMI
template <typename Dtype>
class CuDNNLRNLayer : public LRNLayer<Dtype> {
 public:
  explicit CuDNNLRNLayer(const LayerParameter& param)
      : LRNLayer<Dtype>(param), handles_setup_(false)
#ifdef USE_MIOPEN
        , workspaceSize(0), workspace(NULL)
#endif
      {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual ~CuDNNLRNLayer();

 protected:
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);


  bool handles_setup_;

#ifdef USE_MIOPEN
  miopenHandle_t             handle_;
  miopenLRNDescriptor_t norm_desc_;
  miopenTensorDescriptor_t bottom_desc_, top_desc_;

  size_t workspaceSize;
  void* workspace;
#endif

#ifdef USE_CUDNN
  cudnnHandle_t             handle_;
  cudnnLRNDescriptor_t norm_desc_;
  cudnnTensorDescriptor_t bottom_desc_, top_desc_;
#endif

  int size_;
  Dtype alpha_, beta_, k_;
};
#endif

}  // namespace caffe

#endif  // CAFFE_CUDNN_LRN_LAYER_HPP_
