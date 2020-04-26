/*
 * iprocessblock.h
 *
 *  Created on: 11 Jan 2010
 *      Author: phg
 */

#ifndef IPROCESSBLOCK_H_
#define IPROCESSBLOCK_H_

#include <string>
#include <boost/shared_ptr.hpp>

namespace penux
{
using boost::shared_ptr;
using std::string;

class OutPortBase;
class InPortBase;
class iProcessBlock
{
public:

        virtual ~iProcessBlock(){/*Do nothing*/};

        virtual void run(int time)=0;
        virtual bool readyToRun()=0;

        virtual void simSetUp(int time) = 0;
        virtual void reset() = 0;

        void connectOutPort(int numOut, iProcessBlock* pOther, int numIn = 0)
        {
                connectOutPort_impl(numOut, pOther, numIn);
        }

        void connectOutPort(iProcessBlock* pOther, int numIn = 0)
        {
                connectOutPort_impl(0, pOther, numIn);
        }

        virtual void disconnectOutPort(int portNum) = 0;

        void connectInPort(int numIn, iProcessBlock* pOther, int numOut = 0)
        {
                connectInPort_impl(numIn, pOther, numOut);
        }

        void connectInPort(iProcessBlock* pOther, int numOut = 0)
        {
                connectInPort_impl(0, pOther, numOut);
        }

        virtual void disconnectInPort(int portNum) = 0;

        virtual int getNumOutPorts() const = 0;
        virtual int getNumInPorts() const = 0;

        virtual OutPortBase* getOutPort(int) const = 0;
        virtual InPortBase* getInPort(int) const = 0;

        virtual int getDuration() const = 0;

        virtual string getName() const = 0;

protected:
        virtual void connectOutPort_impl(int numOut, iProcessBlock* pOther, int numIn) = 0;
        virtual void connectInPort_impl(int numIn, iProcessBlock* pOther, int numOut) = 0;

};//iProcessBlock

} // namespace penux

#endif /* IPROCESSBLOCK_H_ */