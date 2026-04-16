//
// Generated file, do not edit! Created by opp_msgtool 6.3 from messages.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "messages_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n=v.size();
    doParsimPacking(buffer, n);
    for (int i=0; i<n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i=0; i<n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it=l.begin(); it!=l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it=s.begin(); it!=s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it=m.begin(); it!=m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k]=v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i=0; i<n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i=0; i<n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(SubtaskMsg)

SubtaskMsg::SubtaskMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

SubtaskMsg::SubtaskMsg(const SubtaskMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

SubtaskMsg::~SubtaskMsg()
{
    delete [] this->elements;
}

SubtaskMsg& SubtaskMsg::operator=(const SubtaskMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void SubtaskMsg::copy(const SubtaskMsg& other)
{
    this->subtaskId=other.subtaskId;
    this->initiatorId=other.initiatorId;
    this->targetClientId=other.targetClientId;
    this->sourceClientId=other.sourceClientId;
    delete [] this->elements;
    this->elements=(other.elements_arraysize==0) ? nullptr : new int[other.elements_arraysize];
    elements_arraysize=other.elements_arraysize;
    for (size_t i=0; i<elements_arraysize; i++) {
        this->elements[i]=other.elements[i];
    }
    this->hopCount=other.hopCount;
}

void SubtaskMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->subtaskId);
    doParsimPacking(b,this->initiatorId);
    doParsimPacking(b,this->targetClientId);
    doParsimPacking(b,this->sourceClientId);
    b->pack(elements_arraysize);
    doParsimArrayPacking(b,this->elements,elements_arraysize);
    doParsimPacking(b,this->hopCount);
}

void SubtaskMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->subtaskId);
    doParsimUnpacking(b,this->initiatorId);
    doParsimUnpacking(b,this->targetClientId);
    doParsimUnpacking(b,this->sourceClientId);
    delete [] this->elements;
    b->unpack(elements_arraysize);
    if (elements_arraysize==0) {
        this->elements=nullptr;
    } else {
        this->elements=new int[elements_arraysize];
        doParsimArrayUnpacking(b,this->elements,elements_arraysize);
    }
    doParsimUnpacking(b,this->hopCount);
}

int SubtaskMsg::getSubtaskId() const
{
    return this->subtaskId;
}

void SubtaskMsg::setSubtaskId(int subtaskId)
{
    this->subtaskId=subtaskId;
}

int SubtaskMsg::getInitiatorId() const
{
    return this->initiatorId;
}

void SubtaskMsg::setInitiatorId(int initiatorId)
{
    this->initiatorId=initiatorId;
}

int SubtaskMsg::getTargetClientId() const
{
    return this->targetClientId;
}

void SubtaskMsg::setTargetClientId(int targetClientId)
{
    this->targetClientId=targetClientId;
}

int SubtaskMsg::getSourceClientId() const
{
    return this->sourceClientId;
}

void SubtaskMsg::setSourceClientId(int sourceClientId)
{
    this->sourceClientId=sourceClientId;
}

size_t SubtaskMsg::getElementsArraySize() const
{
    return elements_arraysize;
}

int SubtaskMsg::getElements(size_t k) const
{
    if (k>=elements_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)elements_arraysize, (unsigned long)k);
    return this->elements[k];
}

void SubtaskMsg::setElementsArraySize(size_t newSize)
{
    int *elements2=(newSize==0) ? nullptr : new int[newSize];
    size_t minSize=elements_arraysize<newSize ? elements_arraysize : newSize;
    for (size_t i=0; i<minSize; i++)
        elements2[i]=this->elements[i];
    for (size_t i=minSize; i<newSize; i++)
        elements2[i]=0;
    delete [] this->elements;
    this->elements=elements2;
    elements_arraysize=newSize;
}

void SubtaskMsg::setElements(size_t k, int elements)
{
    if (k>=elements_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)elements_arraysize, (unsigned long)k);
    this->elements[k]=elements;
}

void SubtaskMsg::insertElements(size_t k, int elements)
{
    if (k>elements_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)elements_arraysize, (unsigned long)k);
    size_t newSize=elements_arraysize + 1;
    int *elements2=new int[newSize];
    size_t i;
    for (i=0; i<k; i++)
        elements2[i]=this->elements[i];
    elements2[k]=elements;
    for (i=k + 1; i<newSize; i++)
        elements2[i]=this->elements[i-1];
    delete [] this->elements;
    this->elements=elements2;
    elements_arraysize=newSize;
}

void SubtaskMsg::appendElements(int elements)
{
    insertElements(elements_arraysize, elements);
}

void SubtaskMsg::eraseElements(size_t k)
{
    if (k>=elements_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)elements_arraysize, (unsigned long)k);
    size_t newSize=elements_arraysize - 1;
    int *elements2=(newSize==0) ? nullptr : new int[newSize];
    size_t i;
    for (i=0; i<k; i++)
        elements2[i]=this->elements[i];
    for (i=k; i<newSize; i++)
        elements2[i]=this->elements[i+1];
    delete [] this->elements;
    this->elements=elements2;
    elements_arraysize=newSize;
}

int SubtaskMsg::getHopCount() const
{
    return this->hopCount;
}

void SubtaskMsg::setHopCount(int hopCount)
{
    this->hopCount=hopCount;
}

class SubtaskMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_subtaskId,
        FIELD_initiatorId,
        FIELD_targetClientId,
        FIELD_sourceClientId,
        FIELD_elements,
        FIELD_hopCount,
    };
  public:
    SubtaskMsgDescriptor();
    virtual ~SubtaskMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(SubtaskMsgDescriptor)

SubtaskMsgDescriptor::SubtaskMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(SubtaskMsg)), "omnetpp::cMessage")
{
    propertyNames=nullptr;
}

SubtaskMsgDescriptor::~SubtaskMsgDescriptor()
{
    delete[] propertyNames;
}

bool SubtaskMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SubtaskMsg *>(obj)!=nullptr;
}

const char **SubtaskMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[]={  nullptr };
        omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
        const char **baseNames=base ? base->getPropertyNames() : nullptr;
        propertyNames=mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *SubtaskMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int SubtaskMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    return base ? 6+base->getFieldCount() : 6;
}

unsigned int SubtaskMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -=base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[]={
        FD_ISEDITABLE,    // FIELD_subtaskId
        FD_ISEDITABLE,    // FIELD_initiatorId
        FD_ISEDITABLE,    // FIELD_targetClientId
        FD_ISEDITABLE,    // FIELD_sourceClientId
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_elements
        FD_ISEDITABLE,    // FIELD_hopCount
    };
    return (field>=0&&field<6) ? fieldTypeFlags[field] : 0;
}

const char *SubtaskMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldName(field);
        field -=base->getFieldCount();
    }
    static const char *fieldNames[]={
        "subtaskId",
        "initiatorId",
        "targetClientId",
        "sourceClientId",
        "elements",
        "hopCount",
    };
    return (field>=0&&field<6) ? fieldNames[field] : nullptr;
}

int SubtaskMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    int baseIndex=base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "subtaskId")==0) return baseIndex + 0;
    if (strcmp(fieldName, "initiatorId")==0) return baseIndex + 1;
    if (strcmp(fieldName, "targetClientId")==0) return baseIndex + 2;
    if (strcmp(fieldName, "sourceClientId")==0) return baseIndex + 3;
    if (strcmp(fieldName, "elements")==0) return baseIndex + 4;
    if (strcmp(fieldName, "hopCount")==0) return baseIndex + 5;
    return base ? base->findField(fieldName) : -1;
}

const char *SubtaskMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldTypeString(field);
        field -=base->getFieldCount();
    }
    static const char *fieldTypeStrings[]={
        "int",    // FIELD_subtaskId
        "int",    // FIELD_initiatorId
        "int",    // FIELD_targetClientId
        "int",    // FIELD_sourceClientId
        "int",    // FIELD_elements
        "int",    // FIELD_hopCount
    };
    return (field>=0&&field<6) ? fieldTypeStrings[field] : nullptr;
}

const char **SubtaskMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -=base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SubtaskMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -=base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SubtaskMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -=base->getFieldCount();
    }
    SubtaskMsg *pp=omnetpp::fromAnyPtr<SubtaskMsg>(object); (void)pp;
    switch (field) {
        case FIELD_elements: return pp->getElementsArraySize();
        default: return 0;
    }
}

void SubtaskMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -=base->getFieldCount();
    }
    SubtaskMsg *pp=omnetpp::fromAnyPtr<SubtaskMsg>(object); (void)pp;
    switch (field) {
        case FIELD_elements: pp->setElementsArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'SubtaskMsg'", field);
    }
}

const char *SubtaskMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -=base->getFieldCount();
    }
    SubtaskMsg *pp=omnetpp::fromAnyPtr<SubtaskMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SubtaskMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -=base->getFieldCount();
    }
    SubtaskMsg *pp=omnetpp::fromAnyPtr<SubtaskMsg>(object); (void)pp;
    switch (field) {
        case FIELD_subtaskId: return long2string(pp->getSubtaskId());
        case FIELD_initiatorId: return long2string(pp->getInitiatorId());
        case FIELD_targetClientId: return long2string(pp->getTargetClientId());
        case FIELD_sourceClientId: return long2string(pp->getSourceClientId());
        case FIELD_elements: return long2string(pp->getElements(i));
        case FIELD_hopCount: return long2string(pp->getHopCount());
        default: return "";
    }
}

void SubtaskMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -=base->getFieldCount();
    }
    SubtaskMsg *pp=omnetpp::fromAnyPtr<SubtaskMsg>(object); (void)pp;
    switch (field) {
        case FIELD_subtaskId: pp->setSubtaskId(string2long(value)); break;
        case FIELD_initiatorId: pp->setInitiatorId(string2long(value)); break;
        case FIELD_targetClientId: pp->setTargetClientId(string2long(value)); break;
        case FIELD_sourceClientId: pp->setSourceClientId(string2long(value)); break;
        case FIELD_elements: pp->setElements(i,string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SubtaskMsg'", field);
    }
}

omnetpp::cValue SubtaskMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -=base->getFieldCount();
    }
    SubtaskMsg *pp=omnetpp::fromAnyPtr<SubtaskMsg>(object); (void)pp;
    switch (field) {
        case FIELD_subtaskId: return pp->getSubtaskId();
        case FIELD_initiatorId: return pp->getInitiatorId();
        case FIELD_targetClientId: return pp->getTargetClientId();
        case FIELD_sourceClientId: return pp->getSourceClientId();
        case FIELD_elements: return pp->getElements(i);
        case FIELD_hopCount: return pp->getHopCount();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'SubtaskMsg' as cValue -- field index out of range?", field);
    }
}

void SubtaskMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -=base->getFieldCount();
    }
    SubtaskMsg *pp=omnetpp::fromAnyPtr<SubtaskMsg>(object); (void)pp;
    switch (field) {
        case FIELD_subtaskId: pp->setSubtaskId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_initiatorId: pp->setInitiatorId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_targetClientId: pp->setTargetClientId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sourceClientId: pp->setSourceClientId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_elements: pp->setElements(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SubtaskMsg'", field);
    }
}

const char *SubtaskMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldStructName(field);
        field -=base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr SubtaskMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -=base->getFieldCount();
    }
    SubtaskMsg *pp=omnetpp::fromAnyPtr<SubtaskMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void SubtaskMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -=base->getFieldCount();
    }
    SubtaskMsg *pp=omnetpp::fromAnyPtr<SubtaskMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SubtaskMsg'", field);
    }
}

Register_Class(SubtaskResultMsg)

SubtaskResultMsg::SubtaskResultMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

SubtaskResultMsg::SubtaskResultMsg(const SubtaskResultMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

SubtaskResultMsg::~SubtaskResultMsg()
{
}

SubtaskResultMsg& SubtaskResultMsg::operator=(const SubtaskResultMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void SubtaskResultMsg::copy(const SubtaskResultMsg& other)
{
    this->subtaskId=other.subtaskId;
    this->initiatorId=other.initiatorId;
    this->targetClientId=other.targetClientId;
    this->sourceClientId=other.sourceClientId;
    this->maxValue=other.maxValue;
    this->hopCount=other.hopCount;
}

void SubtaskResultMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->subtaskId);
    doParsimPacking(b,this->initiatorId);
    doParsimPacking(b,this->targetClientId);
    doParsimPacking(b,this->sourceClientId);
    doParsimPacking(b,this->maxValue);
    doParsimPacking(b,this->hopCount);
}

void SubtaskResultMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->subtaskId);
    doParsimUnpacking(b,this->initiatorId);
    doParsimUnpacking(b,this->targetClientId);
    doParsimUnpacking(b,this->sourceClientId);
    doParsimUnpacking(b,this->maxValue);
    doParsimUnpacking(b,this->hopCount);
}

int SubtaskResultMsg::getSubtaskId() const
{
    return this->subtaskId;
}

void SubtaskResultMsg::setSubtaskId(int subtaskId)
{
    this->subtaskId=subtaskId;
}

int SubtaskResultMsg::getInitiatorId() const
{
    return this->initiatorId;
}

void SubtaskResultMsg::setInitiatorId(int initiatorId)
{
    this->initiatorId=initiatorId;
}

int SubtaskResultMsg::getTargetClientId() const
{
    return this->targetClientId;
}

void SubtaskResultMsg::setTargetClientId(int targetClientId)
{
    this->targetClientId=targetClientId;
}

int SubtaskResultMsg::getSourceClientId() const
{
    return this->sourceClientId;
}

void SubtaskResultMsg::setSourceClientId(int sourceClientId)
{
    this->sourceClientId=sourceClientId;
}

int SubtaskResultMsg::getMaxValue() const
{
    return this->maxValue;
}

void SubtaskResultMsg::setMaxValue(int maxValue)
{
    this->maxValue=maxValue;
}

int SubtaskResultMsg::getHopCount() const
{
    return this->hopCount;
}

void SubtaskResultMsg::setHopCount(int hopCount)
{
    this->hopCount=hopCount;
}

class SubtaskResultMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_subtaskId,
        FIELD_initiatorId,
        FIELD_targetClientId,
        FIELD_sourceClientId,
        FIELD_maxValue,
        FIELD_hopCount,
    };
  public:
    SubtaskResultMsgDescriptor();
    virtual ~SubtaskResultMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(SubtaskResultMsgDescriptor)

SubtaskResultMsgDescriptor::SubtaskResultMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(SubtaskResultMsg)), "omnetpp::cMessage")
{
    propertyNames=nullptr;
}

SubtaskResultMsgDescriptor::~SubtaskResultMsgDescriptor()
{
    delete[] propertyNames;
}

bool SubtaskResultMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SubtaskResultMsg *>(obj)!=nullptr;
}

const char **SubtaskResultMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[]={  nullptr };
        omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
        const char **baseNames=base ? base->getPropertyNames() : nullptr;
        propertyNames=mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *SubtaskResultMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int SubtaskResultMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    return base ? 6+base->getFieldCount() : 6;
}

unsigned int SubtaskResultMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -=base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[]={
        FD_ISEDITABLE,    // FIELD_subtaskId
        FD_ISEDITABLE,    // FIELD_initiatorId
        FD_ISEDITABLE,    // FIELD_targetClientId
        FD_ISEDITABLE,    // FIELD_sourceClientId
        FD_ISEDITABLE,    // FIELD_maxValue
        FD_ISEDITABLE,    // FIELD_hopCount
    };
    return (field>=0&&field<6) ? fieldTypeFlags[field] : 0;
}

const char *SubtaskResultMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldName(field);
        field -=base->getFieldCount();
    }
    static const char *fieldNames[]={
        "subtaskId",
        "initiatorId",
        "targetClientId",
        "sourceClientId",
        "maxValue",
        "hopCount",
    };
    return (field>=0&&field<6) ? fieldNames[field] : nullptr;
}

int SubtaskResultMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    int baseIndex=base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "subtaskId")==0) return baseIndex + 0;
    if (strcmp(fieldName, "initiatorId")==0) return baseIndex + 1;
    if (strcmp(fieldName, "targetClientId")==0) return baseIndex + 2;
    if (strcmp(fieldName, "sourceClientId")==0) return baseIndex + 3;
    if (strcmp(fieldName, "maxValue")==0) return baseIndex + 4;
    if (strcmp(fieldName, "hopCount")==0) return baseIndex + 5;
    return base ? base->findField(fieldName) : -1;
}

const char *SubtaskResultMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldTypeString(field);
        field -=base->getFieldCount();
    }
    static const char *fieldTypeStrings[]={
        "int",    // FIELD_subtaskId
        "int",    // FIELD_initiatorId
        "int",    // FIELD_targetClientId
        "int",    // FIELD_sourceClientId
        "int",    // FIELD_maxValue
        "int",    // FIELD_hopCount
    };
    return (field>=0&&field<6) ? fieldTypeStrings[field] : nullptr;
}

const char **SubtaskResultMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -=base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SubtaskResultMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -=base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SubtaskResultMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -=base->getFieldCount();
    }
    SubtaskResultMsg *pp=omnetpp::fromAnyPtr<SubtaskResultMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void SubtaskResultMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -=base->getFieldCount();
    }
    SubtaskResultMsg *pp=omnetpp::fromAnyPtr<SubtaskResultMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'SubtaskResultMsg'", field);
    }
}

const char *SubtaskResultMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -=base->getFieldCount();
    }
    SubtaskResultMsg *pp=omnetpp::fromAnyPtr<SubtaskResultMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SubtaskResultMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -=base->getFieldCount();
    }
    SubtaskResultMsg *pp=omnetpp::fromAnyPtr<SubtaskResultMsg>(object); (void)pp;
    switch (field) {
        case FIELD_subtaskId: return long2string(pp->getSubtaskId());
        case FIELD_initiatorId: return long2string(pp->getInitiatorId());
        case FIELD_targetClientId: return long2string(pp->getTargetClientId());
        case FIELD_sourceClientId: return long2string(pp->getSourceClientId());
        case FIELD_maxValue: return long2string(pp->getMaxValue());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        default: return "";
    }
}

void SubtaskResultMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -=base->getFieldCount();
    }
    SubtaskResultMsg *pp=omnetpp::fromAnyPtr<SubtaskResultMsg>(object); (void)pp;
    switch (field) {
        case FIELD_subtaskId: pp->setSubtaskId(string2long(value)); break;
        case FIELD_initiatorId: pp->setInitiatorId(string2long(value)); break;
        case FIELD_targetClientId: pp->setTargetClientId(string2long(value)); break;
        case FIELD_sourceClientId: pp->setSourceClientId(string2long(value)); break;
        case FIELD_maxValue: pp->setMaxValue(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SubtaskResultMsg'", field);
    }
}

omnetpp::cValue SubtaskResultMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -=base->getFieldCount();
    }
    SubtaskResultMsg *pp=omnetpp::fromAnyPtr<SubtaskResultMsg>(object); (void)pp;
    switch (field) {
        case FIELD_subtaskId: return pp->getSubtaskId();
        case FIELD_initiatorId: return pp->getInitiatorId();
        case FIELD_targetClientId: return pp->getTargetClientId();
        case FIELD_sourceClientId: return pp->getSourceClientId();
        case FIELD_maxValue: return pp->getMaxValue();
        case FIELD_hopCount: return pp->getHopCount();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'SubtaskResultMsg' as cValue -- field index out of range?", field);
    }
}

void SubtaskResultMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -=base->getFieldCount();
    }
    SubtaskResultMsg *pp=omnetpp::fromAnyPtr<SubtaskResultMsg>(object); (void)pp;
    switch (field) {
        case FIELD_subtaskId: pp->setSubtaskId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_initiatorId: pp->setInitiatorId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_targetClientId: pp->setTargetClientId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sourceClientId: pp->setSourceClientId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_maxValue: pp->setMaxValue(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SubtaskResultMsg'", field);
    }
}

const char *SubtaskResultMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldStructName(field);
        field -=base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr SubtaskResultMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -=base->getFieldCount();
    }
    SubtaskResultMsg *pp=omnetpp::fromAnyPtr<SubtaskResultMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void SubtaskResultMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -=base->getFieldCount();
    }
    SubtaskResultMsg *pp=omnetpp::fromAnyPtr<SubtaskResultMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SubtaskResultMsg'", field);
    }
}

Register_Class(GossipMsg)

GossipMsg::GossipMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

GossipMsg::GossipMsg(const GossipMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

GossipMsg::~GossipMsg()
{
}

GossipMsg& GossipMsg::operator=(const GossipMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void GossipMsg::copy(const GossipMsg& other)
{
    this->originClientId=other.originClientId;
    this->timestamp=other.timestamp;
    this->ipAddress=other.ipAddress;
    this->msgContent=other.msgContent;
    this->senderClientId=other.senderClientId;
    this->msgHash=other.msgHash;
}

void GossipMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->originClientId);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->ipAddress);
    doParsimPacking(b,this->msgContent);
    doParsimPacking(b,this->senderClientId);
    doParsimPacking(b,this->msgHash);
}

void GossipMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->originClientId);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->ipAddress);
    doParsimUnpacking(b,this->msgContent);
    doParsimUnpacking(b,this->senderClientId);
    doParsimUnpacking(b,this->msgHash);
}

int GossipMsg::getOriginClientId() const
{
    return this->originClientId;
}

void GossipMsg::setOriginClientId(int originClientId)
{
    this->originClientId=originClientId;
}

const char * GossipMsg::getTimestamp() const
{
    return this->timestamp.c_str();
}

void GossipMsg::setTimestamp(const char * timestamp)
{
    this->timestamp=timestamp;
}

const char * GossipMsg::getIpAddress() const
{
    return this->ipAddress.c_str();
}

void GossipMsg::setIpAddress(const char * ipAddress)
{
    this->ipAddress=ipAddress;
}

const char * GossipMsg::getMsgContent() const
{
    return this->msgContent.c_str();
}

void GossipMsg::setMsgContent(const char * msgContent)
{
    this->msgContent=msgContent;
}

int GossipMsg::getSenderClientId() const
{
    return this->senderClientId;
}

void GossipMsg::setSenderClientId(int senderClientId)
{
    this->senderClientId=senderClientId;
}

const char * GossipMsg::getMsgHash() const
{
    return this->msgHash.c_str();
}

void GossipMsg::setMsgHash(const char * msgHash)
{
    this->msgHash=msgHash;
}

class GossipMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_originClientId,
        FIELD_timestamp,
        FIELD_ipAddress,
        FIELD_msgContent,
        FIELD_senderClientId,
        FIELD_msgHash,
    };
  public:
    GossipMsgDescriptor();
    virtual ~GossipMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(GossipMsgDescriptor)

GossipMsgDescriptor::GossipMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(GossipMsg)), "omnetpp::cMessage")
{
    propertyNames=nullptr;
}

GossipMsgDescriptor::~GossipMsgDescriptor()
{
    delete[] propertyNames;
}

bool GossipMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<GossipMsg *>(obj)!=nullptr;
}

const char **GossipMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[]={  nullptr };
        omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
        const char **baseNames=base ? base->getPropertyNames() : nullptr;
        propertyNames=mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *GossipMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int GossipMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    return base ? 6+base->getFieldCount() : 6;
}

unsigned int GossipMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -=base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[]={
        FD_ISEDITABLE,    // FIELD_originClientId
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_ipAddress
        FD_ISEDITABLE,    // FIELD_msgContent
        FD_ISEDITABLE,    // FIELD_senderClientId
        FD_ISEDITABLE,    // FIELD_msgHash
    };
    return (field>=0&&field<6) ? fieldTypeFlags[field] : 0;
}

const char *GossipMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldName(field);
        field -=base->getFieldCount();
    }
    static const char *fieldNames[]={
        "originClientId",
        "timestamp",
        "ipAddress",
        "msgContent",
        "senderClientId",
        "msgHash",
    };
    return (field>=0&&field<6) ? fieldNames[field] : nullptr;
}

int GossipMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    int baseIndex=base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "originClientId")==0) return baseIndex + 0;
    if (strcmp(fieldName, "timestamp")==0) return baseIndex + 1;
    if (strcmp(fieldName, "ipAddress")==0) return baseIndex + 2;
    if (strcmp(fieldName, "msgContent")==0) return baseIndex + 3;
    if (strcmp(fieldName, "senderClientId")==0) return baseIndex + 4;
    if (strcmp(fieldName, "msgHash")==0) return baseIndex + 5;
    return base ? base->findField(fieldName) : -1;
}

const char *GossipMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldTypeString(field);
        field -=base->getFieldCount();
    }
    static const char *fieldTypeStrings[]={
        "int",    // FIELD_originClientId
        "string",    // FIELD_timestamp
        "string",    // FIELD_ipAddress
        "string",    // FIELD_msgContent
        "int",    // FIELD_senderClientId
        "string",    // FIELD_msgHash
    };
    return (field>=0&&field<6) ? fieldTypeStrings[field] : nullptr;
}

const char **GossipMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -=base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *GossipMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -=base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int GossipMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -=base->getFieldCount();
    }
    GossipMsg *pp=omnetpp::fromAnyPtr<GossipMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void GossipMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -=base->getFieldCount();
    }
    GossipMsg *pp=omnetpp::fromAnyPtr<GossipMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'GossipMsg'", field);
    }
}

const char *GossipMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -=base->getFieldCount();
    }
    GossipMsg *pp=omnetpp::fromAnyPtr<GossipMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string GossipMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -=base->getFieldCount();
    }
    GossipMsg *pp=omnetpp::fromAnyPtr<GossipMsg>(object); (void)pp;
    switch (field) {
        case FIELD_originClientId: return long2string(pp->getOriginClientId());
        case FIELD_timestamp: return oppstring2string(pp->getTimestamp());
        case FIELD_ipAddress: return oppstring2string(pp->getIpAddress());
        case FIELD_msgContent: return oppstring2string(pp->getMsgContent());
        case FIELD_senderClientId: return long2string(pp->getSenderClientId());
        case FIELD_msgHash: return oppstring2string(pp->getMsgHash());
        default: return "";
    }
}

void GossipMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -=base->getFieldCount();
    }
    GossipMsg *pp=omnetpp::fromAnyPtr<GossipMsg>(object); (void)pp;
    switch (field) {
        case FIELD_originClientId: pp->setOriginClientId(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp((value)); break;
        case FIELD_ipAddress: pp->setIpAddress((value)); break;
        case FIELD_msgContent: pp->setMsgContent((value)); break;
        case FIELD_senderClientId: pp->setSenderClientId(string2long(value)); break;
        case FIELD_msgHash: pp->setMsgHash((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'GossipMsg'", field);
    }
}

omnetpp::cValue GossipMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -=base->getFieldCount();
    }
    GossipMsg *pp=omnetpp::fromAnyPtr<GossipMsg>(object); (void)pp;
    switch (field) {
        case FIELD_originClientId: return pp->getOriginClientId();
        case FIELD_timestamp: return pp->getTimestamp();
        case FIELD_ipAddress: return pp->getIpAddress();
        case FIELD_msgContent: return pp->getMsgContent();
        case FIELD_senderClientId: return pp->getSenderClientId();
        case FIELD_msgHash: return pp->getMsgHash();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'GossipMsg' as cValue -- field index out of range?", field);
    }
}

void GossipMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -=base->getFieldCount();
    }
    GossipMsg *pp=omnetpp::fromAnyPtr<GossipMsg>(object); (void)pp;
    switch (field) {
        case FIELD_originClientId: pp->setOriginClientId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.stringValue()); break;
        case FIELD_ipAddress: pp->setIpAddress(value.stringValue()); break;
        case FIELD_msgContent: pp->setMsgContent(value.stringValue()); break;
        case FIELD_senderClientId: pp->setSenderClientId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_msgHash: pp->setMsgHash(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'GossipMsg'", field);
    }
}

const char *GossipMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldStructName(field);
        field -=base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr GossipMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -=base->getFieldCount();
    }
    GossipMsg *pp=omnetpp::fromAnyPtr<GossipMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void GossipMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base=getBaseClassDescriptor();
    if (base) {
        if (field<base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -=base->getFieldCount();
    }
    GossipMsg *pp=omnetpp::fromAnyPtr<GossipMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'GossipMsg'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp
