//
// Generated file, do not edit! Created by nedtool 5.6 from ClientReply.msg.
//

#ifndef __CLIENTREPLY_M_H
#define __CLIENTREPLY_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>ClientReply.msg:19</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * message ClientReply
 * {
 *     // The requested operation over the store may be "GET" and "PUT".
 *     string clientMessage;
 *     string result;
 * 
 *     int sender;
 *     int originator;
 *     int clientId;
 * 
 *     int view;
 * 
 *     // Timestamp is simtime_t timestamp in the _m class.
 * 
 *     string digest;
 *     string signature;
 * 
 * }
 * </pre>
 */
class ClientReply : public ::omnetpp::cMessage
{
  protected:
    ::omnetpp::opp_string clientMessage;
    ::omnetpp::opp_string clientMessageDigest;

    ::omnetpp::opp_string result;
    int sender;
    int originator;
    int clientId;
    int view;
    ::omnetpp::opp_string digest;
    ::omnetpp::opp_string signature;
    ::omnetpp::simtime_t timestamp;

  private:
    void copy(const ClientReply& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const ClientReply&);

  public:
    ClientReply(const char *name=nullptr, short kind=0);
    ClientReply(const ClientReply& other);
    virtual ~ClientReply();
    ClientReply& operator=(const ClientReply& other);
    virtual ClientReply *dup() const override {return new ClientReply(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual const char * getClientMessage() const;
    virtual void setClientMessage(const char * clientMessage);
    virtual const char * getResult() const;
    virtual void setResult(const char * result);
    virtual int getSender() const;
    virtual void setSender(int sender);
    virtual int getOriginator() const;
    virtual void setOriginator(int originator);
    virtual int getClientId() const;
    virtual void setClientId(int clientId);
    virtual int getView() const;
    virtual void setView(int view);
    virtual const char * getDigest() const;
    virtual void setDigest(const char * digest);
    virtual const char * getSignature() const;
    virtual void setSignature(const char * signature);
    virtual const char * getClientMessageDigest() const;
    virtual void setClientMessageDigest(const char * digest);

    const ::omnetpp::simtime_t getTimestamp() const {
        return timestamp;
    }

    void setTimestamp(const ::omnetpp::simtime_t &timestamp) {
        this->timestamp = timestamp;
    }
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const ClientReply& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, ClientReply& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>ClientReply.msg:38</tt> by nedtool.
 * <pre>
 * message ReplyTimeout
 * {
 *     string request_digest;
 * }
 * </pre>
 */
class ReplyTimeout : public ::omnetpp::cMessage
{
  protected:
    ::omnetpp::opp_string request_digest;

  private:
    void copy(const ReplyTimeout& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const ReplyTimeout&);

  public:
    ReplyTimeout(const char *name=nullptr, short kind=0);
    ReplyTimeout(const ReplyTimeout& other);
    virtual ~ReplyTimeout();
    ReplyTimeout& operator=(const ReplyTimeout& other);
    virtual ReplyTimeout *dup() const override {return new ReplyTimeout(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual const char * getRequest_digest() const;
    virtual void setRequest_digest(const char * request_digest);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const ReplyTimeout& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, ReplyTimeout& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>ClientReply.msg:42</tt> by nedtool.
 * <pre>
 * message ClientACK
 * {
 *     string request_digest;
 * }
 * </pre>
 */
class ClientACK : public ::omnetpp::cMessage
{
  protected:
    ::omnetpp::opp_string request_digest;

  private:
    void copy(const ClientACK& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const ClientACK&);

  public:
    ClientACK(const char *name=nullptr, short kind=0);
    ClientACK(const ClientACK& other);
    virtual ~ClientACK();
    ClientACK& operator=(const ClientACK& other);
    virtual ClientACK *dup() const override {return new ClientACK(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual const char * getRequest_digest() const;
    virtual void setRequest_digest(const char * request_digest);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const ClientACK& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, ClientACK& obj) {obj.parsimUnpack(b);}


#endif // ifndef __CLIENTREPLY_M_H
