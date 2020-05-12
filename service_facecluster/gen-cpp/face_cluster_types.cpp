/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "face_cluster_types.h"

#include <algorithm>
#include <ostream>

#include <thrift/TToString.h>




ClusterInput::~ClusterInput() throw() {
}


void ClusterInput::__set_featurelist(const std::vector<std::vector<double> > & val) {
  this->featurelist = val;
}

void ClusterInput::__set_uid(const int64_t val) {
  this->uid = val;
__isset.uid = true;
}

void ClusterInput::__set_did(const std::string& val) {
  this->did = val;
__isset.did = true;
}
std::ostream& operator<<(std::ostream& out, const ClusterInput& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t ClusterInput::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_featurelist = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->featurelist.clear();
            uint32_t _size0;
            ::apache::thrift::protocol::TType _etype3;
            xfer += iprot->readListBegin(_etype3, _size0);
            this->featurelist.resize(_size0);
            uint32_t _i4;
            for (_i4 = 0; _i4 < _size0; ++_i4)
            {
              {
                this->featurelist[_i4].clear();
                uint32_t _size5;
                ::apache::thrift::protocol::TType _etype8;
                xfer += iprot->readListBegin(_etype8, _size5);
                this->featurelist[_i4].resize(_size5);
                uint32_t _i9;
                for (_i9 = 0; _i9 < _size5; ++_i9)
                {
                  xfer += iprot->readDouble(this->featurelist[_i4][_i9]);
                }
                xfer += iprot->readListEnd();
              }
            }
            xfer += iprot->readListEnd();
          }
          isset_featurelist = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_I64) {
          xfer += iprot->readI64(this->uid);
          this->__isset.uid = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_STRING) {
          xfer += iprot->readString(this->did);
          this->__isset.did = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_featurelist)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t ClusterInput::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("ClusterInput");

  xfer += oprot->writeFieldBegin("featurelist", ::apache::thrift::protocol::T_LIST, 1);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_LIST, static_cast<uint32_t>(this->featurelist.size()));
    std::vector<std::vector<double> > ::const_iterator _iter10;
    for (_iter10 = this->featurelist.begin(); _iter10 != this->featurelist.end(); ++_iter10)
    {
      {
        xfer += oprot->writeListBegin(::apache::thrift::protocol::T_DOUBLE, static_cast<uint32_t>((*_iter10).size()));
        std::vector<double> ::const_iterator _iter11;
        for (_iter11 = (*_iter10).begin(); _iter11 != (*_iter10).end(); ++_iter11)
        {
          xfer += oprot->writeDouble((*_iter11));
        }
        xfer += oprot->writeListEnd();
      }
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  if (this->__isset.uid) {
    xfer += oprot->writeFieldBegin("uid", ::apache::thrift::protocol::T_I64, 2);
    xfer += oprot->writeI64(this->uid);
    xfer += oprot->writeFieldEnd();
  }
  if (this->__isset.did) {
    xfer += oprot->writeFieldBegin("did", ::apache::thrift::protocol::T_STRING, 3);
    xfer += oprot->writeString(this->did);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ClusterInput &a, ClusterInput &b) {
  using ::std::swap;
  swap(a.featurelist, b.featurelist);
  swap(a.uid, b.uid);
  swap(a.did, b.did);
  swap(a.__isset, b.__isset);
}

ClusterInput::ClusterInput(const ClusterInput& other12) {
  featurelist = other12.featurelist;
  uid = other12.uid;
  did = other12.did;
  __isset = other12.__isset;
}
ClusterInput& ClusterInput::operator=(const ClusterInput& other13) {
  featurelist = other13.featurelist;
  uid = other13.uid;
  did = other13.did;
  __isset = other13.__isset;
  return *this;
}
void ClusterInput::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "ClusterInput(";
  out << "featurelist=" << to_string(featurelist);
  out << ", " << "uid="; (__isset.uid ? (out << to_string(uid)) : (out << "<null>"));
  out << ", " << "did="; (__isset.did ? (out << to_string(did)) : (out << "<null>"));
  out << ")";
}


ClusterResult::~ClusterResult() throw() {
}


void ClusterResult::__set_cluster_num(const int32_t val) {
  this->cluster_num = val;
}

void ClusterResult::__set_labels(const std::vector<int32_t> & val) {
  this->labels = val;
}

void ClusterResult::__set_busy(const bool val) {
  this->busy = val;
__isset.busy = true;
}
std::ostream& operator<<(std::ostream& out, const ClusterResult& obj)
{
  obj.printTo(out);
  return out;
}


uint32_t ClusterResult::read(::apache::thrift::protocol::TProtocol* iprot) {

  ::apache::thrift::protocol::TInputRecursionTracker tracker(*iprot);
  uint32_t xfer = 0;
  std::string fname;
  ::apache::thrift::protocol::TType ftype;
  int16_t fid;

  xfer += iprot->readStructBegin(fname);

  using ::apache::thrift::protocol::TProtocolException;

  bool isset_cluster_num = false;
  bool isset_labels = false;

  while (true)
  {
    xfer += iprot->readFieldBegin(fname, ftype, fid);
    if (ftype == ::apache::thrift::protocol::T_STOP) {
      break;
    }
    switch (fid)
    {
      case 1:
        if (ftype == ::apache::thrift::protocol::T_I32) {
          xfer += iprot->readI32(this->cluster_num);
          isset_cluster_num = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 2:
        if (ftype == ::apache::thrift::protocol::T_LIST) {
          {
            this->labels.clear();
            uint32_t _size14;
            ::apache::thrift::protocol::TType _etype17;
            xfer += iprot->readListBegin(_etype17, _size14);
            this->labels.resize(_size14);
            uint32_t _i18;
            for (_i18 = 0; _i18 < _size14; ++_i18)
            {
              xfer += iprot->readI32(this->labels[_i18]);
            }
            xfer += iprot->readListEnd();
          }
          isset_labels = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      case 3:
        if (ftype == ::apache::thrift::protocol::T_BOOL) {
          xfer += iprot->readBool(this->busy);
          this->__isset.busy = true;
        } else {
          xfer += iprot->skip(ftype);
        }
        break;
      default:
        xfer += iprot->skip(ftype);
        break;
    }
    xfer += iprot->readFieldEnd();
  }

  xfer += iprot->readStructEnd();

  if (!isset_cluster_num)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  if (!isset_labels)
    throw TProtocolException(TProtocolException::INVALID_DATA);
  return xfer;
}

uint32_t ClusterResult::write(::apache::thrift::protocol::TProtocol* oprot) const {
  uint32_t xfer = 0;
  ::apache::thrift::protocol::TOutputRecursionTracker tracker(*oprot);
  xfer += oprot->writeStructBegin("ClusterResult");

  xfer += oprot->writeFieldBegin("cluster_num", ::apache::thrift::protocol::T_I32, 1);
  xfer += oprot->writeI32(this->cluster_num);
  xfer += oprot->writeFieldEnd();

  xfer += oprot->writeFieldBegin("labels", ::apache::thrift::protocol::T_LIST, 2);
  {
    xfer += oprot->writeListBegin(::apache::thrift::protocol::T_I32, static_cast<uint32_t>(this->labels.size()));
    std::vector<int32_t> ::const_iterator _iter19;
    for (_iter19 = this->labels.begin(); _iter19 != this->labels.end(); ++_iter19)
    {
      xfer += oprot->writeI32((*_iter19));
    }
    xfer += oprot->writeListEnd();
  }
  xfer += oprot->writeFieldEnd();

  if (this->__isset.busy) {
    xfer += oprot->writeFieldBegin("busy", ::apache::thrift::protocol::T_BOOL, 3);
    xfer += oprot->writeBool(this->busy);
    xfer += oprot->writeFieldEnd();
  }
  xfer += oprot->writeFieldStop();
  xfer += oprot->writeStructEnd();
  return xfer;
}

void swap(ClusterResult &a, ClusterResult &b) {
  using ::std::swap;
  swap(a.cluster_num, b.cluster_num);
  swap(a.labels, b.labels);
  swap(a.busy, b.busy);
  swap(a.__isset, b.__isset);
}

ClusterResult::ClusterResult(const ClusterResult& other20) {
  cluster_num = other20.cluster_num;
  labels = other20.labels;
  busy = other20.busy;
  __isset = other20.__isset;
}
ClusterResult& ClusterResult::operator=(const ClusterResult& other21) {
  cluster_num = other21.cluster_num;
  labels = other21.labels;
  busy = other21.busy;
  __isset = other21.__isset;
  return *this;
}
void ClusterResult::printTo(std::ostream& out) const {
  using ::apache::thrift::to_string;
  out << "ClusterResult(";
  out << "cluster_num=" << to_string(cluster_num);
  out << ", " << "labels=" << to_string(labels);
  out << ", " << "busy="; (__isset.busy ? (out << to_string(busy)) : (out << "<null>"));
  out << ")";
}

