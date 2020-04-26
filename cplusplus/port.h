// Warning! This is not thread safe! Atomic check and connect/write/read
// needed for that!

#ifndef PENUX_PORT_H
#define PENUX_PORT_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>
#include <set>
#include <string>

#include <typeinfo>

#include "iprocessblock.h"

#define DEBUG
#undef DBG_LVL
#define DBG_LVL (DBG_ERR|DBG_WARN)

#include "debug.h"


namespace penux
{

using boost::shared_ptr;
using boost::weak_ptr;

using std::vector;
using std::set;
using std::string;


template <typename T> class OutPortType;

class iMediatorBase
{
public:
        virtual ~iMediatorBase() { }
        virtual const std::type_info& getInDataType() const  = 0;
        virtual const std::type_info& getOutDataType() const = 0;
        virtual iMediatorBase* createNew() const = 0;
};

template <typename OutT>
class iInMediator : public virtual iMediatorBase
{
public:
        virtual ~iInMediator() { }
        virtual OutT getData() const = 0;
};

template <typename InT>
class iOutMediator : public virtual iMediatorBase
{
public:
        virtual ~iOutMediator() { }
        virtual void setPort(OutPortType<InT>* pPort) = 0;
};

template <typename OutT, typename InT>
class iMediator : public virtual iInMediator<OutT>, public virtual iOutMediator<InT>
{
public:
        virtual ~iMediator() { }
        // Compilation only?
};

template <typename OutT, typename InT>
class MediatorBase : public virtual iMediator<OutT, InT>
{
public:
        MediatorBase() : pPort(0) { }
        virtual ~MediatorBase() { }
        virtual const std::type_info& getInDataType() const {return typeid(InT);}
        virtual const std::type_info& getOutDataType() const {return typeid(OutT);}
        virtual OutT getData() const = 0; //{return pOutMediator->getData();};
        virtual void setPort(OutPortType<InT>* pPort) {this->pPort = pPort;}
protected:
        OutPortType<InT>* pPort;
};

template <typename T>
class SimpleMediator : public virtual MediatorBase<T, T>
{
public:
        SimpleMediator() { }
        virtual ~SimpleMediator() { }
        virtual inline T getData() const {return this->pPort->getData();}
        virtual iMediatorBase* createNew() const {return new SimpleMediator<T>;}
};

class MediatorRegister
{
public:
        static MediatorRegister* getRegister();
        iMediatorBase* getMediator(const std::type_info& inType, const std::type_info& outType);
        void addMediator(iMediatorBase* pMediator);
private:
        static MediatorRegister* pRegister;

        iMediatorBase* findMediator(const std::type_info& inType, const std::type_info& outType);

        vector<iMediatorBase*> mediatorList; // Change to better (hash like) structure?
};

class iPort
{
public:
        virtual ~iPort(){};

        virtual bool isConnected() const = 0;
        virtual void disconnect() = 0;
        virtual iProcessBlock* getOwner() const = 0;
        virtual int getDuration() const = 0;
        virtual const std::type_info& getDataType() const  = 0;
};

class OutPortBase;

class InPortBase : public virtual iPort
{
public:
        virtual ~InPortBase();

        virtual bool connect(OutPortBase* pOutPort);
        virtual bool isConnected() const;

        virtual bool isReadable() const;

        virtual bool getDataPresent() const;

        // Used by OutPort to inform about new available data:
        // TODO: Move to other interface??!?
        virtual void setDataPresent(bool dataPresent);

        virtual void disconnect();
        virtual iProcessBlock* getOwner() const;
        virtual int getDuration() const;

        virtual OutPortBase* getConnectedPort() const;
protected:
        InPortBase(iProcessBlock* pOwner, int duration);

        virtual iMediatorBase* getMediator() = 0;
        virtual void setMediator(iMediatorBase* pMediator) = 0;

        iProcessBlock* pOwner;
        int duration;

        OutPortBase* pConnectedPort;
        bool dataPresent;
};

inline InPortBase::~InPortBase()
{
        ENTER("InPortBase::~InPortBase()");
        LEAVE("InPortBase::~InPortBase()");
}

inline InPortBase::InPortBase(iProcessBlock* pOwner, int duration)
  : pOwner(pOwner), duration(duration), pConnectedPort(0),
    dataPresent(false)
{
        ENTER("InPortBase::InPortBase()");
        LEAVE("InPortBase::InPortBase()");
}

inline bool InPortBase::isConnected() const
{
        ENTER("InPort<T>::isConneceted()");
        LEAVE("InPort<T>::isConneceted()");
        return (pConnectedPort);
}

inline bool InPortBase::isReadable() const
{
        ENTER("InPortBase::isReadable()");
        LEAVE("InPortBase::isReadable()");
        return getDataPresent();
}

inline bool InPortBase::getDataPresent() const
{
        return dataPresent;
}

inline void InPortBase::setDataPresent(bool dataPresent)
{
        this->dataPresent = dataPresent;
}

inline iProcessBlock* InPortBase::getOwner() const
{
        ENTER("InPortBase::getOwner()");
        LEAVE("InPortBase::getOwner()");
        return pOwner;
}


inline int InPortBase::getDuration() const
{
        ENTER("InPortBase::getDuration()");
        LEAVE("InPortBase::getDuration()");
        return duration;
}

inline OutPortBase* InPortBase::getConnectedPort() const
{
        ENTER("InPort<T>::getConnectedOutPort()");
        LEAVE("InPort<T>::getConnectedInPort()");
        return pConnectedPort;
}

class OutPortBase : public iPort
{
public:
        virtual ~OutPortBase();

        virtual bool connect(InPortBase* pInPort);
        virtual void disconnect();

        virtual bool isConnected() const;

        virtual bool isWritable() const;

        virtual iProcessBlock* getOwner() const;
        virtual int getDuration() const;

        virtual int getNumConnected() const;

        virtual int getNumStillProcessing() const;

        // Used by InPort during connect/disconnect:
        virtual void addInPort(InPortBase* pInPort);
        virtual void removeInPort(InPortBase* pInPort);

        virtual bool getConstReader() const;
        virtual void setConstReader(bool constReader);

        virtual void incrementNumStillProcessing();
        virtual void decrementNumStillProcessing();

        virtual InPortBase* getConnectedPort(int portNum) const;

        virtual iMediatorBase* getMediator(const std::type_info& type) = 0;

protected:
        OutPortBase(iProcessBlock* pOwner, int duration);

        virtual void removeMediator(const std::type_info& type) = 0;

        iProcessBlock* pOwner;
        int duration;

        vector<InPortBase*> inPortVector;

        bool constReader;
        int numStillProcessing;
};

inline OutPortBase::~OutPortBase()
{
        ENTER("OutPortBase::~OutPortBase()");
        LEAVE("OutPortBase::~OutPortBase()");
}

inline OutPortBase::OutPortBase(iProcessBlock* pOwner, int duration)
  : pOwner(pOwner), duration(duration), constReader(false), numStillProcessing(0)
{
        ENTER("OutPortBase::OutPortBase()");
        LEAVE("OutPortBase::OutPortBase()");
}


inline bool OutPortBase::isConnected() const
{
        return (getNumConnected() > 0);
}

inline bool OutPortBase::isWritable() const
{
        ENTER("OutPortBase::isWritable()");
        LEAVE("OutPortBase::isWritable()");
        return (getNumStillProcessing() == 0);
}

inline iProcessBlock* OutPortBase::getOwner() const
{
        ENTER("OutPortBase::getOwner()");
        LEAVE("OutPortBase::getOwner()");
        return pOwner;
}

inline int OutPortBase::getDuration() const
{
        ENTER("OutPortBase::getDuration()");
        LEAVE("OutPortBase::getDuration()");
        return duration;
}

inline int OutPortBase::getNumConnected() const
{
        ENTER("OutPortBase::getNumConnected()");
        LEAVE("OutPortBase::getNumConnected()");
        return inPortVector.size();
}

inline int OutPortBase::getNumStillProcessing() const
{
        ENTER("OutPortBase::getNumConnected()");
        LEAVE("OutPortBase::getNumConnected()");
        return numStillProcessing;
}

inline bool OutPortBase::getConstReader() const
{
        return constReader;
}

inline void OutPortBase::setConstReader(bool constReader)
{
        this->constReader = constReader;
}

inline void OutPortBase::incrementNumStillProcessing()
{
        ++numStillProcessing;
}

inline void OutPortBase::decrementNumStillProcessing()
{
        --numStillProcessing;
}

//----------------------------------------------------------------//

template <typename T>
class InPortType : public InPortBase
{
public:
        virtual ~InPortType();
        virtual const std::type_info& getDataType() const {return typeid(T);}
protected:
        InPortType(iProcessBlock* pOwner, int duration) : InPortBase(pOwner, duration) { }

        virtual iMediatorBase* getMediator();
        virtual void setMediator(iMediatorBase* pMediator);

        iInMediator<T>* pMediator;
};

template <typename T>
InPortType<T>::~InPortType()
{
        this->disconnect();
};

template <typename T>
iMediatorBase* InPortType<T>::getMediator()
{
        return pMediator;
}

template <typename T>
void InPortType<T>::setMediator(iMediatorBase* pMediator)
{
        this->pMediator = dynamic_cast<iInMediator<T>*>(pMediator);
}

template <typename T>
class InPort : public InPortType<shared_ptr<T> >
{
public:
        InPort(iProcessBlock* pOwner, int duration);
        virtual ~InPort();

        virtual void clearData();
        virtual shared_ptr<const T> getConstData();
        virtual shared_ptr<T> getData();
};

template <typename T>
InPort<T>::InPort(iProcessBlock* pOwner, int duration)
  : InPortType<shared_ptr<T> >(pOwner, duration)
{
        ENTER("InPort<T>::InPort()");
        LEAVE("InPort<T>::InPort()");
}

template <typename T>
inline InPort<T>::~InPort()
{
        ENTER("InPort<T>::~InPort()");
        LEAVE("InPort<T>::~InPort()");
}

template <typename T>
void InPort<T>::clearData()
{
        ENTER("InPort<T>::clearData()");
        if (this->isReadable())
        {
                this->setDataPresent(false);
                this->pConnectedPort->decrementNumStillProcessing();
        }
        else
        {
                string blockName = (this->pOwner) ? this->pOwner->getName() : "Unknown";
                ERROR("InPort::clearData() - No data! (In port belongs to "<< blockName << ").");
        }
        LEAVE("InPort<T>::clearData()");
}

template <typename T>
shared_ptr<const T> InPort<T>::getConstData()
{
        ENTER("InPort<T>::getConstData()");
        if (this->isReadable())
        {
                shared_ptr<const T> pConstData = this->pMediator->getData();
                this->pConnectedPort->setConstReader(true);
                this->setDataPresent(false);
                this->pConnectedPort->decrementNumStillProcessing();
                LEAVE("InPort<T>::getConstData()");
                return pConstData;
        }
        else
        {
                string blockName = (this->pOwner) ? this->pOwner->getName() : "Unknown";
                ERROR("InPort<T>::getConstData() - No data! (In port belongs to " << blockName << ").");
                LEAVE("InPort<T>::getConstData()");
                return shared_ptr<const T>();;
        }
}

template <typename T>
shared_ptr<T> InPort<T>::getData()
{
        ENTER("InPort<T>::getData()");
        if (this->isReadable())
        {
                shared_ptr<T> pRetData = this->pMediator->getData();
                if (this->pConnectedPort->getNumStillProcessing() > 1 || this->pConnectedPort->getConstReader())
                {
                        // More people are using this pointer. Make copy of data
                        if (pRetData)
                        {
                                pRetData = shared_ptr<T>(new T(*pRetData));
                        }
                        else
                        {
                                pRetData = shared_ptr<T>();
                        }
                }
                this->setDataPresent(false);
                this->pConnectedPort->decrementNumStillProcessing();
                LEAVE("InPort<T>::getConstData()");
                return pRetData;
        }
        else
        {
                string blockName = (this->pOwner) ? this->pOwner->getName() : "Unknown";
                ERROR("InPort<T>::getData() - No data! (In port belongs to " << blockName << ").");
                LEAVE("InPort<T>::getConstData()");
                return shared_ptr<T>();
        }
}

// ---------------------------------------------------------- //

template <typename T>
class OutPortType : public OutPortBase
{
public:
        virtual ~OutPortType();
        virtual T getData() {return data;}
        virtual const std::type_info& getDataType() const {return typeid(T);}
        virtual iMediatorBase* getMediator(const std::type_info& type);
protected:
        OutPortType(iProcessBlock* pOwner, int duration);
        virtual void removeMediator(const std::type_info& type);

        struct MediatorCount
        {
                MediatorCount(iOutMediator<T>* pMediator = 0, int count = 1) : pMediator(pMediator), count(count) {};
                iOutMediator<T>* pMediator;
                int count;
        };

        MediatorRegister* pRegister;
        vector<MediatorCount> mediatorList;

        T data;
};

template <typename T>
OutPortType<T>::~OutPortType()
{
        this->disconnect();
}

template <typename T>
OutPortType<T>::OutPortType(iProcessBlock* pOwner, int duration)
  : OutPortBase(pOwner, duration)
{
        pRegister = MediatorRegister::getRegister();
        SimpleMediator<T>* pMediator = new SimpleMediator<T>;
        pRegister->addMediator(pMediator);
}

template <typename T>
iMediatorBase* OutPortType<T>::getMediator(const std::type_info& type)
{
        // search vector:
        iMediatorBase* pMediator = 0;
        bool inList = false;
        typename vector<MediatorCount>::iterator it = mediatorList.begin();
        for (; !inList && it != mediatorList.end(); ++it)
        {
                if (it->pMediator->getOutDataType() == type)
                {
                        inList = true;
                        ++it->count;
                        pMediator = it->pMediator;
                }
        }

        if (!inList)
        {
                pMediator = pRegister->getMediator(typeid(T), type);
                if (pMediator)
                {
                        iOutMediator<T>* pOutMediator = dynamic_cast<iOutMediator<T>*>(pMediator);
                        pOutMediator->setPort(this);
                        mediatorList.push_back(MediatorCount(pOutMediator));
                }
        }
        return pMediator;
}

template <typename T>
void OutPortType<T>::removeMediator(const std::type_info& type)
{
        typename vector<MediatorCount>::iterator it = mediatorList.begin();
        for (; it != mediatorList.end(); ++it)
        {
                if (it->pMediator->getOutDataType() == type)
                {
                        --it->count;
                        if (it->count == 0)
                        {
                                mediatorList.erase(it);
                        }
                        break;
                }
        }
}

template <typename T>
class OutPort : public OutPortType<shared_ptr<T> >
{
public:
        OutPort(iProcessBlock* pOwner, int duration);
        virtual ~OutPort();

        virtual void sendEmptyData();
        virtual void sendData(shared_ptr<T> pData);
};

template <typename T>
inline OutPort<T>::OutPort(iProcessBlock* pOwner, int duration)
  : OutPortType<shared_ptr<T> >(pOwner, duration)
{
        ENTER("OutPort<T>::OutPort(iProcessBlock*, int)");
        LEAVE("OutPort<T>::OutPort(iProcessBlock*, int)");
}

template <typename T>
inline OutPort<T>::~OutPort()
{
        ENTER("OutPort<T>::~OutPort");
        LEAVE("OutPort<T>::~OutPort");
}

template <typename T>
void OutPort<T>::sendEmptyData()
{
        ENTER("OutPort<T>::sendEmptyData()");
        sendData(shared_ptr<T>());
        LEAVE("OutPort<T>::sendEmptyData()");
}

template <typename T>
void OutPort<T>::sendData(shared_ptr<T> pData)
{
        ENTER("OutPort<T>::sendData(shared_ptr)");

        if (this->isWritable())
        {
                this->data = pData;
                vector<InPortBase*>::iterator itPV;
                for (itPV = this->inPortVector.begin(); itPV != this->inPortVector.end(); ++itPV)
                {
                        (*itPV)->setDataPresent(true);
                }
                this->numStillProcessing = this->getNumConnected();
                this->constReader = false;
        }
        else
        {
                string blockName = (this->pOwner) ? this->pOwner->getName() : "Unknown";
                ERROR("Cannot write, old data not processed yet! (Port belongs to " << blockName <<").");
        }

        LEAVE("OutPort<T>::sendData(shared_ptr)");
}

} // end namespace penux

#endif // PENUX_PORT_H
