/*
 * pbwrap.h
 *
 *  Created on: 29 jan 2010
 *      Author: jln
 */

#ifndef PBWRAP_H_
#define PBWRAP_H_

#include <boost/python.hpp>

#include "iprocessblock.h"
#include "processblock.h"

namespace penux
{

using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(cOP_overloads1, connectOutPort, 2, 3);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(cOP_overloads2, connectOutPort, 1, 2);
void (BasicProcessBlock::*cOP1)(int, iProcessBlock*, int) =
  &BasicProcessBlock::connectOutPort;
void (BasicProcessBlock::*cOP2)(iProcessBlock*, int) =
  &BasicProcessBlock::connectOutPort;


BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(cIP_overloads1, connectInPort, 2, 3);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(cIP_overloads2, connectInPort, 1, 2);
void (BasicProcessBlock::*cIP1)(int, iProcessBlock*, int) =
  &BasicProcessBlock::connectInPort;
void (BasicProcessBlock::*cIP2)(iProcessBlock*, int) =
  &BasicProcessBlock::connectInPort;

class BasicProcessBlockWrap : public BasicProcessBlock, public wrapper<BasicProcessBlock>
{
public:
        BasicProcessBlockWrap(string name = "BasicProcessBlock", int duration = 1)
          : BasicProcessBlock(name, duration),
            wrapper<BasicProcessBlock>() { }

        virtual void run(int time) {this->get_override("run")(time);}

        virtual bool readyToRun()
        {
                if (override readyToRun = this->get_override("readyToRun"))
                {
                        return readyToRun();
                }
                return BasicProcessBlock::readyToRun();
        }
        bool d_readyToRun()
        {
                return BasicProcessBlock::readyToRun();
        }

        virtual void simSetUp(int time)
        {
                if (override simSetUp = this->get_override("simSetUp"))
                {
                        simSetUp(time);
                        return;
                }
                BasicProcessBlock::simSetUp(time);
        }

        void d_simSetUp(int time)
        {
                BasicProcessBlock::simSetUp(time);
        }

        virtual void reset()
        {
                if (override reset = this->get_override("reset"))
                {
                        reset();
                        return;
                }
                BasicProcessBlock::reset();
        }

        void d_reset()
        {
                BasicProcessBlock::reset();
        }

        void addInPort(InPortBase* pInPort)
        {
                BasicProcessBlock::addInPort(pInPort);
        }

        void addOutPort(OutPortBase* pOutPort)
        {
                BasicProcessBlock::addOutPort(pOutPort);
        }
};

} // End name space

#endif /* PBWRAP_H_ */
