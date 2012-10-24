FAQ
===

Boost libraries
---------------

.. topic:: Custom boost libraries compilation problem

    I'm trying to use custom boost libraries, but something is going wrong. What should I do?

Please refer to :ref:`boost-custom-install` section for an example how custom boost libraries could be built.


Visualizer problems
-------------------

.. topic:: Visualizer module is not working

    Every time I'm trying to run visualizer, I get the following error::

        Waf: Entering directory `/ndnSIM/ns-3/build'
        Could not find a task generator for the name 'ns3-visualizer'..

Something is wrong with your python bindings and python bindings dependencies. 
Please follow the :ref:`requirements` section that lists what should be installed in order to run visualizer.

Code questions
--------------

.. topic:: Failing a link between nodes

    How can I fail a link between to NDN nodes?


Right now, NS-3 does not provide ability to actually "break" the link between nodes in NS-3.
However, exactly the same effect can be achieved by making an interface (:ndnsim:`ndn::Face`) up or down (:ndnsim:`ndn::Face::SetUp(true)` or :ndnsim:`ndn::Face::SetUp(false)`).

Here is an example of function to "fail" a point-to-point link between two NDN nodes:

.. code-block:: c++

    // hijacker is more than an application. just disable all faces
    void
    FailLinks (Ptr<Node> node1, Ptr<Node> node2)
    {
      Ptr<ndn::L3Protocol> ndn1 = node1->GetObject<ndn::L3Protocol> ();
      Ptr<ndn::L3Protocol> ndn2 = node2->GetObject<ndn::L3Protocol> ();
    
      // iterate over all faces to find the right one
      for (uint32_t faceId = 0; faceId < ndn1->GetNFaces (); faceId++)
        {
          Ptr<ndn::NetDeviceFace> ndFace = ndn1->GetFace (faceId)->GetObject<ndn::NetDeviceFace> ();
          if (ndFace == 0) continue;
     
          Ptr<PointToPointNetDevice> nd1 = ndFace->GetNetDevice ()->GetObject<PointToPointNetDevice> ();
          if (nd1 == 0) continue;
     
          Ptr<Channel> channel = nd1->GetChannel ();
          if (channel == 0) continue;
     
          Ptr<PointToPointChannel> ppChannel = DynamicCast<PointToPointChannel> (channel);
     
          Ptr<NetDevice> nd2 = ppChannel->GetDevice (0);
          if (nd2->GetNode () == node1)
            nd2 = ppChannel->GetDevice (1);
     
          if (nd2->GetNode () == node2)
            {
              Ptr<ndn::Face> face1 = ndn1->GetFaceByNetDevice (nd1);
              Ptr<ndn::Face> face2 = ndn2->GetFaceByNetDevice (nd2);
     
              face1->SetUp (false);
              face2->SetUp (false);
              break;
            }
        }
    }
     

General questions
-----------------

.. topic:: Errors/bugs reporting

    I found an error in the documentation / bug in the code. What should I do?

Please create an issue for the documentation error or code bug.  
We will try to resolve the problem as fast as we can.
