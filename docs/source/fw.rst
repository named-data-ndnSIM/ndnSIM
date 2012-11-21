
Forwarding Strategies
=====================

ndnSIM provides simple ways to experiment with custom Interest/Data forwarding strategies. 
A new forwarding strategy can be implement completely different processing or override just specific actions/events of the :ndnsim:`forwarding strategy interface <ndn::ForwardingStrategy>`.
Please refer to :ndnsim:`API documentation <ndn::ForwardingStrategy>` of the forwarding strategy interface, which lists all default actions/events.

Writing your own custom strategy
++++++++++++++++++++++++++++++++

First step in creating your own strategy is to decide which existing strategy you want to extend.  You can either use realize :ndnsim:`forwarding strategy interface <ndn::ForwardingStrategy>` (:ndnsim:`ndn::ForwardingStrategy::DoPropagateInterest` call must be implemented) or extended one of the available forwarding strategies (:ndnsim:`fw::BestRoute` or :ndnsim:`fw::Flooding`).  
The following example assumes that we are realizing :ndnsim:`forwarding strategy interface <ndn::ForwardingStrategy>`. 

The follwoing are template strategy h/cc files:

.. literalinclude:: _static/code-samples/custom-strategy.h
   :language: c++
   :linenos:
   :lines: 1-36,51-55,59-

.. literalinclude:: _static/code-samples/custom-strategy.cc
   :language: c++
   :linenos:
   :lines: 1-40,42-50,75-76,115-

After having the template, we can fill the necesasry functionality.  

Let us say, that we want Interest be forwarded to first two best-metric faces specified by FIB. 
That is, if node has two or more alternative paths to forward the Interests, this Interest will be forwarded to the best two neighbors.
The following implementation of CustomStrategy::DoPropagateInterest accomplishes the task:

.. literalinclude:: _static/code-samples/custom-strategy.cc
   :language: c++
   :linenos:
   :lines: 45-75
   :emphasize-lines: 7-30

After the compilation, you can use ``"ns3::ndn::fw::CustomStrategy"`` as a parameter to :ndnsim:`ndn::StackHelper::SetForwardingStrategy` helper method.

 .. as well as NS_LOG=ndn.fw.CustomStrategy when running in a debug mode

Extending strategy
++++++++++++++++++

If you need more customization for the forwarding strategy, there are many forwarding strategy events that can be overriden.
For example, if we want to perform special logging of all forwarded, timed out, and satisfied Intersts, we can override the following events (for more events, refer to :ndnsim:`ForwardingStrategy API documentation <ForwardingStrategy>`):

- :ndnsim:`DidSendOutInterest <ForwardingStrategy::DidSendOutInterest>`, which fired just after forwarding the Interest

- :ndnsim:`WillEraseTimedOutPendingInterest <ForwardingStrategy::WillEraseTimedOutPendingInterest>`, which fired just before PIT entry is removed by timeout

- :ndnsim:`WillSatisfyPendingInterest <ForwardingStrategy::WillSatisfyPendingInterest>`, which fired just before Interest will be satisfied.

The highlighted ares of the following code demonstrates how it can be impelmented:

.. literalinclude:: _static/code-samples/custom-strategy.h
   :language: c++
   :linenos:
   :emphasize-lines: 37-50,56-58

.. literalinclude:: _static/code-samples/custom-strategy.cc
   :language: c++
   :linenos:
   :emphasize-lines: 41,51-74,77-114
