/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef FaceCluster_H
#define FaceCluster_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "face_cluster_types.h"



#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class FaceClusterIf {
 public:
  virtual ~FaceClusterIf() {}
  virtual void Cluster(ClusterResult& _return, const ClusterInput& input) = 0;
};

class FaceClusterIfFactory {
 public:
  typedef FaceClusterIf Handler;

  virtual ~FaceClusterIfFactory() {}

  virtual FaceClusterIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(FaceClusterIf* /* handler */) = 0;
};

class FaceClusterIfSingletonFactory : virtual public FaceClusterIfFactory {
 public:
  FaceClusterIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<FaceClusterIf>& iface) : iface_(iface) {}
  virtual ~FaceClusterIfSingletonFactory() {}

  virtual FaceClusterIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(FaceClusterIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<FaceClusterIf> iface_;
};

class FaceClusterNull : virtual public FaceClusterIf {
 public:
  virtual ~FaceClusterNull() {}
  void Cluster(ClusterResult& /* _return */, const ClusterInput& /* input */) {
    return;
  }
};

typedef struct _FaceCluster_Cluster_args__isset {
  _FaceCluster_Cluster_args__isset() : input(false) {}
  bool input :1;
} _FaceCluster_Cluster_args__isset;

class FaceCluster_Cluster_args {
 public:

  FaceCluster_Cluster_args(const FaceCluster_Cluster_args&);
  FaceCluster_Cluster_args& operator=(const FaceCluster_Cluster_args&);
  FaceCluster_Cluster_args() {
  }

  virtual ~FaceCluster_Cluster_args() throw();
  ClusterInput input;

  _FaceCluster_Cluster_args__isset __isset;

  void __set_input(const ClusterInput& val);

  bool operator == (const FaceCluster_Cluster_args & rhs) const
  {
    if (!(input == rhs.input))
      return false;
    return true;
  }
  bool operator != (const FaceCluster_Cluster_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FaceCluster_Cluster_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class FaceCluster_Cluster_pargs {
 public:


  virtual ~FaceCluster_Cluster_pargs() throw();
  const ClusterInput* input;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FaceCluster_Cluster_result__isset {
  _FaceCluster_Cluster_result__isset() : success(false) {}
  bool success :1;
} _FaceCluster_Cluster_result__isset;

class FaceCluster_Cluster_result {
 public:

  FaceCluster_Cluster_result(const FaceCluster_Cluster_result&);
  FaceCluster_Cluster_result& operator=(const FaceCluster_Cluster_result&);
  FaceCluster_Cluster_result() {
  }

  virtual ~FaceCluster_Cluster_result() throw();
  ClusterResult success;

  _FaceCluster_Cluster_result__isset __isset;

  void __set_success(const ClusterResult& val);

  bool operator == (const FaceCluster_Cluster_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const FaceCluster_Cluster_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const FaceCluster_Cluster_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _FaceCluster_Cluster_presult__isset {
  _FaceCluster_Cluster_presult__isset() : success(false) {}
  bool success :1;
} _FaceCluster_Cluster_presult__isset;

class FaceCluster_Cluster_presult {
 public:


  virtual ~FaceCluster_Cluster_presult() throw();
  ClusterResult* success;

  _FaceCluster_Cluster_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class FaceClusterClient : virtual public FaceClusterIf {
 public:
  FaceClusterClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  FaceClusterClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void Cluster(ClusterResult& _return, const ClusterInput& input);
  void send_Cluster(const ClusterInput& input);
  void recv_Cluster(ClusterResult& _return);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class FaceClusterProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<FaceClusterIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (FaceClusterProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_Cluster(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  FaceClusterProcessor(::apache::thrift::stdcxx::shared_ptr<FaceClusterIf> iface) :
    iface_(iface) {
    processMap_["Cluster"] = &FaceClusterProcessor::process_Cluster;
  }

  virtual ~FaceClusterProcessor() {}
};

class FaceClusterProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  FaceClusterProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< FaceClusterIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< FaceClusterIfFactory > handlerFactory_;
};

class FaceClusterMultiface : virtual public FaceClusterIf {
 public:
  FaceClusterMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<FaceClusterIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~FaceClusterMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<FaceClusterIf> > ifaces_;
  FaceClusterMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<FaceClusterIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void Cluster(ClusterResult& _return, const ClusterInput& input) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->Cluster(_return, input);
    }
    ifaces_[i]->Cluster(_return, input);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class FaceClusterConcurrentClient : virtual public FaceClusterIf {
 public:
  FaceClusterConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  FaceClusterConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void Cluster(ClusterResult& _return, const ClusterInput& input);
  int32_t send_Cluster(const ClusterInput& input);
  void recv_Cluster(ClusterResult& _return, const int32_t seqid);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif



#endif