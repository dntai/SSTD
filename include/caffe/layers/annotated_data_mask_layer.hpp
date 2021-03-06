#ifndef CAFFE_DATA_LAYER_HPP_
#define CAFFE_DATA_LAYER_HPP_

#include <boost/thread.hpp>                                                                
#include <string>
#include <vector>

#include "caffe/blob.hpp"
#include "caffe/data_reader.hpp"
#include "caffe/data_transformer.hpp"
#include "caffe/internal_thread.hpp"
#include "caffe/layer.hpp"
#include "caffe/layers/base_data_layer.hpp"
#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include "caffe/util/blocking_queue.hpp"                                          
namespace caffe {

template <typename Dtype>
class AnnotatedDataMaskLayer : public BaseDataMaskLayer<Dtype>, public InternalThread{
 public:
  explicit AnnotatedDataMaskLayer(const LayerParameter& param);
  virtual ~AnnotatedDataMaskLayer();
  void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void DataLayerSetUp(const vector<Blob<Dtype>*>& bottom,
          const vector<Blob<Dtype>*>& top);
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  // AnnotatedDataMaskLayer uses DataReader instead for sharing for parallelism
  virtual inline bool ShareInParallel() const { return false; }
  virtual inline const char* type() const { return "AnnotatedDataMask"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }
  static const int PREFETCH_COUNT = 3;
 protected:
  virtual void InternalThreadEntry();
  virtual void load_batch(BatchMask<Dtype>* batch);

  BatchMask<Dtype> prefetch_[PREFETCH_COUNT];
  BlockingQueue<BatchMask<Dtype>*> prefetch_free_;
  BlockingQueue<BatchMask<Dtype>*> prefetch_full_;


  Blob<Dtype> transformed_data_;
  Blob<Dtype> transformed_mask_;
  DataReader<AnnotatedMaskDatum> reader_;
  bool has_anno_type_;
  AnnotatedMaskDatum_AnnotationType anno_type_;
  vector<BatchSampler> batch_samplers_;
  string label_map_file_;
  bool oriented_;

};

}  // namespace caffe

#endif  // CAFFE_DATA_LAYER_HPP_
