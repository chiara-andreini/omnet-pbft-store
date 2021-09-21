//
// Generated file, do not edit! Created by nedtool 5.6 from ProtocolMessages.msg.
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
#include "ProtocolMessages_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
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
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
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
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
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
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
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


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(ProtocolMessage)

ProtocolMessage::ProtocolMessage(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    for (unsigned int i=0; i<11; i++)
        this->bitmap[i] = 0;
    this->view = 0;
    this->sequenceNumber = 0;
}

ProtocolMessage::ProtocolMessage(const ProtocolMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

ProtocolMessage::~ProtocolMessage()
{
}

ProtocolMessage& ProtocolMessage::operator=(const ProtocolMessage& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ProtocolMessage::copy(const ProtocolMessage& other)
{
    for (unsigned int i=0; i<11; i++)
        this->bitmap[i] = other.bitmap[i];
    this->view = other.view;
    this->sequenceNumber = other.sequenceNumber;
    this->clientMessage = other.clientMessage;
    this->clientMessageDigest = other.clientMessageDigest;
    this->signature = other.signature;
}

void ProtocolMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimArrayPacking(b,this->bitmap,11);
    doParsimPacking(b,this->view);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->clientMessage);
    doParsimPacking(b,this->clientMessageDigest);
    doParsimPacking(b,this->signature);
}

void ProtocolMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimArrayUnpacking(b,this->bitmap,11);
    doParsimUnpacking(b,this->view);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->clientMessage);
    doParsimUnpacking(b,this->clientMessageDigest);
    doParsimUnpacking(b,this->signature);
}

unsigned int ProtocolMessage::getBitmapArraySize() const
{
    return 11;
}

int ProtocolMessage::getBitmap(unsigned int k) const
{
    if (k>=11) throw omnetpp::cRuntimeError("Array of size 11 indexed by %lu", (unsigned long)k);
    return this->bitmap[k];
}

void ProtocolMessage::setBitmap(unsigned int k, int bitmap)
{
    if (k>=11) throw omnetpp::cRuntimeError("Array of size 11 indexed by %lu", (unsigned long)k);
    this->bitmap[k] = bitmap;
}

int ProtocolMessage::getView() const
{
    return this->view;
}

void ProtocolMessage::setView(int view)
{
    this->view = view;
}

int ProtocolMessage::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void ProtocolMessage::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

const char * ProtocolMessage::getClientMessage() const
{
    return this->clientMessage.c_str();
}

void ProtocolMessage::setClientMessage(const char * clientMessage)
{
    this->clientMessage = clientMessage;
}

const char * ProtocolMessage::getClientMessageDigest() const
{
    return this->clientMessageDigest.c_str();
}

void ProtocolMessage::setClientMessageDigest(const char * clientMessageDigest)
{
    this->clientMessageDigest = clientMessageDigest;
}

const char * ProtocolMessage::getSignature() const
{
    return this->signature.c_str();
}

void ProtocolMessage::setSignature(const char * signature)
{
    this->signature = signature;
}

class ProtocolMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    ProtocolMessageDescriptor();
    virtual ~ProtocolMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(ProtocolMessageDescriptor)

ProtocolMessageDescriptor::ProtocolMessageDescriptor() : omnetpp::cClassDescriptor("ProtocolMessage", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

ProtocolMessageDescriptor::~ProtocolMessageDescriptor()
{
    delete[] propertynames;
}

bool ProtocolMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ProtocolMessage *>(obj)!=nullptr;
}

const char **ProtocolMessageDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *ProtocolMessageDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int ProtocolMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount() : 6;
}

unsigned int ProtocolMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *ProtocolMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "bitmap",
        "view",
        "sequenceNumber",
        "clientMessage",
        "clientMessageDigest",
        "signature",
    };
    return (field>=0 && field<6) ? fieldNames[field] : nullptr;
}

int ProtocolMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='b' && strcmp(fieldName, "bitmap")==0) return base+0;
    if (fieldName[0]=='v' && strcmp(fieldName, "view")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumber")==0) return base+2;
    if (fieldName[0]=='c' && strcmp(fieldName, "clientMessage")==0) return base+3;
    if (fieldName[0]=='c' && strcmp(fieldName, "clientMessageDigest")==0) return base+4;
    if (fieldName[0]=='s' && strcmp(fieldName, "signature")==0) return base+5;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *ProtocolMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "string",
        "string",
        "string",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : nullptr;
}

const char **ProtocolMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ProtocolMessageDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ProtocolMessageDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    ProtocolMessage *pp = (ProtocolMessage *)object; (void)pp;
    switch (field) {
        case 0: return 11;
        default: return 0;
    }
}

const char *ProtocolMessageDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    ProtocolMessage *pp = (ProtocolMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ProtocolMessageDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    ProtocolMessage *pp = (ProtocolMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getBitmap(i));
        case 1: return long2string(pp->getView());
        case 2: return long2string(pp->getSequenceNumber());
        case 3: return oppstring2string(pp->getClientMessage());
        case 4: return oppstring2string(pp->getClientMessageDigest());
        case 5: return oppstring2string(pp->getSignature());
        default: return "";
    }
}

bool ProtocolMessageDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    ProtocolMessage *pp = (ProtocolMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setBitmap(i,string2long(value)); return true;
        case 1: pp->setView(string2long(value)); return true;
        case 2: pp->setSequenceNumber(string2long(value)); return true;
        case 3: pp->setClientMessage((value)); return true;
        case 4: pp->setClientMessageDigest((value)); return true;
        case 5: pp->setSignature((value)); return true;
        default: return false;
    }
}

const char *ProtocolMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *ProtocolMessageDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    ProtocolMessage *pp = (ProtocolMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(PrePrepare)

PrePrepare::PrePrepare(const char *name, short kind) : ::ProtocolMessage(name,kind)
{
    this->originator = 0;
    this->clientId = 0;
}

PrePrepare::PrePrepare(const PrePrepare& other) : ::ProtocolMessage(other)
{
    copy(other);
}

PrePrepare::~PrePrepare()
{
}

PrePrepare& PrePrepare::operator=(const PrePrepare& other)
{
    if (this==&other) return *this;
    ::ProtocolMessage::operator=(other);
    copy(other);
    return *this;
}

void PrePrepare::copy(const PrePrepare& other)
{
    this->originator = other.originator;
    this->clientId = other.clientId;
}

void PrePrepare::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::ProtocolMessage::parsimPack(b);
    doParsimPacking(b,this->originator);
    doParsimPacking(b,this->clientId);
}

void PrePrepare::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::ProtocolMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->originator);
    doParsimUnpacking(b,this->clientId);
}

int PrePrepare::getOriginator() const
{
    return this->originator;
}

void PrePrepare::setOriginator(int originator)
{
    this->originator = originator;
}

int PrePrepare::getClientId() const
{
    return this->clientId;
}

void PrePrepare::setClientId(int clientId)
{
    this->clientId = clientId;
}

class PrePrepareDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    PrePrepareDescriptor();
    virtual ~PrePrepareDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(PrePrepareDescriptor)

PrePrepareDescriptor::PrePrepareDescriptor() : omnetpp::cClassDescriptor("PrePrepare", "ProtocolMessage")
{
    propertynames = nullptr;
}

PrePrepareDescriptor::~PrePrepareDescriptor()
{
    delete[] propertynames;
}

bool PrePrepareDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PrePrepare *>(obj)!=nullptr;
}

const char **PrePrepareDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *PrePrepareDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int PrePrepareDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int PrePrepareDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *PrePrepareDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "originator",
        "clientId",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int PrePrepareDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='o' && strcmp(fieldName, "originator")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "clientId")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *PrePrepareDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **PrePrepareDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PrePrepareDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PrePrepareDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    PrePrepare *pp = (PrePrepare *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *PrePrepareDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    PrePrepare *pp = (PrePrepare *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PrePrepareDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    PrePrepare *pp = (PrePrepare *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getOriginator());
        case 1: return long2string(pp->getClientId());
        default: return "";
    }
}

bool PrePrepareDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    PrePrepare *pp = (PrePrepare *)object; (void)pp;
    switch (field) {
        case 0: pp->setOriginator(string2long(value)); return true;
        case 1: pp->setClientId(string2long(value)); return true;
        default: return false;
    }
}

const char *PrePrepareDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *PrePrepareDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    PrePrepare *pp = (PrePrepare *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(Prepare)

Prepare::Prepare(const char *name, short kind) : ::ProtocolMessage(name,kind)
{
    this->senderOverlayKey = 0;
}

Prepare::Prepare(const Prepare& other) : ::ProtocolMessage(other)
{
    copy(other);
}

Prepare::~Prepare()
{
}

Prepare& Prepare::operator=(const Prepare& other)
{
    if (this==&other) return *this;
    ::ProtocolMessage::operator=(other);
    copy(other);
    return *this;
}

void Prepare::copy(const Prepare& other)
{
    this->senderOverlayKey = other.senderOverlayKey;
}

void Prepare::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::ProtocolMessage::parsimPack(b);
    doParsimPacking(b,this->senderOverlayKey);
}

void Prepare::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::ProtocolMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->senderOverlayKey);
}

int Prepare::getSenderOverlayKey() const
{
    return this->senderOverlayKey;
}

void Prepare::setSenderOverlayKey(int senderOverlayKey)
{
    this->senderOverlayKey = senderOverlayKey;
}

class PrepareDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    PrepareDescriptor();
    virtual ~PrepareDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(PrepareDescriptor)

PrepareDescriptor::PrepareDescriptor() : omnetpp::cClassDescriptor("Prepare", "ProtocolMessage")
{
    propertynames = nullptr;
}

PrepareDescriptor::~PrepareDescriptor()
{
    delete[] propertynames;
}

bool PrepareDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Prepare *>(obj)!=nullptr;
}

const char **PrepareDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *PrepareDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int PrepareDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int PrepareDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *PrepareDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "senderOverlayKey",
    };
    return (field>=0 && field<1) ? fieldNames[field] : nullptr;
}

int PrepareDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderOverlayKey")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *PrepareDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : nullptr;
}

const char **PrepareDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PrepareDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PrepareDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    Prepare *pp = (Prepare *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *PrepareDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Prepare *pp = (Prepare *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PrepareDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Prepare *pp = (Prepare *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSenderOverlayKey());
        default: return "";
    }
}

bool PrepareDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    Prepare *pp = (Prepare *)object; (void)pp;
    switch (field) {
        case 0: pp->setSenderOverlayKey(string2long(value)); return true;
        default: return false;
    }
}

const char *PrepareDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *PrepareDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    Prepare *pp = (Prepare *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(Commit)

Commit::Commit(const char *name, short kind) : ::ProtocolMessage(name,kind)
{
    this->senderOverlayKey = 0;
}

Commit::Commit(const Commit& other) : ::ProtocolMessage(other)
{
    copy(other);
}

Commit::~Commit()
{
}

Commit& Commit::operator=(const Commit& other)
{
    if (this==&other) return *this;
    ::ProtocolMessage::operator=(other);
    copy(other);
    return *this;
}

void Commit::copy(const Commit& other)
{
    this->senderOverlayKey = other.senderOverlayKey;
}

void Commit::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::ProtocolMessage::parsimPack(b);
    doParsimPacking(b,this->senderOverlayKey);
}

void Commit::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::ProtocolMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->senderOverlayKey);
}

int Commit::getSenderOverlayKey() const
{
    return this->senderOverlayKey;
}

void Commit::setSenderOverlayKey(int senderOverlayKey)
{
    this->senderOverlayKey = senderOverlayKey;
}

class CommitDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    CommitDescriptor();
    virtual ~CommitDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(CommitDescriptor)

CommitDescriptor::CommitDescriptor() : omnetpp::cClassDescriptor("Commit", "ProtocolMessage")
{
    propertynames = nullptr;
}

CommitDescriptor::~CommitDescriptor()
{
    delete[] propertynames;
}

bool CommitDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Commit *>(obj)!=nullptr;
}

const char **CommitDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CommitDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CommitDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int CommitDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *CommitDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "senderOverlayKey",
    };
    return (field>=0 && field<1) ? fieldNames[field] : nullptr;
}

int CommitDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "senderOverlayKey")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CommitDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : nullptr;
}

const char **CommitDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CommitDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CommitDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    Commit *pp = (Commit *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CommitDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Commit *pp = (Commit *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CommitDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Commit *pp = (Commit *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSenderOverlayKey());
        default: return "";
    }
}

bool CommitDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    Commit *pp = (Commit *)object; (void)pp;
    switch (field) {
        case 0: pp->setSenderOverlayKey(string2long(value)); return true;
        default: return false;
    }
}

const char *CommitDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *CommitDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    Commit *pp = (Commit *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(Certificate)

Certificate::Certificate(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    this->view = 0;
    this->sequenceNumber = 0;
    this->issuerOverlayKey = 0;
}

Certificate::Certificate(const Certificate& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

Certificate::~Certificate()
{
}

Certificate& Certificate::operator=(const Certificate& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void Certificate::copy(const Certificate& other)
{
    this->view = other.view;
    this->sequenceNumber = other.sequenceNumber;
    this->clientMessage = other.clientMessage;
    this->issuerOverlayKey = other.issuerOverlayKey;
}

void Certificate::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->view);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->clientMessage);
    doParsimPacking(b,this->issuerOverlayKey);
}

void Certificate::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->view);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->clientMessage);
    doParsimUnpacking(b,this->issuerOverlayKey);
}

int Certificate::getView() const
{
    return this->view;
}

void Certificate::setView(int view)
{
    this->view = view;
}

int Certificate::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void Certificate::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

const char * Certificate::getClientMessage() const
{
    return this->clientMessage.c_str();
}

void Certificate::setClientMessage(const char * clientMessage)
{
    this->clientMessage = clientMessage;
}

int Certificate::getIssuerOverlayKey() const
{
    return this->issuerOverlayKey;
}

void Certificate::setIssuerOverlayKey(int issuerOverlayKey)
{
    this->issuerOverlayKey = issuerOverlayKey;
}

class CertificateDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    CertificateDescriptor();
    virtual ~CertificateDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(CertificateDescriptor)

CertificateDescriptor::CertificateDescriptor() : omnetpp::cClassDescriptor("Certificate", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

CertificateDescriptor::~CertificateDescriptor()
{
    delete[] propertynames;
}

bool CertificateDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Certificate *>(obj)!=nullptr;
}

const char **CertificateDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CertificateDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CertificateDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int CertificateDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *CertificateDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "view",
        "sequenceNumber",
        "clientMessage",
        "issuerOverlayKey",
    };
    return (field>=0 && field<4) ? fieldNames[field] : nullptr;
}

int CertificateDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='v' && strcmp(fieldName, "view")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumber")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "clientMessage")==0) return base+2;
    if (fieldName[0]=='i' && strcmp(fieldName, "issuerOverlayKey")==0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CertificateDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "string",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : nullptr;
}

const char **CertificateDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CertificateDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CertificateDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    Certificate *pp = (Certificate *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CertificateDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Certificate *pp = (Certificate *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CertificateDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Certificate *pp = (Certificate *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getView());
        case 1: return long2string(pp->getSequenceNumber());
        case 2: return oppstring2string(pp->getClientMessage());
        case 3: return long2string(pp->getIssuerOverlayKey());
        default: return "";
    }
}

bool CertificateDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    Certificate *pp = (Certificate *)object; (void)pp;
    switch (field) {
        case 0: pp->setView(string2long(value)); return true;
        case 1: pp->setSequenceNumber(string2long(value)); return true;
        case 2: pp->setClientMessage((value)); return true;
        case 3: pp->setIssuerOverlayKey(string2long(value)); return true;
        default: return false;
    }
}

const char *CertificateDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *CertificateDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    Certificate *pp = (Certificate *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(PrepareCertificate)

PrepareCertificate::PrepareCertificate(const char *name, short kind) : ::Certificate(name,kind)
{
}

PrepareCertificate::PrepareCertificate(const PrepareCertificate& other) : ::Certificate(other)
{
    copy(other);
}

PrepareCertificate::~PrepareCertificate()
{
}

PrepareCertificate& PrepareCertificate::operator=(const PrepareCertificate& other)
{
    if (this==&other) return *this;
    ::Certificate::operator=(other);
    copy(other);
    return *this;
}

void PrepareCertificate::copy(const PrepareCertificate& other)
{
}

void PrepareCertificate::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::Certificate::parsimPack(b);
}

void PrepareCertificate::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::Certificate::parsimUnpack(b);
}

class PrepareCertificateDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    PrepareCertificateDescriptor();
    virtual ~PrepareCertificateDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(PrepareCertificateDescriptor)

PrepareCertificateDescriptor::PrepareCertificateDescriptor() : omnetpp::cClassDescriptor("PrepareCertificate", "Certificate")
{
    propertynames = nullptr;
}

PrepareCertificateDescriptor::~PrepareCertificateDescriptor()
{
    delete[] propertynames;
}

bool PrepareCertificateDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PrepareCertificate *>(obj)!=nullptr;
}

const char **PrepareCertificateDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *PrepareCertificateDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int PrepareCertificateDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount() : 0;
}

unsigned int PrepareCertificateDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    return 0;
}

const char *PrepareCertificateDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

int PrepareCertificateDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *PrepareCertificateDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

const char **PrepareCertificateDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PrepareCertificateDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PrepareCertificateDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    PrepareCertificate *pp = (PrepareCertificate *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *PrepareCertificateDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    PrepareCertificate *pp = (PrepareCertificate *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PrepareCertificateDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    PrepareCertificate *pp = (PrepareCertificate *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool PrepareCertificateDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    PrepareCertificate *pp = (PrepareCertificate *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *PrepareCertificateDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

void *PrepareCertificateDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    PrepareCertificate *pp = (PrepareCertificate *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(CommitLocalCertificate)

CommitLocalCertificate::CommitLocalCertificate(const char *name, short kind) : ::Certificate(name,kind)
{
}

CommitLocalCertificate::CommitLocalCertificate(const CommitLocalCertificate& other) : ::Certificate(other)
{
    copy(other);
}

CommitLocalCertificate::~CommitLocalCertificate()
{
}

CommitLocalCertificate& CommitLocalCertificate::operator=(const CommitLocalCertificate& other)
{
    if (this==&other) return *this;
    ::Certificate::operator=(other);
    copy(other);
    return *this;
}

void CommitLocalCertificate::copy(const CommitLocalCertificate& other)
{
}

void CommitLocalCertificate::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::Certificate::parsimPack(b);
}

void CommitLocalCertificate::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::Certificate::parsimUnpack(b);
}

class CommitLocalCertificateDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    CommitLocalCertificateDescriptor();
    virtual ~CommitLocalCertificateDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(CommitLocalCertificateDescriptor)

CommitLocalCertificateDescriptor::CommitLocalCertificateDescriptor() : omnetpp::cClassDescriptor("CommitLocalCertificate", "Certificate")
{
    propertynames = nullptr;
}

CommitLocalCertificateDescriptor::~CommitLocalCertificateDescriptor()
{
    delete[] propertynames;
}

bool CommitLocalCertificateDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<CommitLocalCertificate *>(obj)!=nullptr;
}

const char **CommitLocalCertificateDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *CommitLocalCertificateDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int CommitLocalCertificateDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 0+basedesc->getFieldCount() : 0;
}

unsigned int CommitLocalCertificateDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    return 0;
}

const char *CommitLocalCertificateDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

int CommitLocalCertificateDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *CommitLocalCertificateDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

const char **CommitLocalCertificateDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *CommitLocalCertificateDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int CommitLocalCertificateDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    CommitLocalCertificate *pp = (CommitLocalCertificate *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *CommitLocalCertificateDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CommitLocalCertificate *pp = (CommitLocalCertificate *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string CommitLocalCertificateDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    CommitLocalCertificate *pp = (CommitLocalCertificate *)object; (void)pp;
    switch (field) {
        default: return "";
    }
}

bool CommitLocalCertificateDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    CommitLocalCertificate *pp = (CommitLocalCertificate *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *CommitLocalCertificateDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    return nullptr;
}

void *CommitLocalCertificateDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    CommitLocalCertificate *pp = (CommitLocalCertificate *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


