#ifndef PROCESSBLOCK_H_
#define PROCESSBLOCK_H_

#include <boost/shared_ptr.hpp>
#include <vector>

#include "port.h"
#include "iprocessblock.h"

//#define DBG_LVL (DBG_ERR|DBG_WARN|DBG_INFO|DBG_ENTER|DBG_LEAVE)

#define DEBUG
#undef DBG_LVL
#define DBG_LVL (DBG_ERR|DBG_WARN|DBG_INFO)

#include "debug.h"

namespace penux
{

using std::vector;
using std::string;

using boost::shared_ptr;

// Move to some other file?
void connectBlocks(iProcessBlock* pOutBlock, int numOut, iProcessBlock* pInBlock, int numIn = 0);
void connectBlocks(iProcessBlock* pOutBlock, iProcessBlock* pInBlock, int numIn = 0);

void connectBlocks(shared_ptr<iProcessBlock> pOutBlock, int numOut, shared_ptr<iProcessBlock> pInBlock, int numIn = 0);
void connectBlocks(shared_ptr<iProcessBlock> pOutBlock, shared_ptr<iProcessBlock> pInBlock, int numIn = 0);


// Setting 
class BasicProcessBlock : public iProcessBlock
{
public:
        virtual ~BasicProcessBlock();

        virtual bool readyToRun();

        virtual void simSetUp(int time);
        virtual void reset();

        virtual void disconnectOutPort(int portNum = 0);

        virtual void disconnectInPort(int portNum = 0);

        virtual string getName() const;

        virtual int getNumOutPorts() const;
        virtual int getNumInPorts() const;

        virtual OutPortBase* getOutPort(int) const;
        virtual InPortBase* getInPort(int) const;

        virtual int getDuration() const;

protected:
        BasicProcessBlock(string name = "BasicProcessBlock", int duration = 1);

        virtual void connectOutPort_impl(int numOut, iProcessBlock* pOther, int numIn);
        virtual void connectInPort_impl(int numIn, iProcessBlock* pOther, int numOut);

        void addInPort(InPortBase* pInPort);
        void addOutPort(OutPortBase* pOutPort);

        string name;

        vector<InPortBase*> vInPort;
        vector<OutPortBase*> vOutPort;

        int duration;

}; //BasicProcessBlock

inline
BasicProcessBlock::BasicProcessBlock(string name, int duration)
  : name(name), duration(duration)
{
        ENTER("BasicProcessBlock::BasicProcessBlock() (" << name << ")");
        LEAVE("BasicProcessBlock::BasicProcessBlock() (" << name << ")");
        return;
}

inline void
BasicProcessBlock::simSetUp(int time)
{
        ENTER("BasicProcessBlock::getBlockName() (" << name << ")");
        // Empty function, can be overridden to allow e.g. feed back, by setting initial output
        LEAVE("BasicProcessBlock::getBlockName() (" << name << ")");
}

inline void
BasicProcessBlock::reset()
{
        ENTER("BasicProcessBlock::reset() (" << name << ")");
        // Reset states in block if simulation is restarted.
        LEAVE("BasicProcessBlock::reset() (" << name << ")");
}

inline string
BasicProcessBlock::getName() const
{
        ENTER("BasicProcessBlock::getName() (" << name << ")");
        LEAVE("BasicProcessBlock::getName() (" << name << ")");
        return name;
}

inline int
BasicProcessBlock::getNumOutPorts() const
{
        ENTER("BasicProcessBlock::getNumOutPorts() (" << name << ")");
        LEAVE("BasicProcessBlock::getNumOutPorts() (" << name << ")");
        return vOutPort.size();
}

inline int
BasicProcessBlock::getNumInPorts() const
{
        ENTER("BasicProcessBlock::getNumInPorts() (" << name << ")");
        LEAVE("BasicProcessBlock::getNumInPorts() (" << name << ")");
        return vInPort.size();
}


inline int
BasicProcessBlock::getDuration() const
{
        ENTER("BasicProcessBlock::getDuration() (" << name << ")");
        LEAVE("BasicProcessBlock::getDuration() (" << name << ")");
        return duration;
}

inline void
BasicProcessBlock::addInPort(InPortBase* pInPort)
{
        vInPort.push_back(pInPort);
}

inline void
BasicProcessBlock::addOutPort(OutPortBase* pOutPort)
{
        vOutPort.push_back(pOutPort);
}

} // Namespace penux

#endif /*PROCESSBLOCK_H_*/